//
//  Elem_trimming.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "Elem_trimming.hpp"
#include "WarpPerspective.hpp"

using namespace cv;



void Elem_trimming::DrawNextContour(
                CvSeq *Contour, //輪郭へのポインタ
                int Level //輪郭のレベル（階層）
){
    for (; Contour != 0; Contour = Contour ->h_next) {
        double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
        if (Area >= 300) {
            CvRect R = cvBoundingRect(Contour,0);
            cvSetImageROI(frame, cvRect(R.x - 5, R.y - 5, R.width + 10, R.height + 10));
            cvSaveImage("image/test/trim.jpg", frame);
            cvResetImageROI(frame);
            //cvRectangle(frame, cvPoint(R.x - 5, R.y - 5), cvPoint(R.x + R.width + 5, R.y + R.height + 5), CV_RGB(255, 0, 0));
            
        }
        
    }
}

void Elem_trimming::trimming(IplImage *src_img){
    
}

void Elem_trimming::get_elem(IplImage *src_img){
    frame = cvCloneImage(src_img);
    gray_img = cvCloneImage(frame);
    cvNot(gray_img, gray_img);
    bin_img = cvCloneImage(gray_img);
    cvThreshold(gray_img, bin_img, 50, 255, CV_THRESH_BINARY);
    
    cv_Labelling(&contours, frame);
    if (contours != NULL){
        DrawNextContour(contours, 1);
        cvShowImage("elem", frame);
        cvShowImage("elem_gray", gray_img);
    }
}
