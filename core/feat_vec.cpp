/**
 * feat_vec.cpp
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#include "feat_vec.h"

namespace semantic {
using std::string;
using std::vector;
string FeatureVector::to_feature(const string& domain, const string& name) {
    return domain + "::" + name;
}

void FeatureVector::add_domain(const string& domain, const string& name) {
    add_feat(to_feature(domain, name));
}

void FeatureVector::add_with_bias(const string& domain, const string& name) {
    add_feat(to_feature(domain, name));
    add_feat(to_feature(domain, name + "-bias"), 1.0);
}

void FeatureVector::add_feat(const string& feat) {
    add_feat(feat, 1.0);
}

void FeatureVector::add_feat(const string& feat, float_t val) {
    if (feat_vec_.count(feat) == 0) {
        feat_vec_[feat] = val;
    }
}

float_t FeatureVector::dot_product(Params* model) {
    if (model == NULL) {return -10000.0;}
    float sum = 0.0;
    for (auto entry : feat_vec_) {
        if (model->contains(entry.first)) {
            sum += entry.second * model->get_weight(entry.first);
        }
    }
    return sum;
}

string FeatureVector::to_set_string() {
    string res("{");
    for (auto entry : feat_vec_) {
        res.append(entry.first);
        res.append(" ");
    }
    boost::algorithm::trim(res);
    res.append("}");
    return res;
}

}

