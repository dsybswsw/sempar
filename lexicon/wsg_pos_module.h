/**
 * wsg_pos_module.h
 *
 * Author: wushiwei
 * Created on: 2016-01-18
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#ifndef WSG_POS_MODULE_H
#define WSG_POS_MODULE_H

#include "module_itf.h"
#include "wsg_and_pos.h"

namespace semantic {

class WsgPosModule : public Module {
public:
    friend class WsgPosResource;
    
    WsgPosModule() : _worker(NULL) {}
    ~WsgPosModule();

    int handle(LexForm* sem_form);
protected:
    basenlp::WordSegAndPosTag* _worker;   
};

class WsgPosResource : public DataResource {
public:
    WsgPosResource() : _resource(NULL) {}
    ~WsgPosResource();

    Module* create_module(); 
    int initialize(ConfigUnit& cfg);
protected:
    basenlp::WordSegAndPosTagResource* _resource;
};

}

#endif // WSG_POS_MODULE_H
