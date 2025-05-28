#ifndef __CVECTORQUEUE_H__
#define __CVECTORQUEUE_H__

#include "CFFCommon.h"
#include <vector>

template<typename T>
class CVectorQueue {
private:
    typedef void(*dProperty1)(T a, INT64** b);
private:
    std::vector<T> data;
    size_t frontIndex = 0;
    static dProperty1 msProp1;
public:
    CVectorQueue(dProperty1 prop1);
    ~CVectorQueue();
    void push(const T& value);
    void pop();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    bool empty() const;
    size_t size() const;
    void clear();
private:
    void compactIfNeeded();
    static bool less(T a, T b);
    static void swapIfLess(T a, T b);
};

#endif //__CVECTORQUEUE_H__