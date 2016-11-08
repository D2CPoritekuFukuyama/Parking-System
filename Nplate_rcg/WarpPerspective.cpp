//
//  WarpPerspective.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/05.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "WarpPerspective.hpp"

using namespace cv;
using namespace std;

/*typedef int (CV_CDECL* CvCmpFunc)(const void* a, const void* b, void* userdata);

static int cmp_func( const void* _a, const void* _b, void* userdata )
{
    CvPoint* a = (CvPoint*)_a;
    CvPoint* b = (CvPoint*)_b;
    int y_diff = a->y - b->y;
    int x_diff = a->x - b->x;
    //return y_diff ? y_diff : x_diff;
     
    return a->y < b->y ? y_diff : x_diff;
}
 */

WarpPerspective::WarpPerspective(IplImage *target_img, CvSeq *approx){
    src_img = target_img;
    //cvSeqSort(approx, cmp_func,0);
    for (int i = 0; i < approx -> total; ++i) {
        CvPoint *p = (CvPoint*)cvGetSeqElem(approx, i);
        src_pnt[i] = cvPoint2D32f(p -> x, p -> y);        
    }
    
    dst_pnt[0] = cvPoint2D32f (150.0, 150.0);
    dst_pnt[1] = cvPoint2D32f (150.0, 300.0);
    dst_pnt[2] = cvPoint2D32f (350.0, 300.0);
    dst_pnt[3] = cvPoint2D32f (350.0, 150.0);
}

IplImage WarpPerspective::conversion(){
    IplImage *dst_img = NULL;
     dst_img = cvCloneImage (src_img);
     //透視投影変換を実行する
    CvMat *map_matrix = cvCreateMat(3, 3, CV_32FC1);
     cvGetPerspectiveTransform(src_pnt, dst_pnt, map_matrix);
    cvWarpPerspective (src_img,
                       dst_img,
                       map_matrix,
                       CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS,
                       cvScalarAll (100));
    cvShowImage("test", dst_img);
    return *dst_img;
}