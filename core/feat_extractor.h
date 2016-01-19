/**
 * feat_extractor.h
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#ifndef SEMANTIC_FEAT_EXTRACTOR_H
#define SEMANTIC_FEAT_EXTRACTOR_H

#include "example.h"

namespace semantic {

class FeatureExtractor {
public:
    FeatureExtractor() : ngram_(3) {}

    void extract_features(Example& ex, Derivation* deriv);
    void extract_local(Example& ex, Derivation* deriv);
    void extract_recursive(Example& ex, Derivation* deriv);
    void extract_rule(Example& ex, Derivation* deriv);
    void extract_prior_recursively(Example& ex, Derivation* deriv);
private:
    int ngram_;
};

}

#endif // SEMANTIC_FEAT_EXTRACTOR_H

