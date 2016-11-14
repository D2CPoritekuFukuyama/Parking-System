//
//  labeling.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/11.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef labeling_hpp
#define labeling_hpp

#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#endif /* labeling_hpp */


class Labeling{
private:
    void DrawNextContour(
                         CvSeq *Contour, //輪郭へのポインタ
                         int Level //輪郭のレベル（階層）
    );
    bool check_rectangle(CvSeq *Nplate_seq);
    void contrast_correct(cv::Mat img);
    void draw_poly(CvSeq *approx);
    void Binarization();
public:
    IplImage *frame; //webカメラの画像格納用
    IplImage *gray_img;//グレースケール画像
    IplImage *bin_img;//2値化画像
    IplImage *result_img;//ラベリング結果
    cv::Mat Nplate_point;
    CvCapture *videoCapture;
    cv::Rect Nplate_rect; //ナンバーの位置を格納
    double Area;
    
    Labeling();
    
    void cv_Labelling();

    void trimming(IplImage *src_img);
    

    
    

    
};

