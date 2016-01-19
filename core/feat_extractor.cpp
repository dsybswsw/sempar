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
    extract_rule(ex, deriv);
    extract_prior_recursively(ex, deriv);
}

void FeatureExtractor::extract_rule(Example& ex, Derivation* deriv) {
    if (deriv == NULL) {return;}
    // extract rull feature.
    string rule_str = deriv->get_rule_string();
    deriv->local_feature_.add_domain("rule", rule_str);
}

void FeatureExtractor::extract_prior_recursively(Example& ex, Derivation* deriv) {
    if (deriv == NULL) {return;}
    string cat = deriv->_cat;
    deriv->local_feature_.add_domain("cat1", cat);
    for (auto child : deriv->_children) {
        for (auto feat_pair : child->local_feature_.feat_vec_)  {
            string feat = feat_pair.first;
            // feature start with cat.
            for (int i = 1; i < ngram_; ++i) {
                string prefix = "cat" + boost::lexical_cast<string>(i) + "::";
                int idx = feat.find(prefix);
                if (feat.find(prefix) == 0) { 
                    string name = feat.substr(idx + prefix.size());
                    string npref = "cat" + boost::lexical_cast<string>(i + 1); 
                    name = cat + "+" + name;
                    LOG(INFO) << "new feature " << npref << "::" << name;
                    deriv->local_feature_.add_domain(npref, name);
                }
            }
        }
    }
}

}
