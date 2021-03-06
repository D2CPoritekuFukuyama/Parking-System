//
//  labeling.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/11.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "labeling.hpp"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"

#define TOLERANCE 50

using namespace cv;


Labeling::Labeling(){
    videoCapture = cvCreateCameraCapture( 0 );
    cvSetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_WIDTH, 1280);
    cvSetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_HEIGHT, 720);
    frame = cvQueryFrame(videoCapture);
    gray_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    bin_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    resutl_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
    Nplate_rect = cv::Rect(0,0,0,0);
}

void Labeling::DrawNextContour(
                IplImage *img,//ラベリング結果を描画するIplImage(8Bit3chカラー）
                CvSeq *Contour, //輪郭へのポインタ
                int Level //輪郭のレベル（階層）
){
    // 輪郭を描画する色の設定
    CvScalar ContoursColor;
    
    if ((Level % 2) == 1){
        //白の輪郭の場合、赤で輪郭を描画
        ContoursColor = CV_RGB( 255, 0, 0 );
    }else{
        //黒の輪郭の場合、青で輪郭を描画
        ContoursColor = CV_RGB( 0, 0, 255 );
    }
    
    
    for (; Contour != 0; Contour = Contour ->h_next) {
        //輪郭のポリゴン近似
        CvSeq *approx = cvApproxPoly(Contour, sizeof(CvContour), NULL, CV_POLY_APPROX_DP, 20); //25

        //面積
        //double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
        
        //頂点四つ且つcheck_rectangle
        if(approx->total == 4 && check_rectangle(approx))
        {
            cvDrawContours( img, approx, ContoursColor, ContoursColor, 0, 2);
            if (Contour -> h_next != NULL){
                DrawNextContour(img, approx->h_next, Level);
                //trimming(gray_img);
            }
        }
    }
}

bool Labeling::check_rectangle(CvSeq *Nplate_point){
    int target[4][2];   //4つの座標格納
    int diffe[2][2];    //それぞれの差分
    //四角形かどうか

        //4角形の座標をtargetに格納
    for (int i = 0; i < Nplate_point -> total; ++i) {
        CvPoint *p = (CvPoint*)cvGetSeqElem(Nplate_point, i);
        target[i][0] = p -> x;
        target[i][1] = p -> y;
    }
    //対角線上の頂点座標の差の絶対値をとってdiffeに格納
    for (int i = 0; i < 2; i++) {
        diffe[i][0] = abs(target[i+2][0] - target[i][0]);
        diffe[i][1] = abs(target[i+2][1] - target[i][1]);
    }
   
    //1:2の比率に近く、直角に近ければtrue
    if(abs(diffe[0][0] - (2 * diffe[0][1])) <= TOLERANCE)
    {
        if(abs(diffe[1][0] - (2 * diffe[1][1])) <= TOLERANCE){
            if(abs(diffe[0][0] - diffe[1][0]) <= TOLERANCE){
                if(abs(diffe[0][1] - diffe[1][1]) <= TOLERANCE){
                    //4点を包含する矩形を求める
                    Nplate_rect = cvBoundingRect(Nplate_point, 0);
                    return true;
                }
            }
        }
    }
    return false;
    
}

//ラベリング関数
void Labeling::cv_Labelling(
                IplImage *src_img,
                IplImage *dst_img
                    ){
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *contours = NULL;
    if(src_img == NULL)
        return;
    
    int contour_count = cvFindContours(src_img,
                                       storage,
                                       &contours,
                                       sizeof(CvContour),
                                       CV_RETR_EXTERNAL,
                                       CV_CHAIN_APPROX_SIMPLE);
    if (contour_count <= 0)
        return ;
    
    if(contours != NULL){
        cvSet(dst_img, CV_RGB(0, 0, 0));
        DrawNextContour(dst_img, contours, 1);
    }
    resutl_img = dst_img;
    cvShowImage("Labeling", resutl_img);
    cvReleaseMemStorage(&storage);
}

//トリミング関数
//ラベリング時に取得したナンバープレートの点列を用いてトリミング
void Labeling::trimming(IplImage *src_img){
    Mat src_mat = src_img;
    
    Mat cut_img(src_mat, Nplate_rect); //プレートのトリミング
    Nplate_point = cut_img;
    resize(Nplate_point, Nplate_point, cvSize(300, 150));  //200 x 100にリサイズ
    Mat Nplate_up1(Nplate_point, Rect(65, 0, 95, 55));
    Mat Nplate_up2(Nplate_point, Rect(160,0, 85, 55));
    Mat Nplate_up(Nplate_point, Rect(65,0,180, 55));
    Mat Nplate_down(Nplate_point, Rect(10,50,285,100));

    contrast_correct(Nplate_point);
    /*imshow("Nplate-up", Nplate_up);
    imshow("Nplate-down", Nplate_down);
    imwrite("image/Nplate.jpg", Nplate_point);
    imwrite("image/Nplate-up.jpg", Nplate_up);*/
    imwrite("image/Nplate-up1.jpg", Nplate_up1);
    imwrite("image/Nplate-cate.jpg", Nplate_up2);
    imwrite("image/Nplate-down.jpg", Nplate_down);
}

void Labeling::contrast_correct(Mat img){
    // ルックアップテーブル作成
    float a = 20.0; // 入力パラメータ
    uchar lut[256];
    for (int i = 0; i < 256; i++)
        lut[i] = 255.0 / (1+exp(-a*(i-128)/255));
    cv::LUT(img, cv::Mat(cvSize(256, 1), CV_8U, lut),img);
}

//2値化関数
void Labeling::Binarization(
                  IplImage *src_img,
                  IplImage *dst_img
                  ){
    IplImage *bin_img1 = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
    IplImage *bin_img2 = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
    cvThreshold(src_img, bin_img1, 165, 255, CV_THRESH_BINARY); //閾値165で2値化
    //適応的閾値処理
    cvAdaptiveThreshold(src_img, bin_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY);
    cvAnd(bin_img1, bin_img2, dst_img); //二つの２値化画像の論理積
    //cvShowImage("bin1", bin_img1);
    //cvShowImage("bin2", bin_img2);
    //cvShowImage("result", dst_img);
}
