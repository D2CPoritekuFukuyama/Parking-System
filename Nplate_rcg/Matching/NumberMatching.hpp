//
//  NumberMatching.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/02.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef NumberMatching_hpp
#define NumberMatching_hpp

#include <stdio.h>
#include "Matching.hpp"



class NumberMatching:public Matching{
private:
public:
    NumberMatching(IplImage *target_img);
    string Number_Matching();
    
};

#endif /* NumberMatching_hpp */

