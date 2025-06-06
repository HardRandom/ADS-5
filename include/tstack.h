// Copyright 2021 NNTU-CS
#ifndef INCLUDE_TSTACK_H_
#define INCLUDE_TSTACK_H_

#include <stdexcept>

template <typename T, int kMaxStackSize>
class TStack {
 public:
    TStack() : pointer_(-1) {}
    bool isVoid() const { return pointer_ == -1; }
    bool isFull() const { return pointer_ == kMaxStackSize - 1; }
    void add(const T& value) {
        if (isFull()) throw std::overflow_error("Stack overflow");
        storage_[++pointer_] = value;
    }
    T remove() {
        if (isVoid()) throw std::underflow_error("Stack underflow");
        return storage_[pointer_--];
    }
    T getTop() const {
        if (isVoid()) throw std::underflow_error("Stack empty");
        return storage_[pointer_];
    }
 private:
    T storage_[kMaxStackSize];
    int pointer_;
};
#endif // INCLUDE_TSTACK_H_
