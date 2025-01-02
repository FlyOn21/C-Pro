//
// Created by flyon21 on 21.11.24.
//

#ifndef C_PRO_CUSTOM_SHARED_PTR_HPP_custom_shared_ptr
#define C_PRO_CUSTOM_SHARED_PTR_HPP_custom_shared_ptr

#include <iostream>
#include <atomic>
#include <stdexcept>
namespace custom_shared_ptr {
    template <typename T>
    class CustomSharedPtr {
    private:
        T* ptrData;
        std::atomic<int>* ptrCounter;
        void release();

    public:
        explicit CustomSharedPtr(T* value = nullptr);
        CustomSharedPtr(const CustomSharedPtr<T>& other);
        CustomSharedPtr(CustomSharedPtr<T>&& other) noexcept;
        CustomSharedPtr<T>& operator=(const CustomSharedPtr<T>& other);
        CustomSharedPtr<T>& operator=(CustomSharedPtr<T>&& other) noexcept;

        T& operator*() const;
        T* operator->() const;
        ~CustomSharedPtr();

        int use_count() const;
        bool operator==(const CustomSharedPtr<T>& other) const;

        struct Hash {
            size_t operator()(const CustomSharedPtr<T>& ptr) const;
        };
    };

    const char* getVersion();
}


#endif //C_PRO_CUSTOM_SHARED_PTR_HPP_custom_shared_ptr
