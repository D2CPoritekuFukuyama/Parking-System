//
//  trimming.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef trimming_hpp
#define trimming_hpp

#include <stdio.h>
#include "Nplate_trim.hpp"
#include "Elem_trimming.hpp"

class Trimming{
public:
    int get_plateData();
    Trimming();
    Nplate_trim nplate_trim;
    Elem_trimming elem_trimming;
};


#endif /* trimming_hpp */
