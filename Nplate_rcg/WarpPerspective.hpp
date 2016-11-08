//
//  WarpPerspective.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/05.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef WarpPerspective_hpp
#define WarpPerspective_hpp

#include <stdio.h>
#include "cv.h"
#include "highgui.h"

class WarpPerspective{
public:
    IplImage *src_img;
    CvPoint2D32f src_pnt[4];
    CvPoint2D32f dst_pnt[4];
    WarpPerspective(IplImage *target_img, CvSeq *approx);
    IplImage conversion();
};
#endif /* WarpPerspective_hpp */
