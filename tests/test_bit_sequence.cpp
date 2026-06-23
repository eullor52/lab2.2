#include "fixtures.hpp"
#include <gtest/gtest.h>
#include "../headers/bitsequence.hpp"

class MutableBitSeqFixture : public testing::Test {
protected:
    Bit<uint8_t> bits[5] = {
        Bit<uint8_t>(1), Bit<uint8_t>(0), Bit<uint8_t>(1),
        Bit<uint8_t>(1), Bit<uint8_t>(0)
    };
    MutableBitSequence<uint8_t>* seq;
    void SetUp() override {
        seq = new MutableBitSequence<uint8_t>(bits, 5);
    }
    void TearDown() override {
        delete seq;
    }
};

TEST_F(MutableBitSeqFixture, default_constructor) {
    MutableBitSequence<uint8_t> s;
    EXPECT_EQ(s.GetLength(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор по умолчанию.\n";
        std::cerr << "Ожидаемая длина: 0\n";
        std::cerr << "Полученная длина: " << s.GetLength() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, array_constructor) {
    Bit<uint8_t> arr[3] = {Bit<uint8_t>(1), Bit<uint8_t>(0), Bit<uint8_t>(1)};
    MutableBitSequence<uint8_t> s(arr, 3);
    EXPECT_EQ(s.GetLength(), 3);
    EXPECT_EQ(s.Get(0).GetValue(), 1);
    EXPECT_EQ(s.Get(1).GetValue(), 0);
    EXPECT_EQ(s.Get(2).GetValue(), 1);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от массива.\n";
        std::cerr << "Ожидаемые биты: 1 0 1\n";
        std::cerr << "Полученные: " << (int)s.Get(0).GetValue() << " "
                  << (int)s.Get(1).GetValue() << " "
                  << (int)s.Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, array_constructor_null_throws) {
    EXPECT_THROW(MutableBitSequence<uint8_t> s(nullptr, 5), InvalidArgumentException);
}

TEST_F(MutableBitSeqFixture, copy_constructor) {
    MutableBitSequence<uint8_t> copy(*seq);
    EXPECT_EQ(copy.GetLength(), seq->GetLength());
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        EXPECT_EQ(copy.Get(i).GetValue(), seq->Get(i).GetValue());
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор копирования.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < seq->GetLength(); ++i)
            std::cerr << (int)seq->Get(i).GetValue() << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetLength(); ++i)
            std::cerr << (int)copy.Get(i).GetValue() << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, get_length) {
    EXPECT_EQ(seq->GetLength(), 5);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLength.\n";
        std::cerr << "Ожидалось 5, получено " << seq->GetLength() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, get_valid) {
    EXPECT_EQ(seq->Get(0).GetValue(), 1);
    EXPECT_EQ(seq->Get(1).GetValue(), 0);
    EXPECT_EQ(seq->Get(2).GetValue(), 1);
    EXPECT_EQ(seq->Get(3).GetValue(), 1);
    EXPECT_EQ(seq->Get(4).GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Get (корректные индексы).\n";
        std::cerr << "Ожидаемые биты: 1 0 1 1 0\n";
        std::cerr << "Полученные: ";
        for (size_t i = 0; i < seq->GetLength(); ++i)
            std::cerr << (int)seq->Get(i).GetValue() << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, get_out_of_range) {
    EXPECT_THROW(seq->Get(5), OutOfRangeException);
    EXPECT_THROW(seq->Get(100), OutOfRangeException);
}

TEST_F(MutableBitSeqFixture, get_first) {
    EXPECT_EQ(seq->GetFirst().GetValue(), 1);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetFirst.\n";
        std::cerr << "Ожидалось 1, получено " << (int)seq->GetFirst().GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, get_first_empty) {
    MutableBitSequence<uint8_t> empty;
    EXPECT_THROW(empty.GetFirst(), OutOfRangeException);
}

TEST_F(MutableBitSeqFixture, get_last) {
    EXPECT_EQ(seq->GetLast().GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLast.\n";
        std::cerr << "Ожидалось 0, получено " << (int)seq->GetLast().GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, get_last_empty) {
    MutableBitSequence<uint8_t> empty;
    EXPECT_THROW(empty.GetLast(), OutOfRangeException);
}

TEST_F(MutableBitSeqFixture, append) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(0));
    EXPECT_EQ(s.GetLength(), 2);
    EXPECT_EQ(s.Get(0).GetValue(), 1);
    EXPECT_EQ(s.Get(1).GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Append.\n";
        std::cerr << "Ожидаемые биты: 1 0\n";
        std::cerr << "Полученные: " << (int)s.Get(0).GetValue() << " "
                  << (int)s.Get(1).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, append_rvalue) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(0));
    EXPECT_EQ(s.GetLength(), 2);
    EXPECT_EQ(s.Get(0).GetValue(), 1);
    EXPECT_EQ(s.Get(1).GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Append (rvalue).\n";
        std::cerr << "Ожидаемые биты: 1 0\n";
        std::cerr << "Полученные: " << (int)s.Get(0).GetValue() << " "
                  << (int)s.Get(1).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, prepend) {
    MutableBitSequence<uint8_t> s;
    s.Prepend(Bit<uint8_t>(0));
    s.Prepend(Bit<uint8_t>(1));
    EXPECT_EQ(s.GetLength(), 2);
    EXPECT_EQ(s.Get(0).GetValue(), 1);
    EXPECT_EQ(s.Get(1).GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Prepend.\n";
        std::cerr << "Ожидаемые биты: 1 0\n";
        std::cerr << "Полученные: " << (int)s.Get(0).GetValue() << " "
                  << (int)s.Get(1).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, insert_at_beginning) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(0));
    s.Append(Bit<uint8_t>(1));
    s.InsertAt(Bit<uint8_t>(2), 0);
    EXPECT_EQ(s.GetLength(), 3);
    EXPECT_EQ(s.Get(0).GetValue(), 2);
    EXPECT_EQ(s.Get(1).GetValue(), 0);
    EXPECT_EQ(s.Get(2).GetValue(), 1);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в начало).\n";
        std::cerr << "Ожидаемые биты: 2 0 1\n";
        std::cerr << "Полученные: " << (int)s.Get(0).GetValue() << " "
                  << (int)s.Get(1).GetValue() << " "
                  << (int)s.Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, insert_at_middle) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(0));
    s.Append(Bit<uint8_t>(1));
    s.InsertAt(Bit<uint8_t>(2), 1);
    EXPECT_EQ(s.GetLength(), 4);
    EXPECT_EQ(s.Get(0).GetValue(), 1);
    EXPECT_EQ(s.Get(1).GetValue(), 2);
    EXPECT_EQ(s.Get(2).GetValue(), 0);
    EXPECT_EQ(s.Get(3).GetValue(), 1);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в середину).\n";
        std::cerr << "Ожидаемые биты: 1 2 0 1\n";
        std::cerr << "Полученные: " << (int)s.Get(0).GetValue() << " "
                  << (int)s.Get(1).GetValue() << " "
                  << (int)s.Get(2).GetValue() << " "
                  << (int)s.Get(3).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, insert_at_end) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(0));
    s.InsertAt(Bit<uint8_t>(2), 2);
    EXPECT_EQ(s.GetLength(), 3);
    EXPECT_EQ(s.Get(0).GetValue(), 1);
    EXPECT_EQ(s.Get(1).GetValue(), 0);
    EXPECT_EQ(s.Get(2).GetValue(), 2);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в конец).\n";
        std::cerr << "Ожидаемые биты: 1 0 2\n";
        std::cerr << "Полученные: " << (int)s.Get(0).GetValue() << " "
                  << (int)s.Get(1).GetValue() << " "
                  << (int)s.Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, insert_at_out_of_range) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    EXPECT_THROW(s.InsertAt(Bit<uint8_t>(2), 5), OutOfRangeException);
}

TEST_F(MutableBitSeqFixture, get_subsequence) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(0));
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(0));
    BitSequence<uint8_t>* sub = s.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0).GetValue(), 0);
    EXPECT_EQ(sub->Get(1).GetValue(), 1);
    EXPECT_EQ(sub->Get(2).GetValue(), 1);
    delete sub;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetSubsequence (1,3).\n";
        std::cerr << "Ожидаемые биты: 0 1 1\n";
        std::cerr << "Полученные: " << (int)sub->Get(0).GetValue() << " "
                  << (int)sub->Get(1).GetValue() << " "
                  << (int)sub->Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, get_subsequence_invalid) {
    EXPECT_THROW(seq->GetSubsequence(2, 1), OutOfRangeException);
    EXPECT_THROW(seq->GetSubsequence(0, 10), OutOfRangeException);
    EXPECT_THROW(seq->GetSubsequence(5, 6), OutOfRangeException);
}

TEST_F(MutableBitSeqFixture, concat) {
    MutableBitSequence<uint8_t> s1;
    s1.Append(Bit<uint8_t>(1));
    s1.Append(Bit<uint8_t>(0));
    MutableBitSequence<uint8_t> s2;
    s2.Append(Bit<uint8_t>(1));
    s2.Append(Bit<uint8_t>(1));
    s1.Concat(s2);
    EXPECT_EQ(s1.GetLength(), 4);
    EXPECT_EQ(s1.Get(0).GetValue(), 1);
    EXPECT_EQ(s1.Get(1).GetValue(), 0);
    EXPECT_EQ(s1.Get(2).GetValue(), 1);
    EXPECT_EQ(s1.Get(3).GetValue(), 1);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Concat.\n";
        std::cerr << "Ожидаемые биты: 1 0 1 1\n";
        std::cerr << "Полученные: " << (int)s1.Get(0).GetValue() << " "
                  << (int)s1.Get(1).GetValue() << " "
                  << (int)s1.Get(2).GetValue() << " "
                  << (int)s1.Get(3).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, bitwise_and) {
    MutableBitSequence<uint8_t> s1;
    s1.Append(Bit<uint8_t>(1));
    s1.Append(Bit<uint8_t>(0));
    s1.Append(Bit<uint8_t>(1));
    MutableBitSequence<uint8_t> s2;
    s2.Append(Bit<uint8_t>(0));
    s2.Append(Bit<uint8_t>(1));
    s2.Append(Bit<uint8_t>(1));
    BitSequence<uint8_t> res = s1 & s2;
    EXPECT_EQ(res.GetLength(), 3);
    EXPECT_EQ(res.Get(0).GetValue(), 0);
    EXPECT_EQ(res.Get(1).GetValue(), 0);
    EXPECT_EQ(res.Get(2).GetValue(), 1);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем оператор &.\n";
        std::cerr << "Ожидаемые биты: 0 0 1\n";
        std::cerr << "Полученные: " << (int)res.Get(0).GetValue() << " "
                  << (int)res.Get(1).GetValue() << " "
                  << (int)res.Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, bitwise_and_length_mismatch) {
    MutableBitSequence<uint8_t> s1;
    s1.Append(Bit<uint8_t>(1));
    s1.Append(Bit<uint8_t>(0));
    MutableBitSequence<uint8_t> s2;
    s2.Append(Bit<uint8_t>(1));
    EXPECT_THROW(s1 & s2, LengthErrorException);
}

TEST_F(MutableBitSeqFixture, bitwise_or) {
    MutableBitSequence<uint8_t> s1;
    s1.Append(Bit<uint8_t>(1));
    s1.Append(Bit<uint8_t>(0));
    s1.Append(Bit<uint8_t>(0));
    MutableBitSequence<uint8_t> s2;
    s2.Append(Bit<uint8_t>(0));
    s2.Append(Bit<uint8_t>(1));
    s2.Append(Bit<uint8_t>(0));
    BitSequence<uint8_t> res = s1 | s2;
    EXPECT_EQ(res.GetLength(), 3);
    EXPECT_EQ(res.Get(0).GetValue(), 1);
    EXPECT_EQ(res.Get(1).GetValue(), 1);
    EXPECT_EQ(res.Get(2).GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем оператор |.\n";
        std::cerr << "Ожидаемые биты: 1 1 0\n";
        std::cerr << "Полученные: " << (int)res.Get(0).GetValue() << " "
                  << (int)res.Get(1).GetValue() << " "
                  << (int)res.Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, bitwise_xor) {
    MutableBitSequence<uint8_t> s1;
    s1.Append(Bit<uint8_t>(1));
    s1.Append(Bit<uint8_t>(0));
    s1.Append(Bit<uint8_t>(1));
    MutableBitSequence<uint8_t> s2;
    s2.Append(Bit<uint8_t>(0));
    s2.Append(Bit<uint8_t>(1));
    s2.Append(Bit<uint8_t>(1));
    BitSequence<uint8_t> res = s1 ^ s2;
    EXPECT_EQ(res.GetLength(), 3);
    EXPECT_EQ(res.Get(0).GetValue(), 1);
    EXPECT_EQ(res.Get(1).GetValue(), 1);
    EXPECT_EQ(res.Get(2).GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем оператор ^.\n";
        std::cerr << "Ожидаемые биты: 1 1 0\n";
        std::cerr << "Полученные: " << (int)res.Get(0).GetValue() << " "
                  << (int)res.Get(1).GetValue() << " "
                  << (int)res.Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, bitwise_not) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    s.Append(Bit<uint8_t>(0));
    s.Append(Bit<uint8_t>(1));
    BitSequence<uint8_t> res = ~s;
    EXPECT_EQ(res.GetLength(), 3);
    EXPECT_EQ(res.Get(0).GetValue(), 0);
    EXPECT_EQ(res.Get(1).GetValue(), 1);
    EXPECT_EQ(res.Get(2).GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем оператор ~.\n";
        std::cerr << "Ожидаемые биты: 0 1 0\n";
        std::cerr << "Полученные: " << (int)res.Get(0).GetValue() << " "
                  << (int)res.Get(1).GetValue() << " "
                  << (int)res.Get(2).GetValue() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, iterator_begin_end) {
    auto it = seq->begin();
    auto end = seq->end();
    EXPECT_NE(it, end);
    EXPECT_EQ((*it).GetValue(), 1);
    size_t idx = 0;
    Bit<uint8_t> expected[] = {Bit<uint8_t>(1), Bit<uint8_t>(0), Bit<uint8_t>(1), Bit<uint8_t>(1), Bit<uint8_t>(0)};
    for (auto it2 = seq->begin(); it2 != seq->end(); ++it2) {
        EXPECT_EQ((*it2).GetValue(), expected[idx].GetValue());
        ++idx;
    }
    EXPECT_EQ(idx, 5);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор (обход).\n";
        std::cerr << "Ожидаемые биты: 1 0 1 1 0\n";
        std::cerr << "Пройдено элементов: " << idx << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, iterator_empty) {
    MutableBitSequence<uint8_t> empty;
    auto it = empty.begin();
    auto end = empty.end();
    EXPECT_EQ(it, end);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор для пустой последовательности.\n";
        std::cerr << "begin == end? " << (it == end) << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, immutable_append) {
    ImmutableBitSequence<uint8_t> immut;
    immut.Append(Bit<uint8_t>(1));
    immut.Append(Bit<uint8_t>(0));
    Sequence<Bit<uint8_t>>* newSeq = immut.Append(Bit<uint8_t>(1));
    EXPECT_EQ(immut.GetLength(), 2);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0).GetValue(), 1);
    EXPECT_EQ(newSeq->Get(1).GetValue(), 0);
    EXPECT_EQ(newSeq->Get(2).GetValue(), 1);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableBitSequence::Append.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, immutable_prepend) {
    ImmutableBitSequence<uint8_t> immut;
    immut.Append(Bit<uint8_t>(0));
    Sequence<Bit<uint8_t>>* newSeq = immut.Prepend(Bit<uint8_t>(1));
    EXPECT_EQ(immut.GetLength(), 1);
    EXPECT_EQ(newSeq->GetLength(), 2);
    EXPECT_EQ(newSeq->Get(0).GetValue(), 1);
    EXPECT_EQ(newSeq->Get(1).GetValue(), 0);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableBitSequence::Prepend.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, immutable_insert_at) {
    ImmutableBitSequence<uint8_t> immut;
    immut.Append(Bit<uint8_t>(1));
    immut.Append(Bit<uint8_t>(0));
    Sequence<Bit<uint8_t>>* newSeq = immut.InsertAt(Bit<uint8_t>(2), 1);
    EXPECT_EQ(immut.GetLength(), 2);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0).GetValue(), 1);
    EXPECT_EQ(newSeq->Get(1).GetValue(), 2);
    EXPECT_EQ(newSeq->Get(2).GetValue(), 0);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableBitSequence::InsertAt.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n\n";
    }
}

TEST_F(MutableBitSeqFixture, mutable_append_returns_this) {
    MutableBitSequence<uint8_t> s;
    s.Append(Bit<uint8_t>(1));
    BitSequence<uint8_t>* result = s.Append(Bit<uint8_t>(2));
    EXPECT_EQ(result, &s);
}