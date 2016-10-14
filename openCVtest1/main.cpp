//
//  main.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/01.
//  Copyright © 2016年 石川諒. All rights reserved.

#include "cv.h"
#include "highgui.h"
#include "labeling.hpp"

using namespace cv;

void cv_Labelling(IplImage *src_img, IplImage *dst_img);
void GetContourFeature(CvSeq *Contour);
void Binarization(IplImage *src_img, IplImage *dst_img);

int main (int argc, char **argv)
{
    Labeling labeling; //ナンバープレート検出のクラス
    if( labeling.videoCapture == NULL )
    {
        return -1;
    }
    //webカメラ用window生成
    cvNamedWindow("webCamera", CV_WINDOW_AUTOSIZE);

    
    while (1) {
        cvCvtColor(labeling.frame, labeling.gray_img, CV_RGB2GRAY);
        cvShowImage("webCamera", labeling.frame);
        int key = waitKey(1);
        if(key == 113)//qボタンが押されたとき
        {
            break;//whileループから抜ける．
        }else if(key == 115)
        {
            labeling.Binarization(labeling.gray_img, labeling.bin_img);
            labeling.cv_Labelling(labeling.bin_img, labeling.resutl_img);
            imshow("trimming", labeling.Nplate_point);
            //cvShowImage("labelling", labeling.resutl_img);
        }
        labeling.frame = cvQueryFrame(labeling.videoCapture);
    }
    cvReleaseCapture(&labeling.videoCapture);
    cvDestroyAllWindows();
    return 0;
}
