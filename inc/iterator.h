/**
 * parser.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#ifndef  __ITERATOR_H_
#define  __ITERATOR_H_

namespace semantic {

//------------------------------defination of iterator--------------------------------#

template<typename Item>
class Iterator {
public:
    Iterator();
    Iterator(std::vector<Item>& items);
    ~Iterator();
    
    int init_iterator(std::vector<Item>& items);

    Item& get_value();
    bool next();
    bool done();
    void reset();
private:
    typename std::vector<Item>::iterator _m_iter;
    typename std::vector<Item>::iterator _begin;
    typename std::vector<Item>::iterator _end;

    std::vector<Item> _error_mark;
};

//------------------------------implementation of iteration--------------------------------#

template<typename Item>
Iterator<Item>::Iterator() {
    _m_iter = _error_mark.begin();
    _end = _error_mark.end();
    _begin = _error_mark.begin();
}

template<typename Item>
Iterator<Item>::Iterator(std::vector<Item>& items) {
    int ret = 0;
    ret = init_iterator(items);
    if (ret < 0) {
    }
}

template<typename Item>
Iterator<Item>::~Iterator() {}

template<typename Item>
inline int Iterator<Item>::init_iterator(std::vector<Item>& items) {
    _m_iter = items.begin();
    _begin = items.begin();
    _end = items.end();
    return 0;
}

template<typename Item>
inline Item& Iterator<Item>::get_value() {
    return *_m_iter;
}

template<typename Item>
inline bool Iterator<Item>::done() {
    if (_m_iter == _end) {
        return true;
    }
    return false;
}

template<typename Item>
inline bool Iterator<Item>::next() {
    if (_m_iter == _end) {
        return false;
    }
    ++_m_iter;
    return true;
}

template<typename Item>
inline void Iterator<Item>::reset() {
    _m_iter = _begin;
}

}

#endif  //__ITERATOR_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
