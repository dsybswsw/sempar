/**
 * wsg_pos_module.cpp
 *
 * Author: wushiwei
 * Created on: 2016-01-18
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#include "wsg_pos_module.h"

namespace semantic {

using std::string;
using std::vector;
using basenlp::CRFWordSegAndPosTag;
using basenlp::CRFWordSegAndPosTagResource;
using basenlp::WordSegAndPosTagResource;
using basenlp::WsgPosRes;

WsgPosModule::~WsgPosModule() {
    if (_worker == NULL) {
        delete _worker;
        _worker = NULL;
    }
}

int WsgPosModule::handle(LexForm* sem_form) {
    if (sem_form == NULL) {
        LOG(WARNING) << "input sem_form is null.";
        return -1;
    }
    if (_worker == NULL) {
        LOG(WARNING) << "wordseg and postag worker has not been well initialzed.";
        return -1;
    }
    vector<WsgPosRes> results;
    vector<Term> terms;
    int ret = sem_form->get_terms(CHARACTER, terms);
    if (ret < 0) {
        LOG(WARNING) << "Failed to get charactor sequence.";
        return -1;
    }
    vector<string> chars;
    for (auto term : terms) {
        chars.push_back(term._text);
    }
    _worker->parse(chars, results);
    for (auto unit : results) {
        Term term(unit.word, unit.pos_tag, unit.start, unit.end);
        sem_form->add_term(WORDSEG, term);
    }
    return 0;
}

WsgPosResource::~WsgPosResource() {
    if (_resource == NULL) {
        delete _resource;
        _resource = NULL;
    }
}

Module* WsgPosResource::create_module() {
    if (_resource == NULL) {return NULL;}
    WsgPosModule* module = new WsgPosModule;
    module->_worker = _resource->create_worker();
    return module;
}

int WsgPosResource::initialize(ConfigUnit& cfg) {
    string crf_path = cfg.get_string("CRF_PATH");
    CRFWordSegAndPosTagResource* crf_rss = new CRFWordSegAndPosTagResource;
    int ret = crf_rss->init(crf_path);
    if (ret < 0) {
        LOG(FATAL) << "Failed to init crf word seg model in " << crf_path;
        return -1;
    }
    _resource = crf_rss;
    return 0;
}

}

