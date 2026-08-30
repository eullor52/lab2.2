#pragma once 

#include "sequence.hpp"
#include "arraysequence.hpp"
#include "exceptions.h"

template <typename T>
Sequence<T>* Map(const Sequence<T>& seq, T (*func)(T)) {
    Sequence<T>* result = seq.EmptyClone();
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        result->Append(func(*it));
    }
    return result;
}

template <typename T, typename U>
U Reduce(const Sequence<T>& seq, U (*func)(U, T), U init) {
    U result = init;
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        result = func(result, *it);
    }
    return result;
}

template <typename T>
Sequence<T>* Where(const Sequence<T>& seq, bool (*pred)(T)) {
    Sequence<T>* result = seq.EmptyClone();
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        if (pred(*it))
            result->Append(*it);
    }
    return result;
}

template <typename T>
Sequence<T>* MapIndexed(const Sequence<T>& seq, T (*func)(T, size_t)) {
    Sequence<T>* result = seq.EmptyClone();
    size_t index = 0;
    for (auto it = seq.begin(); it != seq.end(); ++it, ++index) {
        result->Append(func(*it, index));
    }
    return result;
}

template <typename T>
struct MinMaxAvg {
    T min;
    T max;
    double avg;
};

template <typename T>
struct MinMaxAvgAcc {
    T min;
    T max;
    T sum;
    size_t count;
};

template <typename T>
MinMaxAvgAcc<T> MinMaxAvgStep(MinMaxAvgAcc<T> acc, T x) {
    if (acc.count == 0) {
        acc.min = x;
        acc.max = x;
    } else {
        if (x < acc.min) acc.min = x;
        if (x > acc.max) acc.max = x;
    }
    acc.sum = acc.sum + x;
    acc.count += 1;
    return acc;
}

template <typename T>
MinMaxAvg<T> ComputeMinMaxAvg(const Sequence<T>& seq) {
    if (seq.GetLength() == 0)
        throw OutOfRangeException("ComputeMinMaxAvg: sequence is empty");
    MinMaxAvgAcc<T> init{T(), T(), static_cast<T>(0), 0};
    MinMaxAvgAcc<T> acc = Reduce<T, MinMaxAvgAcc<T>>(seq, MinMaxAvgStep<T>, init);
    return MinMaxAvg<T>{acc.min, acc.max, static_cast<double>(acc.sum) / static_cast<double>(acc.count)};
}

template <typename T>
Sequence<double>* MovingAverage3(const Sequence<T>& seq) {
    size_t len = seq.GetLength();
    if (len == 0)
        throw OutOfRangeException("MovingAverage3: sequence is empty");
    auto* result = new MutableArraySequence<double>();
    for (size_t i = 0; i < len; ++i) {
        double prev = (i > 0) ? static_cast<double>(seq.Get(i - 1)) : 0.0;
        double cur = static_cast<double>(seq.Get(i));
        double next = (i + 1 < len) ? static_cast<double>(seq.Get(i + 1)) : 0.0;
        result->Append((prev + cur + next) / 3.0);
    }
    return result;
}

template <typename T>
Sequence<T>* ReflectSum(const Sequence<T>& seq) {
    size_t len = seq.GetLength();
    Sequence<T>* result = seq.EmptyClone();
    for (size_t i = 0; i < len; ++i) {
        result->Append(static_cast<T>(seq.Get(i) + seq.Get(len - 1 - i)));
    }
    return result;
}

template <typename T>
MutableArraySequence<Sequence<T>*>* Prefixes(const Sequence<T>& seq) {
    auto* result = new MutableArraySequence<Sequence<T>*>();
    size_t len = seq.GetLength();
    for (size_t i = 0; i < len; ++i) {
        result->Append(seq.GetSubsequence(0, i));
    }
    return result;
}

template <typename T>
MutableArraySequence<Sequence<T>*>* Postfixes(const Sequence<T>& seq) {
    auto* result = new MutableArraySequence<Sequence<T>*>();
    size_t len = seq.GetLength();
    for (size_t i = 0; i < len; ++i) {
        result->Append(seq.GetSubsequence(i, len - 1));
    }
    return result;
}