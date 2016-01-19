/**
 * wsg_and_pos.h
 *
 * Author: wushiwei
 * Created on: 2016-01-14
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#ifndef BASENLP_WSG_AND_POS_H
#define BASENLP_WSG_AND_POS_H

#include "crf_tagger.h"

namespace basenlp {

// [start, end) represent span index.
typedef struct __WsgPosRes__ {
    int start;
    int end;
    std::string pos_tag;
    std::string word;
} WsgPosRes;

class WordSegAndPosTag {
public:
    virtual ~WordSegAndPosTag() {}
    virtual void parse(const std::vector<std::string>& sequence, 
                    std::vector<WsgPosRes>& results) = 0;
};

class CRFWordSegAndPosTag : public WordSegAndPosTag {
public:
    friend class CRFWordSegAndPosTagResource;

    CRFWordSegAndPosTag() : _tagger(NULL) {}
    ~CRFWordSegAndPosTag();
    
    void parse(const std::vector<std::string>& sequence, std::vector<WsgPosRes>& results);
private:
    void collect_results(const std::vector<std::string>& sequence, 
                         const std::vector<std::string>& tags,
                         std::vector<WsgPosRes>& results);
protected:
    CRFTagger* _tagger;

    static const std::string BEGIN_SUFFIX;
    static const std::string END_SUFFIX;
    static const std::string IN_SUFFIX;
    static const std::string OUT_SUFFIX;
};

class WordSegAndPosTagResource {
public:
    WordSegAndPosTagResource() {}
    virtual ~WordSegAndPosTagResource() {}
    virtual int init(const std::string& model_path) = 0;
    virtual WordSegAndPosTag* create_worker() = 0;
};

class CRFWordSegAndPosTagResource : public WordSegAndPosTagResource {
public:
    CRFWordSegAndPosTagResource() : _model(NULL) {}
    ~CRFWordSegAndPosTagResource();
    
    int init(const std::string& model_path);
    CRFTagger* create_crf_tagger();

    WordSegAndPosTag* create_worker();
protected:
    CRFModel* _model; 
};

}

#endif // BASENLP_WSG_AND_POS_H 

