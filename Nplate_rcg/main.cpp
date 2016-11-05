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
#include "NplateMatch.hpp"

//#define VISUAL
//#define DEBUG_LABELING
using namespace cv;

int main (int argc, char **argv)
{
    string result;
    Labeling labeling = Labeling(); //ナンバープレート検出のクラス
    NplateMatch nplateMatch = NplateMatch();
    if( labeling.videoCapture == NULL )
    {
        return -1;
    }
    //webカメラ用window生成
#ifdef VISUAL
    cvNamedWindow("webCamera", CV_WINDOW_AUTOSIZE);
#endif

    
    while (1) {
        cvCvtColor(labeling.frame, labeling.gray_img, CV_RGB2GRAY);
#ifdef VISUAL
        cvShowImage("webCamera", labeling.frame);
#endif

            labeling.Binarization(labeling.gray_img, labeling.bin_img);
            labeling.cv_Labelling(labeling.bin_img, labeling.resutl_img);
#ifdef DEBUG_LABELING
        if (cvWaitKey(1) == 0x71) {
            break;
        }
#endif
#if !defined DEBUG_LABELING
            if (labeling.Nplate_rect.width > 0 || labeling.Nplate_rect.height > 0){
                labeling.trimming(labeling.gray_img);
                result = nplateMatch.Matching();
                if (result[0] != ' ') {
                    printf("%s\n", result.c_str());
                    cvReleaseCapture(&labeling.videoCapture);
                    cvDestroyAllWindows();
                    break;
                }
            }
            
            labeling.Nplate_rect = Rect(0,0,0,0);
            //locationのゼロクリア
#endif
        
        labeling.frame = cvQueryFrame(labeling.videoCapture);
    }

    cvReleaseCapture(&labeling.videoCapture);
    cvDestroyAllWindows();
    return 0;
}
