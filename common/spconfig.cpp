/**
 * spconfig.cpp
 *
 * Author: wushiwei
 * Created on: 2016-01-18
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#include "spconfig.h"

namespace semantic {

const std::string& ConfigUnit::get_string(const std::string& key) {
    if (_str_units.count(key) == 0) {
        _str_units[key] = "";
    }
    return _str_units[key];
}

int ConfigUnit::get_integer(const std::string& key) {
    if (_int_units.count(key) == 0) {
        _int_units[key] = 0;
    }
    return _int_units[key];
}

void ConfigUnit::set_val(const std::string& key, const std::string& val) {
    _str_units[key] = val;
}

void ConfigUnit::set_val(const std::string& key, int val) {
    _int_units[key] = val;
}

}

