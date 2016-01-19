/**
 * crf_tagger.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) 2015 wushiwei. All Rights Reserved
 *
 */

#ifndef BASENLP_CRF_TAGGER_H
#define BASENLP_CRF_TAGGER_H

#include "utils.h"
#include "crfpp.h"

namespace basenlp {

class CRFTagger;
class CRFModel {
public:
    friend class CRFTagger;

    CRFModel() : _model(NULL) {}
    ~CRFModel() {
        if (_model) {
            delete _model;
            _model = NULL;
        }
    }

    int init(const std::string& model_path);
    CRFTagger* create_crf_tagger();
protected:
    CRFPP::Model* _model;
};

class CRFTagger {
public:
    friend class CRFModel;
    CRFTagger() : _tagger(NULL) {}
    ~CRFTagger() {
        if (_tagger) {
            _tagger->close();
        }
    }
    int init(CRFModel& crf_model);
    int decode(const std::vector<std::string>& sequence, std::vector<std::string>& tags);
    int decode(const std::vector<std::string> sequence, 
               std::vector<std::string>& tags, 
               double& score);
    int decode(const std::vector<std::string>& sequence, 
               int n_best, 
               std::vector<std::string>& tags);
protected:
    CRFPP::Tagger* _tagger;
};
}

#endif // BASENLP_CRF_TAGGER_H
