#include "stdafx.h"
#include "CVectorQueue.h"

template<typename T>
typename CVectorQueue<T>::dProperty1 CVectorQueue<T>::msProp1 = NULL;

template<typename T>
CVectorQueue<T>::CVectorQueue(dProperty1 prop1)
{
    msProp1 = prop1;
}

template<typename T>
CVectorQueue<T>::~CVectorQueue()
{
}

template<typename T>
void CVectorQueue<T>::compactIfNeeded() 
{
    if (frontIndex > data.size() / 2) 
    {
        data.erase(data.begin(), data.begin() + frontIndex);
        frontIndex = 0;
    }
}

template<typename T>
void CVectorQueue<T>::push(const T& value) 
{
    data.push_back(value);
    if (size() > 1)
    {
        for (auto it1 = data.begin() + frontIndex; it1 < data.end() - 1; ++it1)
        {
            for (auto it2 = data.begin() + frontIndex; it2 < data.end() - 1; ++it2)
            {
                swapIfLess(*it2, *(it2 + 1));
            }
        }
    }
}

template<typename T>
void CVectorQueue<T>::pop() 
{
    if (empty()) 
    {
        throw std::out_of_range("Queue is empty");
    }
    ++frontIndex;
    compactIfNeeded();
}

template<typename T>
T& CVectorQueue<T>::front() 
{
    if (empty()) 
    {
        throw std::out_of_range("Queue is empty");
    }
    return data[frontIndex];
}

template<typename T>
const T& CVectorQueue<T>::front() const 
{
    if (empty()) 
    {
        throw std::out_of_range("Queue is empty");
    }
    return data[frontIndex];
}

template<typename T>
T& CVectorQueue<T>::back() 
{
    if (empty()) 
    {
        throw std::out_of_range("Queue is empty");
    }
    return data.back();
}

template<typename T>
const T& CVectorQueue<T>::back() const 
{
    if (empty()) 
    {
        throw std::out_of_range("Queue is empty");
    }
    return data.back();
}

template<typename T>
bool CVectorQueue<T>::empty() const 
{
    return frontIndex >= data.size();
}

template<typename T>
size_t CVectorQueue<T>::size() const 
{
    return data.size() - frontIndex;
}

template<typename T>
void CVectorQueue<T>::clear() 
{
    data.clear();
    frontIndex = 0;
}

template<typename T>
bool CVectorQueue<T>::less(T a, T b)
{
    INT64* aTemp = NULL;
    INT64** aPTS = &aTemp;
    INT64* bTemp = NULL;
    INT64** bPTS = &bTemp;
    msProp1(a, aPTS);
    msProp1(b, bPTS);
    return **aPTS < **bPTS;
}

template<typename T>
void CVectorQueue<T>::swapIfLess(T a, T b)
{
    INT64* aTemp = NULL;
    INT64** aPTS = &aTemp;
    INT64* bTemp = NULL;
    INT64** bPTS = &bTemp;
    INT64 temp;
    msProp1(a, aPTS);
    msProp1(b, bPTS);
    if (**aPTS > **bPTS)
    {
        temp = **aPTS;
        **aPTS = **bPTS;
        **bPTS = temp;
    }
}