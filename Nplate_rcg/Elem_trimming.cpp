//
//  Elem_trimming.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//
#include <iostream>
#include "Elem_trimming.hpp"
#include "WarpPerspective.hpp"
#include <fstream>

using namespace cv;
using namespace std;


void Elem_trimming::DrawNextContour(
                CvSeq *Contour, //輪郭へのポインタ
                int Level //輪郭のレベル（階層）
){
    int count = 0;
    for (; Contour != 0; Contour = Contour ->h_next) {
        double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
        CvRect R = cvBoundingRect(Contour,0);
        cvSetImageROI(frame, cvRect(R.x, R.y, R.width, R.height));
        if( Area >= 100 && Area <= 900){
            if (count == 0) {
                count ++;
            }else{
                trimming(frame);
                output_to_csv(count, param_mat);
                count ++;
            }
        }else if(Area >= 1000){
            trimming(frame);
            output_to_csv(count, param_mat);
            count ++;
        }
        cvResetImageROI(frame);
    }
}

void Elem_trimming::trimming(IplImage *src_img){
    Mat src_mat = src_img;
    param_mat = Mat::ones(28, 28, CV_8U)*255;
    double ratio = (double)src_mat.cols / src_mat.rows;
    resize(src_mat, src_mat, Size(), ratio * (15.0 / src_mat.cols), (15.0 / src_mat.rows) );
    //28x28の白背景の画像に,15x15の画像を描画 (padding = 6)
    for (int row = 0; row < src_mat.rows; row++) {
        for (int col = 0 ; col < src_mat.cols; col ++) {
            param_mat.at<unsigned char>(row+6,col+8) = src_mat.at<unsigned char>(row, col);
        }
    }
    threshold(param_mat, param_mat, 90, 255, THRESH_BINARY);
    cout << param_mat << endl;
    imwrite("image/test/trim.jpg", param_mat);
    param_mat = param_mat.reshape(0, 784); //1行784列に変換
}

void Elem_trimming::output_to_csv(int count, Mat src_mat){
    stringstream ss;
    switch (count) {
        case 1:
            ss << "Dataset/" << "Hiragana" << ".csv";
            break;
        case 2:
            ss << "Dataset/" << "Number4" << ".csv";
            break;
        case 3:
            ss << "Dataset/" << "Number3" << ".csv";
            break;
        case 4:
            ss << "Dataset/" << "Number2" << ".csv";
            break;
        case 5:
            ss << "Dataset/" << "Number1" << ".csv";
            break;
        case 6:
            ss << "Dataset/" << "Category_number3" << ".csv";
            break;
        case 7:
            ss << "Dataset/" << "Category_number2" << ".csv";
            break;
        case 8:
            ss << "Dataset/" << "Category_number1" << ".csv";
            break;
        default:
            return;
    }
    ofstream writing_file;//出力用
    writing_file.open(ss.str().c_str(), std::ios::out);
    writing_file << cv::format(src_mat,"csv") << ", ";
    writing_file << "1"<< std::endl;
}

int Elem_trimming::get_elem(IplImage *src_img){
    frame = cvCloneImage(src_img);
    gray_img = cvCloneImage(frame);
    cvNot(gray_img, gray_img);
    bin_img = cvCloneImage(gray_img);
    cvThreshold(gray_img, bin_img, 165, 255, CV_THRESH_BINARY);
    cv_Labelling(&contours, frame);
    if (contours != NULL){
        DrawNextContour(contours, 1);
        return 0;
    }
    return -1;
    
}
