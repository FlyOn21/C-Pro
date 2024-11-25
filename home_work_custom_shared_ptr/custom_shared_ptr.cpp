//
// Created by flyon21 on 21.11.24.
//
#include "custom_shared_ptr.h"

int main() {
    CustomSharedPtr<int> ptr1(new int(42));
    std::cout << "ptr1 count: " << ptr1.use_count() << std::endl;

    {
        CustomSharedPtr<int> ptr2 = ptr1;
        std::cout << "ptr2 count: " << ptr2.use_count() << std::endl;

        *ptr2 = 100;
        std::cout << "ptr1 value: " << *ptr1 << std::endl;
    }

    std::cout << "After ptr2 out of scope: " << ptr1.use_count() << std::endl;

    CustomSharedPtr<int> ptr3 = std::move(ptr1);
    std::cout << "After move, ptr3 count: " << ptr3.use_count() << std::endl;

    return 0;
}
