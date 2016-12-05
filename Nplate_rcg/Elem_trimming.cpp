//
//  Elem_trimming.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "Elem_trimming.hpp"

using namespace cv;



void Elem_trimming::DrawNextContour(
                CvSeq *Contour, //輪郭へのポインタ
                int Level //輪郭のレベル（階層）
){

}

void Elem_trimming::trimming(IplImage *src_img){
    
}

void Elem_trimming::get_elem(IplImage *src_img){
    frame = cvCloneImage(src_img);
    gray_img = cvCloneImage(frame);
    bin_img = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
    cv_Labelling(&contours, frame);
    if (contours != NULL){
        for (; contours != 0; contours = contours ->h_next) {
            CvRect R = cvBoundingRect(contours,0);
            cvRectangle(frame, cvPoint(R.x, R.y), cvPoint(R.x + R.width, R.y + R.height), CV_RGB(255, 0, 0));
        }
        
        cvSaveImage("image/test/elem_trim.jpg", frame);
        //DrawNextContour(contours, 1);
    }
}
