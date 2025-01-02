//
// Created by flyon21 on 26.12.24.
//
#include "../include/custom_shared_ptr.hpp"

namespace custom_shared_ptr {
    const char* getVersion() {
        return "0.1.0";
    }

    template <typename T>
    void CustomSharedPtr<T>::release() {
        if (ptrCounter) {
            if (ptrCounter->fetch_sub(1) == 1) {
                delete ptrData;
                delete ptrCounter;
                ptrData = nullptr;
                ptrCounter = nullptr;
            }
        }
    }

    template <typename T>
    CustomSharedPtr<T>::CustomSharedPtr(T* value)
            : ptrData(value), ptrCounter(value ? new std::atomic<int>(1) : nullptr) {}

    template <typename T>
    CustomSharedPtr<T>::CustomSharedPtr(const CustomSharedPtr<T>& other)
            : ptrData(other.ptrData), ptrCounter(other.ptrCounter) {
        if (ptrCounter) {
            ptrCounter->fetch_add(1);
        }
    }

    template <typename T>
    CustomSharedPtr<T>::CustomSharedPtr(CustomSharedPtr<T>&& other) noexcept
            : ptrData(other.ptrData), ptrCounter(other.ptrCounter) {
        other.ptrData = nullptr;
        other.ptrCounter = nullptr;
    }

    template <typename T>
    CustomSharedPtr<T>& CustomSharedPtr<T>::operator=(const CustomSharedPtr<T>& other) {
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

    template <typename T>
    CustomSharedPtr<T>& CustomSharedPtr<T>::operator=(CustomSharedPtr<T>&& other) noexcept {
        if (this != &other) {
            release();
            ptrData = other.ptrData;
            ptrCounter = other.ptrCounter;
            other.ptrData = nullptr;
            other.ptrCounter = nullptr;
        }
        return *this;
    }

    template <typename T>
    T& CustomSharedPtr<T>::operator*() const {
        if (!ptrData) {
            throw std::runtime_error("Dereferencing null pointer");
        }
        return *ptrData;
    }

    template <typename T>
    T* CustomSharedPtr<T>::operator->() const {
        if (!ptrData) {
            throw std::runtime_error("Accessing null pointer");
        }
        return ptrData;
    }

    template <typename T>
    CustomSharedPtr<T>::~CustomSharedPtr() {
        release();
    }

    template <typename T>
    int CustomSharedPtr<T>::use_count() const {
        return ptrCounter ? ptrCounter->load() : 0;
    }

    template <typename T>
    bool CustomSharedPtr<T>::operator==(const CustomSharedPtr<T>& other) const {
        return ptrData == other.ptrData;
    }

    template <typename T>
    size_t CustomSharedPtr<T>::Hash::operator()(const CustomSharedPtr<T>& ptr) const {
        return std::hash<T*>()(ptr.ptrData);
    }
}