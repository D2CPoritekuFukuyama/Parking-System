//
//  main.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/01.
//  Copyright © 2016年 石川諒. All rights reserved.
#include "stdio.h"
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "Elem_trimming.hpp"
#include "Nplate_trim.hpp"


//#define VISUAL
//#define DEBUG_LABELING
using namespace cv;
using namespace std;

int main (int argc, char **argv)
{
    Nplate_trim nplate_trim = Nplate_trim(); //ナンバープレート検出のクラス
    Elem_trimming elem_trimming = Elem_trimming();
    if(nplate_trim.videoCapture  == NULL )
        return -1;
    
    //webカメラ用window生成
#ifdef VISUAL
    cvNamedWindow("webCamera", CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("elem", CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("elem_gray", CV_WINDOW_AUTOSIZE);
#endif
    while (1) {
        nplate_trim.frame = cvQueryFrame(nplate_trim.videoCapture);
        cvShowImage("webCamera", nplate_trim.frame);		
        if (nplate_trim.get_Nplate() == 0){
			if(nplate_trim.result_img != NULL){
#ifndef DEBUG_LABELING 
    	        if (elem_trimming.get_elem(nplate_trim.result_img) == 0)
                    return 0;
//    	        cvShowImage("elem", elem_trimming.frame);
//        	    cvShowImage("elem_gray", elem_trimming.gray_img);
#endif			
     		}
		}
        if (cvWaitKey(1) == 0x71) {
            break;
        }

    }

    //cvReleaseCapture(&labeling.videoCapture);
    //cvDestroyAllWindows();
    return 0;
}
