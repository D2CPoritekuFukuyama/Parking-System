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
                         IplImage *img,//ラベリング結果を描画するIplImage(8Bit3chカラー）
                         CvSeq *Contour, //輪郭へのポインタ
                         int Level //輪郭のレベル（階層）
    );
public:
    void cv_Labelling(
                      IplImage *src_img,
                      IplImage *dst_img
                      );
    void Binarization(
                      IplImage *src_img,
                      IplImage *dst_img
                      );
    
};

