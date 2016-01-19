/**
 * wsg_and_pos.cpp
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) 2015 wushiwei. All Rights Reserved
 *
 */

#include "wsg_and_pos.h"

namespace basenlp {

using std::vector;
using std::string;

const string CRFWordSegAndPosTag::BEGIN_SUFFIX = "_S";
const string CRFWordSegAndPosTag::IN_SUFFIX = "_I";
const string CRFWordSegAndPosTag::END_SUFFIX = "_E";
const string CRFWordSegAndPosTag::OUT_SUFFIX = "_O";

CRFWordSegAndPosTag::~CRFWordSegAndPosTag() {
    if (_tagger != NULL) {
        delete _tagger;
        _tagger = NULL;
    }
}

void CRFWordSegAndPosTag::parse(const vector<string>& sequence, vector<WsgPosRes>& results) {
    if (!_tagger) {
        results.clear();
        return;
    }
    vector<string> tags;
    int ret = _tagger->decode(sequence, tags);
    if (ret < 0) {
        LOG(WARNING) << "Failed to tag sequence.";
        return;
    }
    if (sequence.size() != tags.size()) {
        LOG(WARNING) << "input size is not equal to output size.";
        return;
    }
    collect_results(sequence, tags, results);
}

void CRFWordSegAndPosTag::collect_results(const vector<string>& sequence, 
                                          const vector<string>& tags, 
                                          vector<WsgPosRes>& results) {
    if (sequence.size() != tags.size()) {return;}
    int seq_size = sequence.size();
    string pos_tag;
    string word;
    int start = 0;
    int end = 0;
    for (int i = 0; i < seq_size; ++i) {
        if (tags[i].find(BEGIN_SUFFIX) > 0) {
            word.append(sequence[i]);
        } else if (tags[i].find(IN_SUFFIX) > 0) {
            word.append(sequence[i]);
        } else if (tags[i].find(END_SUFFIX) > 0) {
            pos_tag = tags[i].substr(0, tags[i].size() - 2);
            end = i + 1;
            word.append(sequence[i]);
            // make parse result token.
            WsgPosRes res;
            res.pos_tag = pos_tag;
            res.word = word;
            res.start = start;
            res.end = end;
            // reset flags.
            word = "";
            pos_tag = "";
            start = end;
        }
    }
}

CRFWordSegAndPosTagResource::~CRFWordSegAndPosTagResource() {
   if (_model != NULL) {
       delete _model;
       _model = NULL;
   }
}

WordSegAndPosTag* CRFWordSegAndPosTagResource::create_worker() {
    if (_model == NULL) {
        return NULL;
    }
    CRFWordSegAndPosTag* worker = new CRFWordSegAndPosTag;
    worker->_tagger = _model->create_crf_tagger();
    return worker;
}

int CRFWordSegAndPosTagResource::init(const std::string& model_path) {
    _model = new CRFModel; 
    return _model->init(model_path);
}

CRFTagger* CRFWordSegAndPosTagResource::create_crf_tagger() {
    if (_model == NULL) {return NULL;}
    return _model->create_crf_tagger();
}

}

