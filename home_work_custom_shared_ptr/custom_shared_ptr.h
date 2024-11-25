//
// Created by flyon21 on 21.11.24.
//

#ifndef C_PRO_CUSTOM_SHARED_PTR_H_custom_shared_ptr
#define C_PRO_CUSTOM_SHARED_PTR_H_custom_shared_ptr

#include <iostream>
#include <atomic>
#include <stdexcept>

template <typename T>
class CustomSharedPtr {
private:
    T* ptrData;
    std::atomic<int>* ptrCounter;

    void release() {
        if (ptrCounter) {
            if (ptrCounter->fetch_sub(1) == 1) {
                delete ptrData;
                delete ptrCounter;
                ptrData = nullptr;
                ptrCounter = nullptr;
            }
        }
    }

public:

    explicit CustomSharedPtr(T* value = nullptr)
            : ptrData(value), ptrCounter(value ? new std::atomic<int>(1) : nullptr) {}

    CustomSharedPtr(const CustomSharedPtr<T>& other)
            : ptrData(other.ptrData), ptrCounter(other.ptrCounter) {
        if (ptrCounter) {
            ptrCounter->fetch_add(1);
        }
    }

    CustomSharedPtr(CustomSharedPtr<T>&& other) noexcept
            : ptrData(other.ptrData), ptrCounter(other.ptrCounter) {
        other.ptrData = nullptr;
        other.ptrCounter = nullptr;
    }

    CustomSharedPtr<T>& operator=(const CustomSharedPtr<T>& other) {
        if (this != &other) {
            release();
            ptrData = other.ptrData;
            ptrCounter = other.ptrCounter;
            if (ptrCounter) {
                ptrCounter->fetch_add(1);
            }
        }
        return *this;
    }

    CustomSharedPtr<T>& operator=(CustomSharedPtr<T>&& other) noexcept {
        if (this != &other) {
            release();
            ptrData = other.ptrData;
            ptrCounter = other.ptrCounter;
            other.ptrData = nullptr;
            other.ptrCounter = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        if (!ptrData) {
            throw std::runtime_error("Dereferencing null pointer");
        }
        return *ptrData;
    }

    T* operator->() const {
        if (!ptrData) {
            throw std::runtime_error("Accessing null pointer");
        }
        return ptrData;
    }

    ~CustomSharedPtr() {
        release();
    }

    int use_count() const {
        return ptrCounter ? ptrCounter->load() : 0;
    }
};

#endif //C_PRO_CUSTOM_SHARED_PTR_H_custom_shared_ptr
