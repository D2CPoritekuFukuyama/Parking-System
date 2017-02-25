//
//  labeling.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/11.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef labeling_hpp
#define labeling_hpp
#pragma once

#include <stdio.h>
#include <opencv/cxcore.hpp>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

#endif /* labeling_hpp */


class Labeling{
protected:
    virtual void DrawNextContour(
                                 CvSeq *Contour, //輪郭へのポインタ
                                 int Level //輪郭のレベル（階層）
    ) =0;
    void Binarization();
    void trimming(IplImage *src_img);
    void cv_Labelling(CvSeq **contours, IplImage *src_img);
    void cv_ColorExtraction(IplImage* src_img, IplImage* dst_img,
                            int code,
                            int ch1_lower, int ch1_upper,
                            int ch2_lower, int ch2_upper,
                            int ch3_lower, int ch3_upper
                            );
private:
    void contrast_correct(cv::Mat img);
public:
    IplImage *frame; //webカメラの画像格納用
    IplImage *gray_img;//グレースケール画像
    IplImage *bin_img;//2値化画像
    IplImage *result_img;//ラベリング結果
    cv::Mat Nplate_point;
    
    
    
};

