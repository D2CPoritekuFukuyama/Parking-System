//
//  main.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/01.
//  Copyright © 2016年 石川諒. All rights reserved.
#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "labeling.hpp"
#include "templateMatch.hpp"

using namespace cv;

void cv_Labelling(IplImage *src_img, IplImage *dst_img);
void GetContourFeature(CvSeq *Contour);
void Binarization(IplImage *src_img, IplImage *dst_img);

int main (int argc, char **argv)
{
    string result;
    Labeling labeling; //ナンバープレート検出のクラス
    TemplateMatch templateMatch = TemplateMatch();
    if( labeling.videoCapture == NULL )
    {
        return -1;
    }
    //webカメラ用window生成
    cvNamedWindow("webCamera", CV_WINDOW_AUTOSIZE);

    
    while (1) {
        cvCvtColor(labeling.frame, labeling.gray_img, CV_RGB2GRAY);
        cvShowImage("webCamera", labeling.frame);

            labeling.Binarization(labeling.gray_img, labeling.bin_img);
            labeling.cv_Labelling(labeling.bin_img, labeling.resutl_img);
            if (labeling.Nplate_rect.width > 0 || labeling.Nplate_rect.height > 0){
                labeling.trimming(labeling.gray_img);
                result = templateMatch.Matching();
                if (result[0] != ' ') {
                    printf("%s\n", result.c_str());
                    cvReleaseCapture(&labeling.videoCapture);
                    cvDestroyAllWindows();
                    break;
                }
            }
            
            labeling.Nplate_rect = Rect(0,0,0,0);
            //locationのゼロクリア

            //cvShowImage("labelling", labeling.resutl_img);
        
        labeling.frame = cvQueryFrame(labeling.videoCapture);
    }
    cvReleaseCapture(&labeling.videoCapture);
    cvDestroyAllWindows();
    return 0;
}
