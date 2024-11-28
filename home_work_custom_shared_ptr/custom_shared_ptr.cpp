//
// Created by flyon21 on 21.11.24.
//
#include "custom_shared_ptr.h"
#include "unordered_set"

int main() {
    CustomSharedPtr<int> ptr1(new int(42));
    std::cout << "ptr1 count: " << ptr1.use_count() << std::endl;
    CustomSharedPtr<int> ptr2 (new int(56));

    {
        CustomSharedPtr<int> ptr3 = ptr1;
        std::cout << "ptr2 count: " << ptr3.use_count() << std::endl;

        *ptr3 = 100;
        std::cout << "ptr1 value: " << *ptr1 << std::endl;
        std::cout << "ptr3 == ptr1: " << (ptr3 == ptr1) << std::endl;
    }
    std::cout << "ptr2 == ptr1: " << (ptr2 == ptr1) << std::endl;
    std::unordered_set<CustomSharedPtr<int>, CustomSharedPtr<int>::Hash> set_ptr;
    set_ptr.insert(ptr1);
    set_ptr.insert(ptr2);

    std::cout << "Set data: " << std::endl;
    for (auto &ptr : set_ptr) {
        std::cout << *ptr << std::endl;
    }

    std::cout << "After ptr2 out of scope: " << ptr1.use_count() << std::endl;

    CustomSharedPtr<int> ptr4 = std::move(ptr1);
    std::cout << "After move, ptr4 count: " << ptr4.use_count() << std::endl;

    return 0;
}
