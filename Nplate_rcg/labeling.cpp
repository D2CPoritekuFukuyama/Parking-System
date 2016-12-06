//
//  labeling.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/11.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "labeling.hpp"
#include "WarpPerspective.hpp"
#include <math.h>

//#define VISUAL
#define TOLERANCE 90

using namespace cv;


//ラベリング関数
void Labeling::cv_Labelling(CvSeq **contours, IplImage *src_img){
    int contour_count;
    CvMemStorage *storage = cvCreateMemStorage(0);
    //Nplate_trim用
    if (src_img -> nChannels != 1) {
        cvCvtColor(src_img, gray_img, CV_RGB2GRAY);
        Binarization();
        cvShowImage("src_bin", bin_img);
    }
    
    if(bin_img == NULL)
        return;
    //輪郭抽出
    contour_count = cvFindContours(bin_img,
                   storage,
                   contours,
                   sizeof(CvContour),
                   CV_RETR_EXTERNAL,
                   CV_CHAIN_APPROX_SIMPLE);

}


//2値化関数
void Labeling::Binarization(){
    Ptr<IplImage> bin_img1 = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
    Ptr<IplImage> bin_img2 = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
    cvThreshold(gray_img, bin_img1, 165, 255, CV_THRESH_BINARY); //閾値165で2値化
    //適応的閾値処理
    cvAdaptiveThreshold(gray_img, bin_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY);
    cvAnd(bin_img1, bin_img2, bin_img); //二つの２値化画像の論理積
    //cvShowImage("bin", bin_img);
    //cvShowImage("bin1", bin_img1);
    //cvShowImage("bin2", bin_img2);
    //cvShowImage("result", dst_img);
}
