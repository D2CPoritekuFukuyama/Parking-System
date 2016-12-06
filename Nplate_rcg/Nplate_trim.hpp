//
//  Nplate_trim.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/03.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef Nplate_trim_hpp
#define Nplate_trim_hpp

#include <stdio.h>
#include "labeling.hpp"

using namespace cv;

class Nplate_trim : public Labeling{
private:
    void draw_poly(CvSeq *approx);
    bool check_rectangle(CvSeq *Nplate_seq);
    void DrawNextContour(
                                 CvSeq *Contour, //輪郭へのポインタ
                                 int Level //輪郭のレベル（階層）
    );
    void trimming(IplImage *src_img);
    CvSeq *contours;
    void contrast_correct(Mat img);
    
public:
    CvCapture *videoCapture;
    IplImage *nplate_up;
    IplImage *nplate_down;
    Nplate_trim();
    int get_Nplate();
    
};

#endif /* Nplate_trim_hpp */
