/**
 * parser_test.cpp
 *
 * Author: wushiwei
 * Created on: 2015-12-17
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#include "str_opt.h"
#include "learner.h"

using std::string;
using std::vector;
using std::pair;

using semantic::DataSet;
using semantic::Learner;

void test_dataset() {
    DataSet dataset;
    int ret = dataset.read();
    assert(ret == 0);
}

void test_learner() {
    int ret = 0;
    DataSet dataset;
    ret = dataset.read();
    assert(ret == 0);

    semantic::Parser* parser = new semantic::Parser(); 
    ret = parser->initialize("data/sem.grm");
    assert(ret == 0);

    Learner learner;
    learner.init(parser, &dataset);
    learner.learn();
}

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);  
    FLAGS_log_dir = "./log";
    // test_dataset();
    test_learner();
    return 0;
}

