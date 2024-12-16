//
// Created by flyon21 on 21.11.24.
//
#include "custom_shared_ptr.h"
#include <unordered_set>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;

/**
 * Modifies the value pointed to by the given shared pointer within a thread-safe context.
 *
 * This function increments the value held by the shared pointer by 10.
 * The operation is protected by a mutex to ensure thread-safe access to
 * the shared pointer in a concurrent environment.
 *
 * @param sharedPtr A reference to a `CustomSharedPtr<int>` pointing to
 *                  the integer value to be modified.
 *
 * @throws std::runtime_error If the shared pointer is null
 *                            when dereferencing.
 */
void modifySharedPtr(CustomSharedPtr<int>& sharedPtr) {
    std::lock_guard<std::mutex> lock(mtx);
    *sharedPtr = *sharedPtr + 10;
    std::cout << "Thread-modified value: " << *sharedPtr << "\n";
}

/**
 * @brief Entry point for testing and demonstrating the functionality of the CustomSharedPtr class.
 *
 * This function performs various operations to validate the behavior of the `CustomSharedPtr`
 * class, including creating and managing shared pointers, demonstrating ownership semantics
 * (such as copy, move, and destructors), and verifying thread-safety when accessing and
 * modifying data within a shared pointer.
 *
 * The following behaviors are demonstrated within the `main` function:
 *
 * - Creation of `CustomSharedPtr` instances and managing their lifetime using reference counting.
 * - Copy assignment behavior of shared pointers and the impact on reference count.
 * - Move semantics for transferring ownership of the managed resource.
 * - Overload of dereference operator (*) and equality operator (==) with shared pointers.
 * - Use of `use_count` to track the number of references to the managed resource.
 * - Storing `CustomSharedPtr` objects in an `std::unordered_set` using a custom hash function.
 * - Impact of scope on the shared pointer's lifecycle and reference count.
 * - Demonstrating thread-safe operations on the shared resource by spawning multiple threads
 *   that concurrently modify the managed object via `modifySharedPtr`.
 */
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

    // Test for threads
    std::cout << "Initial value of ptr4: " << *ptr4 << std::endl;

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(modifySharedPtr, ptr4);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Final value of ptr4: " << *ptr4 << std::endl;
    std::cout << "Final use count of ptr4: " << ptr4.use_count() << std::endl;

    return 0;
}