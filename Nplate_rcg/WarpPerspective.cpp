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

typedef int (CV_CDECL* CvCmpFunc)(const void* a, const void* b, void* userdata);


WarpPerspective::WarpPerspective(IplImage *target_img, CvSeq *approx){
    src_img = target_img;
    dst_img = cvCloneImage(src_img);
    
    for (int i = 0; i < approx -> total; ++i) {
        CvPoint *p = (CvPoint*)cvGetSeqElem(approx, i);
        src_pnt[i] = cvPoint2D32f(p -> x, p -> y);
    }
    get_ConvexHull(approx);
     
    //変換後のCvPoint2d32f
    dst_pnt[0] = cvPoint2D32f (dst_img->width, 0.0);
    dst_pnt[1] = cvPoint2D32f (dst_img->width, dst_img->height);
    dst_pnt[2] = cvPoint2D32f (0.0, dst_img->height);
    dst_pnt[3] = cvPoint2D32f (0.0, 0.0);

    /*
    dst_pnt[3] = cvPoint2D32f (150.0, 150.0);
    dst_pnt[2] = cvPoint2D32f (150.0, 300.0);
    dst_pnt[1] = cvPoint2D32f (350.0, 300.0);
    dst_pnt[0] = cvPoint2D32f (350.0, 150.0);
*/ 
}

//凸包を求めて、座標シーケンスのスタート位置を統一化
void WarpPerspective::get_ConvexHull(CvSeq *approx){
    CvSeq *ptseq;
    int hullcount,i;
    CvMemStorage* storage = cvCreateMemStorage();
    
    ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),
                               sizeof(CvPoint), storage );
    
    for( i = 0; i < approx -> total; i++ )
    {
        CvPoint p = *(CvPoint*)cvGetSeqElem(approx, i);
        cvSeqPush( ptseq, &p);
    }
    hull = cvConvexHull2( ptseq, 0, CV_COUNTER_CLOCKWISE, 0 );
    hullcount = hull->total;
    
    for (i = 0; i < hullcount; i++) {
        CvPoint p = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
        src_pnt[i] = cvPointTo32f(p);
    }
    if (src_pnt[1].x < src_pnt[3].x) {
        CvPoint2D32f tmp_p = cvPoint2D32f(src_pnt[hullcount - 1].x, src_pnt[hullcount -1].y);
        for (i = hullcount - 1; i > 0; i--) {
            src_pnt[i] = src_pnt[i - 1];
        }
        src_pnt[0] = tmp_p;
    }
    cvClearMemStorage( storage );
}


IplImage WarpPerspective::conversion(){
     //透視投影変換を実行する
    Ptr<IplImage> result_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
    CvMat *map_matrix = cvCreateMat(3, 3, CV_32FC1);
    cvGetPerspectiveTransform(src_pnt, dst_pnt, map_matrix);
    cvWarpPerspective (src_img,
                       dst_img,
                       map_matrix,
                       CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS,
                       cvScalarAll (100));
    cvCvtColor(dst_img, result_img, CV_RGB2GRAY);
    
    return *result_img;
}
