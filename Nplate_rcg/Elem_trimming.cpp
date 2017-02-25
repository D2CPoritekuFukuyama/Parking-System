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
#include <sys/stat.h>

using namespace cv;
using namespace std;


void Elem_trimming::DrawNextContour(
                CvSeq *Contour, //輪郭へのポインタ
                int Level //輪郭のレベル（階層）
){
    number_count = 0;
    int count = 0;
    for (; Contour != 0; Contour = Contour ->h_next) {
        double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
        CvRect R = cvBoundingRect(Contour,0);
        cvSetImageROI(frame, cvRect(R.x, R.y, R.width, R.height));
        if( Area >= 100 && Area <= 900){
            if (count == 0) {
                count ++;
            }else{
                //trimming(frame);
                //save_param_img(count);
                //param_mat = param_mat.reshape(0, 784); //1行784列に変換
                //output_to_csv2(count, param_mat);
                count ++;
            }
        }else if(Area >= 1000){
            //trimming(frame);
            //save_param_img(count);
            //param_mat = param_mat.reshape(0, 784); //1行784列に変換
            //output_to_csv2(count, param_mat);
            number_count ++;
            count ++;
        }
        cvResetImageROI(frame);
    }
}

void Elem_trimming::trimming(IplImage *src_img, int count){
    Mat src_mat = src_img;
    param_mat = Mat::ones(28, 28, CV_8U)*255;
    if(count == 0){
        //ひらがなの時
        resize(src_mat, src_mat, Size(), (20.0 / src_mat.cols), (20.0 / src_mat.rows) );
        //28x28の白背景の画像に,15x15の画像を描画 
        for (int row = 0; row < src_mat.rows; row++) {
            for (int col = 0 ; col < src_mat.cols; col ++) {
                param_mat.at<unsigned char>(row+5,col+5) = src_mat.at<unsigned char>(row, col);
            }
        }
    }else{
        //数字の時
        resize(src_mat, src_mat, Size(), (25.0 / src_mat.cols), (25.0 / src_mat.rows) );
        //28x28の白背景の画像に,15x15の画像を描画 
        for (int row = 0; row < src_mat.rows; row++) {
            for (int col = 0 ; col < src_mat.cols; col ++) {
                param_mat.at<unsigned char>(row,col) = src_mat.at<unsigned char>(row, col);
            }
        }
    }
}

void Elem_trimming::save_param_img(int count){
    stringstream ss;
    ss << "image/test/" << count << ".jpg";
// if(count == 0)
//     threshold(param_mat, param_mat, 140, 255, THRESH_BINARY);
//    else
//       threshold(param_mat, param_mat, 0, 255, THRESH_BINARY);
    imwrite(ss.str().c_str(), param_mat);
}

void Elem_trimming::output_to_csv(int count, Mat src_mat){
    stringstream ss;
    switch (count) {
        case 0:
            ss << "Dataset/" << "Hiragana" << ".csv";
            break;
        case 1:
            ss << "Dataset/" << "Number1" << ".csv";
            break;
        case 2:
            ss << "Dataset/" << "Number2" << ".csv";
            break;
        case 3:
            ss << "Dataset/" << "Number3" << ".csv";
            break;
        case 4:
            ss << "Dataset/" << "Number4" << ".csv";
            break;
        case 5:
            ss << "Dataset/" << "Category_number3" << ".csv";
            break;
        case 6:
            ss << "Dataset/" << "Category_number2" << ".csv";
            break;
        case 7:
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

int Elem_trimming::get_elem(IplImage *src_img1, IplImage *src_img2){
    int width = 0, count = 0;
    frame = cvCloneImage(src_img1);
	gray_img = cvCloneImage(frame);
    //cvNot(gray_img, bin_img);
    cvShowImage("elem_frame", gray_img);
    cvThreshold(gray_img, gray_img, 100, 255, CV_THRESH_BINARY_INV);
	bin_img = cvCloneImage(gray_img);
    cvShowImage("elem inv", gray_img);
    cv_Labelling(&contours, gray_img);
    if (contours != NULL){
        DrawNextContour(contours, 1);
        if(number_count == 4){
            frame = cvCloneImage(src_img2);
            //ひらがなのトリミング
            cvSetImageROI(frame, Rect(width ,80, 50,50));
			Mat test = frame;
            trimming(frame, count);
            save_param_img(count);
            param_mat = param_mat.reshape(0, 784); //1行784列に変換
            cvResetImageROI(frame);
            output_to_csv(count, param_mat);
            //ナンバーのトリミング
            width = 50;
            for (count = 1; count <= 4; count++) {
                if (count == 3)
                    width += 25;
                cvSetImageROI(frame, Rect(width,55, 60,155));
                trimming(frame, count);
                save_param_img(count);
                param_mat = param_mat.reshape(0, 784); //1行784列に変換
                cvResetImageROI(frame);
                output_to_csv(count, param_mat);
                width += 55;
            }
        }
        return 0;
    }
    return -1;
    
}
