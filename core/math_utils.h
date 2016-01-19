/**
 * utils.h
 *
 * Author: wushiwei
 * Created on: 2015-12-15
 *
 * Copyright (c) 2015 wushiwei. All Rights Reserved
 *
 */

#ifndef SEMANTIC_MATH_UTILS_H
#define SEMANTIC_MATH_UTILS_H

#include <math.h>
#include "utils.h"

namespace semantic {
namespace math {

const static float_t EPS = 10e-6;

inline float_t sigmoid(float_t val) {
    if (val > 0.0) {
        return 1.0 / (1.0 + exp(-val));
    } else {
        float expv = exp(val);
        return expv / (1.0 + expv);
    }
}

}
}

#endif
