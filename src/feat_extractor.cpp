/**
 * feat_extractor.h
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#include "feat_extractor.h"

namespace semantic {
using std::string;

void FeatureExtractor::extract_features(Example& ex, Derivation* deriv) {
    if (deriv == NULL) {return;}
    if (deriv->_cat != ROOT) {
        LOG(INFO) << "dirvation is not root";
    }
    extract_recursive(ex, deriv); 
}

// extract feature vector in root derivation.
void FeatureExtractor::extract_recursive(Example& ex, Derivation* deriv) {
    extract_local(ex, deriv);
    for (int i = 0; i < deriv->_children.size(); ++i) {
        extract_recursive(ex, deriv->_children[i]);
    }
}

void FeatureExtractor::extract_local(Example& ex, Derivation* deriv) {
    if (deriv == NULL) {return;}
    // extract rull feature.
    string rule_str = deriv->get_rule_string();
    deriv->local_feature_.add_domain("rule", rule_str);
}

}
