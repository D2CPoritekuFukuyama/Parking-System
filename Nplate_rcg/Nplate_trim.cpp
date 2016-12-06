//
//  Nplate_trim.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/03.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "Nplate_trim.hpp"
#include "WarpPerspective.hpp"

using namespace cv;

Nplate_trim::Nplate_trim(){
    videoCapture = cvCreateCameraCapture( 0 );
    cvSetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_WIDTH, 480);
    cvSetCaptureProperty(videoCapture, CV_CAP_PROP_FRAME_HEIGHT, 320);
    frame = cvQueryFrame(videoCapture);
    if (frame == NULL)
        return;
    
    gray_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    bin_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	result_img = NULL;
    contours = NULL;
}


bool Nplate_trim::check_rectangle(CvSeq *Nplate_seq){
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

void Nplate_trim::draw_poly(CvSeq *approx){
    int npts[1] = {4};
    CvPoint **pts;
    pts = (CvPoint **)cvAlloc(sizeof(CvPoint*));
    pts[0] = (CvPoint *)cvAlloc(sizeof(CvPoint) * 4);
    //4角形の座標をtargetに格納
    for (int i = 0; i < approx -> total; ++i) {
        pts[0][i] = *(CvPoint*)cvGetSeqElem(approx, i);
    }
    cvPolyLine(frame, pts, npts, 1, true, CV_RGB(255, 0, 0),4);
    cvShowImage("drawPoly", frame);
}

void Nplate_trim::DrawNextContour(
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
                cvShowImage("test", warpPerspective.dst_img);
                draw_poly(approx);
                
                trimming(warpPerspective.dst_img);
                if (Contour -> h_next != NULL){
                    DrawNextContour(approx->h_next, Level);
                }
            }
        }
    }
}

void Nplate_trim::contrast_correct(Mat img){
    // ルックアップテーブル作成
    float a = 20.0; // 入力パラメータ
    uchar lut[256];
    for (int i = 0; i < 256; i++)
        lut[i] = 255.0 / (1+exp(-a*(i-128)/255));
    cv::LUT(img, cv::Mat(cvSize(256, 1), CV_8U, lut),img);
}


//トリミング関数
//ラベリング時に取得したナンバープレートの点列を用いてトリミング
void Nplate_trim::trimming(IplImage *src_img){
    Ptr<IplImage> gray_src = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
    cvCvtColor(src_img, gray_src, CV_RGB2GRAY);
    result_img = cvCreateImage(cvSize(300, 150), IPL_DEPTH_8U, 1);
    std::cout << "get result_img" <<std::endl; 
    cvResize(gray_src, result_img);
    contrast_correct(result_img);
    
    cvSetImageROI(result_img, Rect(65,0,180, 55));
    nplate_down = cvCloneImage(result_img);
    cvSaveImage("image/test/Nplate-down.jpg", result_img);
    cvResetImageROI(result_img);
    cvSetImageROI(result_img, Rect(Rect(10,50,285,100)));
    nplate_up = cvCloneImage(result_img);
    cvSaveImage("image/test/Nplate.jpg", result_img);
    cvResetImageROI(result_img);
    
    /*imshow("Nplate-up", Nplate_up);
     imshow("Nplate-down", Nplate_down);*/
    
    cvSaveImage("image/Nplate.jpg", result_img);
    
}

int Nplate_trim::get_Nplate(){
    cv_Labelling(&contours, frame);
    if (contours != NULL){
        DrawNextContour(contours, 1);
        return 0;
    }
    else
        return -1;
}
