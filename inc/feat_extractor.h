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
    void extract_features(Example& ex, Derivation* deriv);
    void extract_local(Example& ex, Derivation* deriv);
    void extract_recursive(Example& ex, Derivation* deriv);
};

}

#endif // SEMANTIC_FEAT_EXTRACTOR_H

