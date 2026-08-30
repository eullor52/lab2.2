#include "fixtures.hpp"
#include <gtest/gtest.h>
#include "../headers/algorithms.hpp"
#include "../headers/arraysequence.hpp"
#include "../headers/listsequence.hpp"

namespace {
int double_it(int x) { return x * 2; }
bool is_positive(int x) { return x > 0; }
int sum_ints(int acc, int x) { return acc + x; }
int add_idx(int x, size_t idx) { return x + static_cast<int>(idx); }
}

TEST_F(ArraySequenceFixture, map_doubles_elements) {
    Sequence<int>* mapped = Map<int>(*mut_seq_int, double_it);
    ASSERT_EQ(mapped->GetLength(), mut_seq_int->GetLength());
    for (size_t i = 0; i < mapped->GetLength(); ++i) {
        EXPECT_EQ(mapped->Get(i), mut_seq_int->Get(i) * 2);
    }
    delete mapped;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Map (Sequence<T>, x*2) на ArraySequence.\n";
        std::cerr << "Исходные: ";
        for (size_t i = 0; i < mut_seq_int->GetLength(); ++i)
            std::cerr << mut_seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySequenceFixture, reduce_sums_elements) {
    int result = Reduce<int, int>(*mut_seq_int, sum_ints, 0);
    int expected = 0;
    for (size_t i = 0; i < mut_seq_int->GetLength(); ++i)
        expected += mut_seq_int->Get(i);
    EXPECT_EQ(result, expected);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Reduce (сумма) на ArraySequence.\n";
        std::cerr << "Ожидалось " << expected << ", получено " << result << "\n\n";
    }
}

TEST_F(ArraySequenceFixture, where_filters_positive) {
    Sequence<int>* filtered = Where<int>(*mut_seq_int, is_positive);
    for (size_t i = 0; i < filtered->GetLength(); ++i) {
        EXPECT_GT(filtered->Get(i), 0);
    }
    size_t expectedCount = 0;
    for (size_t i = 0; i < mut_seq_int->GetLength(); ++i)
        if (mut_seq_int->Get(i) > 0) ++expectedCount;
    EXPECT_EQ(filtered->GetLength(), expectedCount);
    delete filtered;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Where (x > 0) на ArraySequence.\n";
        std::cerr << "Ожидаемое количество положительных: " << expectedCount << "\n\n";
    }
}

TEST_F(ListSequenceFixture, map_reduce_where_on_list) {
    Sequence<int>* mapped = Map<int>(*list_seq_int, double_it);
    ASSERT_EQ(mapped->GetLength(), list_seq_int->GetLength());
    for (size_t i = 0; i < mapped->GetLength(); ++i)
        EXPECT_EQ(mapped->Get(i), list_seq_int->Get(i) * 2);
    delete mapped;

    int sum = Reduce<int, int>(*list_seq_int, sum_ints, 0);
    int expectedSum = 0;
    for (size_t i = 0; i < list_seq_int->GetLength(); ++i)
        expectedSum += list_seq_int->Get(i);
    EXPECT_EQ(sum, expectedSum);

    Sequence<int>* filtered = Where<int>(*list_seq_int, is_positive);
    EXPECT_EQ(filtered->GetLength(), list_seq_int->GetLength());
    delete filtered;

    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Map/Reduce/Where на ListSequence.\n";
        std::cerr << "Ожидаемая сумма: " << expectedSum << ", получено: " << sum << "\n\n";
    }
}

TEST_F(ArraySequenceFixture, map_indexed_adds_index) {
    Sequence<int>* mapped = MapIndexed<int>(*mut_seq_int, add_idx);
    ASSERT_EQ(mapped->GetLength(), mut_seq_int->GetLength());
    for (size_t i = 0; i < mapped->GetLength(); ++i) {
        EXPECT_EQ(mapped->Get(i), mut_seq_int->Get(i) + static_cast<int>(i));
    }
    delete mapped;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем MapIndexed (x + index).\n";
        std::cerr << "Исходные: ";
        for (size_t i = 0; i < mut_seq_int->GetLength(); ++i)
            std::cerr << mut_seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySequenceFixture, min_max_avg_one_pass) {
    MinMaxAvg<int> stats = ComputeMinMaxAvg<int>(*mut_seq_int);
    int expectedMin = mut_seq_int->Get(0);
    int expectedMax = mut_seq_int->Get(0);
    long long sum = 0;
    for (size_t i = 0; i < mut_seq_int->GetLength(); ++i) {
        int v = mut_seq_int->Get(i);
        if (v < expectedMin) expectedMin = v;
        if (v > expectedMax) expectedMax = v;
        sum += v;
    }
    double expectedAvg = static_cast<double>(sum) / mut_seq_int->GetLength();
    EXPECT_EQ(stats.min, expectedMin);
    EXPECT_EQ(stats.max, expectedMax);
    EXPECT_DOUBLE_EQ(stats.avg, expectedAvg);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ComputeMinMaxAvg.\n";
        std::cerr << "Ожидалось min=" << expectedMin << " max=" << expectedMax
                  << " avg=" << expectedAvg << "\n";
        std::cerr << "Получено min=" << stats.min << " max=" << stats.max
                  << " avg=" << stats.avg << "\n\n";
    }
}

TEST_F(ArraySequenceFixture, min_max_avg_empty_throws) {
    MutableArraySequence<int> empty;
    EXPECT_THROW(ComputeMinMaxAvg<int>(empty), OutOfRangeException);
}

TEST_F(ArraySequenceFixture, moving_average_3) {
    Sequence<double>* avg3 = MovingAverage3<int>(*mut_seq_int);
    ASSERT_EQ(avg3->GetLength(), 4);
    EXPECT_DOUBLE_EQ(avg3->Get(0), (0 + 132 + 23) / 3.0);
    EXPECT_DOUBLE_EQ(avg3->Get(1), (132 + 23 - 56) / 3.0);
    EXPECT_DOUBLE_EQ(avg3->Get(2), (23 - 56 + 98) / 3.0);
    EXPECT_DOUBLE_EQ(avg3->Get(3), (-56 + 98 + 0) / 3.0);
    delete avg3;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем MovingAverage3.\n";
        std::cerr << "Ожидаемая последовательность: "
                  << (0 + 132 + 23) / 3.0 << " " << (132 + 23 - 56) / 3.0 << " "
                  << (23 - 56 + 98) / 3.0 << " " << (-56 + 98 + 0) / 3.0 << "\n\n";
    }
}

TEST_F(ArraySequenceFixture, moving_average_3_empty_throws) {
    MutableArraySequence<int> empty;
    EXPECT_THROW(MovingAverage3<int>(empty), OutOfRangeException);
}

TEST_F(ArraySequenceFixture, reflect_sum) {
    Sequence<int>* reflected = ReflectSum<int>(*mut_seq_int);
    ASSERT_EQ(reflected->GetLength(), 4);
    EXPECT_EQ(reflected->Get(0), 132 + 98);
    EXPECT_EQ(reflected->Get(1), 23 + (-56));
    EXPECT_EQ(reflected->Get(2), -56 + 23);
    EXPECT_EQ(reflected->Get(3), 98 + 132);
    delete reflected;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ReflectSum.\n";
        std::cerr << "Ожидаемая последовательность: " << 132 + 98 << " " << 23 - 56
                  << " " << -56 + 23 << " " << 98 + 132 << "\n\n";
    }
}

TEST_F(ArraySequenceFixture, prefixes) {
    auto* prefixes = Prefixes<int>(*mut_seq_int);
    ASSERT_EQ(prefixes->GetLength(), 4);
    for (size_t i = 0; i < prefixes->GetLength(); ++i) {
        Sequence<int>* p = prefixes->Get(i);
        EXPECT_EQ(p->GetLength(), i + 1);
        for (size_t j = 0; j <= i; ++j)
            EXPECT_EQ(p->Get(j), mut_seq_int->Get(j));
        delete p;
    }
    delete prefixes;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Prefixes.\n";
        std::cerr << "Ожидалось 4 префикса длин 1,2,3,4\n\n";
    }
}

TEST_F(ArraySequenceFixture, postfixes) {
    auto* postfixes = Postfixes<int>(*mut_seq_int);
    ASSERT_EQ(postfixes->GetLength(), 4);
    for (size_t i = 0; i < postfixes->GetLength(); ++i) {
        Sequence<int>* p = postfixes->Get(i);
        size_t expectedLen = mut_seq_int->GetLength() - i;
        EXPECT_EQ(p->GetLength(), expectedLen);
        for (size_t j = 0; j < expectedLen; ++j)
            EXPECT_EQ(p->Get(j), mut_seq_int->Get(i + j));
        delete p;
    }
    delete postfixes;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Postfixes.\n";
        std::cerr << "Ожидалось 4 постфикса длин 4,3,2,1\n\n";
    }
}

TEST_F(ArraySequenceFixture, prefixes_postfixes_empty) {
    MutableArraySequence<int> empty;
    auto* prefixes = Prefixes<int>(empty);
    auto* postfixes = Postfixes<int>(empty);
    EXPECT_EQ(prefixes->GetLength(), 0);
    EXPECT_EQ(postfixes->GetLength(), 0);
    delete prefixes;
    delete postfixes;
}