/**
 * feat_vec.h
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#ifndef SEMANTIC_FEAT_VEC_H
#define SEMANTIC_FEAT_VEC_H

#include "utils.h"
#include "params.h"

namespace semantic {

class FeatureVector {
public:
    FeatureVector() {}
    
    std::string to_feature(const std::string& domain, const std::string& name);
    void add_domain(const std::string& domain, const std::string& name);
    void add_with_bias(const std::string& domain, const std::string& name);
    void add_feat(const std::string& feat);
    void add_feat(const std::string& feat, float_t val);
    float_t dot_product(Params* params);
    std::string to_set_string();
public:
    HashMap<std::string, float_t> feat_vec_;
};

}

#endif // SEMANTIC_FEAT_VEC_H
