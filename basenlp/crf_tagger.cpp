/**
 * crf_tagger.cpp
 *
 * Author: wushiwei
 * Created on: 2016-01-13
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#include "crf_tagger.h"

namespace basenlp {

using std::vector;
using std::string;

int CRFModel::init(const string& model_path) {
    string params = "-m " + model_path + " -v3";
    _model = CRFPP::createModel(params.c_str());
    if (_model == NULL) {
        LOG(FATAL) << "failed to initialize crf model: " << model_path;
        return -1;
    }
    return 0;
}

CRFTagger* CRFModel::create_crf_tagger() {
    if (_model == NULL) {return NULL;}
    CRFTagger* tagger = new CRFTagger;
    tagger->_tagger = _model->createTagger();
    return tagger;
}

int CRFTagger::init(CRFModel& crf_model) {
    if (crf_model._model == NULL) {
        return -1;
    }
    _tagger = crf_model._model->createTagger();
    if (_tagger == NULL) {
        LOG(FATAL) << "failed to initialize crf tagger";
        return -1;
    }
    return 0;
}

int CRFTagger::decode(const vector<string>& sequence, vector<string>& tags) {
    tags.clear();
    if (!_tagger) {
        LOG(WARNING) << "tagger has not been initialize";
        return -1;
    }
    _tagger->clear();
    for (vector<string>::const_iterator iter = sequence.begin(); iter != sequence.end(); iter++) {
        _tagger->add(iter->c_str());
    }
    if (!_tagger->parse()) {
        LOG(WARNING) << "failed to decode";
        return -1;
    }
    for (int i = 0; i < (int)_tagger->size(); i++) {
        tags.push_back(_tagger->y2(i));
    }
    return 0;
}

int CRFTagger::decode(const vector<string> sequence, vector<string>& tags, double& score) {
    score = 0.0;
    tags.clear();
    if (!_tagger) {
        LOG(WARNING) << "tagger has not been initialize";
        return -1;
    }
    _tagger->clear();
    for (vector<string>::const_iterator iter = sequence.begin(); iter != sequence.end(); iter++) {
        _tagger->add((*iter).c_str());
    }
    if (!_tagger->parse()) {
        LOG(WARNING) << "failed to decode";
        return -1;
    }
    score = _tagger->prob();
    for (int i = 0; i < (int)_tagger->size(); i++) {
        tags.push_back(_tagger->y2(i));
    }
    return 0;
}

int CRFTagger::decode(const vector<string>& sequence, int n_best, vector<string>& tags) {
    if (n_best < 1) {
        LOG(WARNING) << "n_best < 1, using default 1.";
        n_best = 1;
    }

    tags.clear();
    if (!_tagger) {
        LOG(WARNING) << "tagger has not been initialize";
        return -1;
    }
    _tagger->clear();
    for (vector<string>::const_iterator iter = sequence.begin(); 
            iter != sequence.end(); iter++) {
        _tagger->add((*iter).c_str());
    }
    _tagger->set_nbest(n_best);

    if (!_tagger->parse()) {
        LOG(WARNING) << "failed to decode";
        return -1;
    }

    tags.clear();
    for (int j = 0; j < n_best; ++j) {
        if (!_tagger->next()) {
            LOG(WARNING) << "failed to get next nbest";
            return -1;
        }
        LOG(INFO) << j << " best: score=" << _tagger->prob();
        for (int i = 0; i < (int)_tagger->size(); ++i) {
            string tag = _tagger->y2(i);
            if (j == 0) {
                tags.push_back(tag);
            }
            LOG(INFO) << "tags " << i << " " << tag;
        }
        LOG(INFO) << std::endl;
    }
    return 0;
}

}

