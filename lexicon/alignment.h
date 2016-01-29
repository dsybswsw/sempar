/**
 * alignment.h
 *
 * Author: wushiwei
 * Created on: 2016-01-18
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#ifndef LEX_ALIGN_ALIGNMENT_H
#define LEX_ALIGN_ALIGNMENT_H

#include "wsg_pos_module.h"

namespace semantic {

class LexAlign {
public:
    friend class LexAlignResource;

    LexAlign() {}
    ~LexAlign();

    void align(LexForm* lex_form);
protected:
    std::vector<Module*> _modules;
};

class LexAlignResource {
public:
    LexAlignResource() {}
    ~LexAlignResource();
    
    int init(ConfigUnit& cfg);

    LexAlign* create_lex_align();
protected:
    std::vector<DataResource*> _resources;
};

}

#endif // LEX_ALIGN_ALIGNMENT_H
