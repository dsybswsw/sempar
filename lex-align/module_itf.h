/**
 * module_itf.h
 *
 * Author: wushiwei
 * Created on: 2016-01-18
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#ifndef MODULE_ITF_H
#define MODULE_ITF_H 

#include "spconfig.h"
#include "lex_data_struct.h"

namespace semantic {
class Module {
public:
    Module() {}
    virtual ~Module() {}
    virtual int handle(LexForm* sem_form) = 0;
};

class Resource {
public:
    Resource() {}
    virtual ~Resource() {} 
    virtual Module* create_module() = 0;
};

class DataResource : public Resource {
public:
    DataResource() {}
    virtual ~DataResource() {}
    
    virtual Module* create_module() = 0;
    virtual int initialize(ConfigUnit& cfg) = 0;
};

}

#endif // MODULE_ITF_H 

