//
//  main.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/01.
//  Copyright © 2016年 石川諒. All rights reserved.

#include "cv.h"
#include "highgui.h"

using namespace cv;

void cv_Labelling(IplImage *src_img, IplImage *dst_img);
void GetContourFeature(CvSeq *Contour);

int main (int argc, char **argv)
{
    CvCapture *videoCapture = cvCreateCameraCapture( 0 );
    if( videoCapture == NULL )
    {
        return -1;
    }
    cvNamedWindow("webCamera", CV_WINDOW_AUTOSIZE);
    
    while (1) {
        IplImage *frame = cvQueryFrame(videoCapture);
        IplImage *gray_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
        cvCvtColor(frame, gray_img, CV_RGB2GRAY);
        cvShowImage("webCamera", frame);
        int key = waitKey(1);
        if(key == 113)//qボタンが押されたとき
        {
            break;//whileループから抜ける．
        }else if(key == 115)
        {
            IplImage *bin_img1 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
            IplImage *bin_img2 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
            IplImage *bin_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
            IplImage *resutl_img = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
             cvThreshold(gray_img, bin_img1, 165, 255, CV_THRESH_BINARY);
             cvAdaptiveThreshold(gray_img, bin_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C,       CV_THRESH_BINARY);
             cvAnd(bin_img1, bin_img2, bin_img);
             cvShowImage("bin2", bin_img2);
             cvShowImage("bin", bin_img);
             cv_Labelling(bin_img, resutl_img);
             cvShowImage("labelling", resutl_img);
        }
    }
    cvReleaseCapture(&videoCapture);
    cvDestroyAllWindows();
    return 0;
}

void DrawNextContour(
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
       /* cvDrawContours( img, approx, ContoursColor, ContoursColor, 0, 2);
        if (Contour -> h_next != NULL)
            DrawNextContour(img, approx->h_next, Level);
        */
        
        //面積
        double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
        
        if(approx->total == 4 )//&& Area >= 1000 && Area <= 6000) //頂点が4で面積が100以上なら描画
        {
            cvDrawContours( img, approx, ContoursColor, ContoursColor, 0, 2);
         
            if (Contour -> h_next != NULL)
                DrawNextContour(img, approx->h_next, Level);
        }
    }
}



void cv_Labelling(IplImage *src_img, IplImage *dst_img){
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
    
    cvReleaseMemStorage(&storage);
}

