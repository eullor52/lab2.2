#pragma once 

#include "sequence.hpp"

template <typename T>
Sequence<T>* Map(const Sequence<T>& seq, T (*func)(const T&)) {
    Sequence<T>* result = seq.EmptyClone();
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        result->Append(func(*it));
    }
    return result;
}

template <typename T>
T Reduce(const Sequence<T>& seq, T (*func)(const T&, const T&), T init) {
    T result = init;
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        result = func(result, *it);
    }
    return result;
}

template <typename T>
Sequence<T>* Where(const Sequence<T>& seq, bool (*pred)(const T&)) {
    Sequence<T>* result = seq.EmptyClone();
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        if (pred(*it))
            result->Append(*it);
    }
    return result;
}