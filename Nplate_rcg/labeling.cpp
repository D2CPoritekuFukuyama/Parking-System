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
#include "WarpPerspective.hpp"
#include <math.h>

//#define VISUAL
#define TOLERANCE 90

using namespace cv;

Labeling::Labeling(){
    videoCapture = cvCreateCameraCapture( 0 );
    cvSetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_WIDTH, 480);
    cvSetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_HEIGHT, 320);
    frame = cvQueryFrame(videoCapture);
    gray_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    bin_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    
    Area = 0;
}

void Labeling::DrawNextContour(
                CvSeq *Contour, //輪郭へのポインタ
                int Level //輪郭のレベル（階層）
){
    for (; Contour != 0; Contour = Contour ->h_next) {
        //輪郭のポリゴン近似
        CvSeq *approx = cvApproxPoly(Contour, sizeof(CvContour), NULL, CV_POLY_APPROX_DP, 20); //25
        //頂点四つ且つcheck_rectangle
        if(approx->total == 4)
        {
            WarpPerspective warpPerspective = WarpPerspective(frame, approx);
            
            
            if (check_rectangle(warpPerspective.hull)) {
                warpPerspective.conversion();
                #ifdef VISUAL
                    draw_poly(approx);
                #endif
#if !defined VISUAL
                trimming(warpPerspective.dst_img);
#endif
                if (Contour -> h_next != NULL){
                    DrawNextContour(approx->h_next, Level);
                }
            }
        }
    }
}


void Labeling::draw_poly(CvSeq *approx){
    int npts[1] = {4};
    CvPoint **pts;
    pts = (CvPoint **)cvAlloc(sizeof(CvPoint*));
    pts[0] = (CvPoint *)cvAlloc(sizeof(CvPoint) * 4);
//4角形の座標をtargetに格納
    for (int i = 0; i < approx -> total; ++i) {
        pts[0][i] = *(CvPoint*)cvGetSeqElem(approx, i);
    }
    cvPolyLine(frame, pts, npts, 1, true, CV_RGB(255, 0, 0),4);
    moveWindow("drawPoly", 500, 0);
    cvShowImage("drawPoly", frame);
}


bool Labeling::check_rectangle(CvSeq *Nplate_seq){
    int i;
    Mat angle1, angle2,magnitude;
    CvPoint target[4];   //4つの座標格納
    CvPoint diffe[2];    //それぞれの差分
    //四角形かどうか
    
        //4角形の座標をtargetに格納
    for (i = 0; i < Nplate_seq -> total; ++i) {
        target[i] = **CV_GET_SEQ_ELEM( CvPoint*, Nplate_seq, i );
    }
    
    cartToPolar(abs(target[0].x - target[3].x), abs(target[0].y - target[3].y), magnitude, angle1, true);
    cartToPolar(abs(target[1].x - target[2].x), abs(target[1].y - target[2].y), magnitude, angle2, true);

    
    //対角線上の頂点座標の差の絶対値をとってdiffeに格納
    for (i = 0; i < 2; i++) {
        diffe[i] = cvPoint(abs(target[i+2].x - target[i].x), abs(target[i+2].y - target[i].y));
    }
    
    
    //角度でフィルタリング
    std::cout << angle1.at<double>(0) << " "<< angle2.at<double>(0) << std::endl;
        if(angle2.at<double>(0) - angle1.at<double>(0) < 10){
            if (abs(diffe[0].x - (2*diffe[0].y)) < 100) {
                if (abs(diffe[1].x - (2*diffe[1].y)) < 100) {
                    return true;
                }
            }
        }
    
        return false;
}

//ラベリング関数
void Labeling::cv_Labelling(){
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *contours = NULL;
    
    cvCvtColor(frame, gray_img, CV_RGB2GRAY);
    Binarization();
    if(bin_img == NULL)
        return;
    
    //輪郭抽出
    int contour_count = cvFindContours(bin_img,
                                       storage,
                                       &contours,
                                       sizeof(CvContour),
                                       CV_RETR_EXTERNAL,
                                       CV_CHAIN_APPROX_SIMPLE);
    if (contour_count <= 0)
        return ;
    
    if(contours != NULL){
        //cvSet(result_img, CV_RGB(0, 0, 0));
        DrawNextContour(contours, 1);
    }
    
    #ifdef VISUAL
        //cvShowImage("Labeling", resutl_img);
    #endif
    Area = 0;
    cvReleaseMemStorage(&storage);
}

//トリミング関数
//ラベリング時に取得したナンバープレートの点列を用いてトリミング
void Labeling::trimming(IplImage *src_img){
    IplImage *gray_src = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
    cvCvtColor(src_img, gray_src, CV_RGB2GRAY);
    result_img = cvCreateImage(cvSize(300, 150), IPL_DEPTH_8U, 1);
    
    cvResize(gray_src, result_img);
    
    Mat Nplate_up1(result_img, Rect(65, 0, 95, 55));
    Mat Nplate_up2(result_img, Rect(160,0, 85, 55));
    Mat Nplate_up(result_img, Rect(65,0,180, 55));
    Mat Nplate_down(result_img, Rect(10,50,285,100));

    contrast_correct(result_img);
    /*imshow("Nplate-up", Nplate_up);
    imshow("Nplate-down", Nplate_down);*/

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
void Labeling::Binarization(){
    IplImage *bin_img1 = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
    IplImage *bin_img2 = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
    cvThreshold(gray_img, bin_img1, 165, 255, CV_THRESH_BINARY); //閾値165で2値化
    //適応的閾値処理
    cvAdaptiveThreshold(gray_img, bin_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY);
    cvAnd(bin_img1, bin_img2, bin_img); //二つの２値化画像の論理積
#ifdef VISUAL
    moveWindow("bin", 500, 500);
    cvShowImage("bin", bin_img);
#endif
    //cvShowImage("bin1", bin_img1);
    //cvShowImage("bin2", bin_img2);
    //cvShowImage("result", dst_img);
}
