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
    cvFindContours(bin_img,
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
    //黄色を2値化で白へ
    cv_ColorExtraction(frame, bin_img1, CV_BGR2HSV, 15, 35, 100, 255, 100, 255);
    cv_ColorExtraction(frame, bin_img2, CV_BGR2HSV, 0, 180, 0, 50, 70, 255);
	cvShowImage("bin_4 test", bin_img2);
    //適応的閾値処理
//    cvAdaptiveThreshold(gray_img, bin_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY);
//    cvAnd(bin_img1, bin_img2, bin_img); //二つの２値化画像の論理積
    cvAdd(bin_img1, bin_img2, bin_img);
    //cvShowImage("bin", bin_img);
    //cvShowImage("bin1", bin_img1);
    //cvShowImage("bin2", bin_img2);
    //cvShowImage("result", dst_img);
}

//軽自動車用
void Labeling::cv_ColorExtraction(IplImage* src_img, IplImage* dst_img,
                        int code,
                        int ch1_lower, int ch1_upper,
                        int ch2_lower, int ch2_upper,
                        int ch3_lower, int ch3_upper
                        ){
    
    int i, k;
    
    IplImage *Color_img;
    IplImage *ch1_img, *ch2_img, *ch3_img;
    IplImage *Mask_img;
    
    int lower[3];
    int upper[3];
    int val[3];
    
    CvMat *lut;
    
    //codeに基づいたカラー変換
    Color_img = cvCreateImage(cvGetSize(src_img), src_img->depth, src_img->nChannels);
    cvCvtColor(src_img, Color_img, code);
    
    //3ChのLUT作成
    lut    = cvCreateMat(256, 1, CV_8UC3);
    
    lower[0] = ch1_lower;
    lower[1] = ch2_lower;
    lower[2] = ch3_lower;
    
    upper[0] = ch1_upper;
    upper[1] = ch2_upper;
    upper[2] = ch3_upper;
    
    for (i = 0; i < 256; i++){
        for (k = 0; k < 3; k++){
            if (lower[k] <= upper[k]){
                if ((lower[k] <= i) && (i <= upper[k])){
                    val[k] = 255;
                }else{
                    val[k] = 0;
                }
            }else{
                if ((i <= upper[k]) || (lower[k] <= i)){
                    val[k] = 255;
                }else{
                    val[k] = 0;
                }
            }
        }
        //LUTの設定
        cvSet1D(lut, i, cvScalar(val[0], val[1], val[2]));
    }
    
    //3ChごとのLUT変換（各チャンネルごとに２値化処理）
    cvLUT(Color_img, Color_img, lut);
    cvReleaseMat(&lut);
    
    //各チャンネルごとのIplImageを確保する
    ch1_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
    ch2_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
    ch3_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
    
    //チャンネルごとに二値化された画像をそれぞれのチャンネルに分解する
    cvSplit(Color_img, ch1_img, ch2_img, ch3_img, NULL);
    
    //3Ch全てのANDを取り、マスク画像を作成する。
    Mask_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
    cvAnd(ch1_img, ch2_img, Mask_img);
    cvAnd(Mask_img, ch3_img, Mask_img);
    
    //入力画像(src_img)のマスク領域を出力画像(dst_img)へコピーする
    cvZero(dst_img);
    cvCopy(Mask_img, dst_img);
    //解放
    cvReleaseImage(&Color_img);
    cvReleaseImage(&ch1_img);
    cvReleaseImage(&ch2_img);
    cvReleaseImage(&ch3_img);
    cvReleaseImage(&Mask_img);
    
}
