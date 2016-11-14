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

#define VISUAL
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
#ifdef VISUAL
        cvShowImage("webCamera", labeling.frame);
#endif

            labeling.cv_Labelling();
        
#ifdef DEBUG_LABELING
        if (cvWaitKey(1) == 0x71) {
            break;
        }
#endif
#if !defined DEBUG_LABELING
            if (labeling.result_img != NULL){
                //labeling.trimming();
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
