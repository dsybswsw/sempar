/**
 * alignment.cpp
 *
 * Author: wushiwei
 * Created on: 2016-01-18
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#include "alignment.h"

namespace semantic {

LexAlign::~LexAlign() {
    for (auto module : _modules) {
        if (module != NULL) {
            delete module;
            module = NULL;
        }
    }
    _modules.clear(); 
}

void LexAlign::align(LexForm* lex_form) {
    for (auto module : _modules) {
        if (module == NULL) {return;}
        module->handle(lex_form);
    }
}

LexAlignResource::~LexAlignResource() {
    for (auto resource : _resources) {
        if (resource != NULL) {
            delete resource;
            resource = NULL;
        }
    }
    _resources.clear();
}

int LexAlignResource::init(ConfigUnit& cfg) {
    DataResource* resource = new WsgPosResource(); 
    int ret = resource->initialize(cfg);
    if (ret < 0) {
        delete resource;
        resource = NULL;
        LOG(FATAL) << "Failde to intialize wordseg and pos tag model";
        return -1;
    }
    _resources.push_back(resource);
    return 0;
}

LexAlign* LexAlignResource::create_lex_align() {
    LexAlign* lex_align = new LexAlign;
    for (auto resource : _resources) {
        Module* module = resource->create_module();
        if (module == NULL) {continue;}
        lex_align->_modules.push_back(module);
    }
    return lex_align;
}

}

