/**
 * utils.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) 2015 wushiwei. All Rights Reserved
 *
 */

#ifndef SEMANTIC_UTILS_H 
#define SEMANTIC_UTILS_H 

//system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <map>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <fstream>

#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include "boost/filesystem.hpp"

#include "jsoncpp/json/json.h"
#include "glog/logging.h"

#ifndef WARN
#define WARN printf
#endif

#ifndef DEBUG
#define DEBUG printf
#endif

#ifndef FATAL
#define FATAL printf 
#endif

typedef int64_t LONG;
typedef uint64_t ULONG;
typedef float float_t ;

namespace semantic {

static const int MAX_LINE_LEN = 1024;
static const int MAX_BUFF_LEN = 1024;

template<typename K, typename V>
class HashMap : public boost::unordered_map<K, V> {};

template<typename K>
class HashSet : public boost::unordered_set<K> {};

static const float INF = 1000.0;

} // namespace semantic

#endif  // SEMANTIC_UTILS_H 

