//
//  Elem_trimming.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef Elem_trimming_hpp
#define Elem_trimming_hpp

#include <stdio.h>
#include "labeling.hpp"

using namespace cv;

class Elem_trimming :public Labeling{
private:
    void DrawNextContour(
                         CvSeq *Contour, //輪郭へのポインタ
                         int Level //輪郭のレベル（階層）
    );
    CvSeq *contours;
    void trimming(IplImage *src_img, int count);
    void output_to_csv(int count, Mat src_mat);
    void save_param_img(int count);
    int number_count;
public:
    int get_elem(IplImage *src_img1, IplImage *src_img2);
    Mat param_mat;
};

#endif /* Elem_trimming_hpp */
