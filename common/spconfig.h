/**
 * spconfig.cpp
 *
 * Author: wushiwei
 * Created on: 2016-01-18
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#ifndef COMMON_SPCONFIG_H
#define COMMON_SPCONFIG_H

#include "utils.h"

namespace semantic {
class ConfigUnit {
public:
    ConfigUnit() {}
    ~ConfigUnit() {}

    const std::string& get_string(const std::string& key);
    int get_integer(const std::string& key);
    void set_val(const std::string& key, const std::string& val);
    void set_val(const std::string& key, int val);
public:
    HashMap<std::string, std::string> _str_units;
    HashMap<std::string, int> _int_units;
};

}

#endif // COMMON_SPCONFIG_H
