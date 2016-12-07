//
//  Elem_trimming.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "Elem_trimming.hpp"
#include "WarpPerspective.hpp"
#include <fstream>

using namespace cv;
using namespace std;


void Elem_trimming::DrawNextContour(
                CvSeq *Contour, //輪郭へのポインタ
                int Level //輪郭のレベル（階層）
){
    for (; Contour != 0; Contour = Contour ->h_next) {
        double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
        CvRect R = cvBoundingRect(Contour,0);
        cvSetImageROI(frame, cvRect(R.x, R.y, R.width, R.height));
        if ( Area >= 600 && Area <= 900) {
            trimming(frame);
            cvResetImageROI(frame);
        }else if(Area >= 1000){
            trimming(frame);
            cvResetImageROI(frame);
        }
    }
}

void Elem_trimming::trimming(IplImage *src_img){
    Mat src_mat = src_img;
    Mat canvas = Mat::ones(28, 28, CV_8U)*255;
    resize(src_mat, src_mat, Size(), (15.0 / src_mat.cols), (15.0 / src_mat.rows) );
    //28x28の白背景の画像に,15x15の画像を描画 (padding = 6)
    for (int row = 0; row < 15; row++) {
        for (int col = 0 ; col < 15; col ++) {
            canvas.at<unsigned char>(row+6,col+6) = src_mat.at<unsigned char>(row, col);
        }
    }
    cout << canvas << endl;
    imwrite("image/test/trim.jpg", canvas);
    src_mat = src_mat.reshape(0, 1); //1行784列に変換
    cout<< format(src_img,"csv") << endl;
    std::string filename = "Dataset/test.csv";//出力用
    std::ofstream writing_file;//出力用
    writing_file.open(filename, std::ios::out);
    writing_file << cv::format(src_mat,"csv") << ", ";
    writing_file << "17"<< std::endl;
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
    }
}
