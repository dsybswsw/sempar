/**
 * params.h
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */
#ifndef SEMANTIC_PARAMS_H
#define SEMANTIC_PARAMS_H

#include "utils.h"

namespace semantic {

class Params {
public:
    Params() : update_num_(0), mu_(0.1) {}
    int read(const char* path);
    int consist(const char* path);
    void update(HashMap<std::string, float_t>& gradient);
    bool contains(const std::string& feature);
    float_t get_weight(const std::string& feature);
    float_t compulte_l2norm();
protected:
    float_t gen_init();

    HashMap<std::string, float_t> weights_;
    HashMap<std::string, float_t> sum_grads_;
    int update_num_;
    float_t mu_;
};

};

#endif // SEMANTIC_PARAMS_H
