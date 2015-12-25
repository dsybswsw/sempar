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
#include "parser.h"

using std::string;
using std::vector;
using std::pair;
using semantic::Params;

int main(int argc, char* argv[]) {
    //google::InitGoogleLogging(argv[0]);  
    //FLAGS_log_dir = "./log";
    semantic::Parser* parser = new semantic::Parser(); 
    int ret = parser->initialize("data/sem.grm");
    if (ret < 0) {
        LOG(INFO) << "Failed to initalize.";
        return -1;
    }
    LOG(INFO) << "Finish initialization.";
    
    Params* model = new Params();
    ret = model->read("data/model/param_toy.mdl.txt");
    assert(ret == 0);

    string query = "-1*3+2*4";
    vector<string> tokens;
    semantic::split2chs(query.c_str(), tokens);
    semantic::Example ex(0, query, tokens, 1);
    parser->parse(model, ex);
    LOG(INFO) << "parse finish.";
    Json::FastWriter fw;
    for (int i = 0; i < ex._trees.size(); ++i) {
        string jstr = fw.write(ex._trees[i]);
        std::cout << i + 1 << "best result: " << jstr << std::endl;
    }
    delete parser;
    return 0;
}

