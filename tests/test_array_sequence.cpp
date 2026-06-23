#include "fixtures.hpp"
#include <gtest/gtest.h>
#include "../headers/arraysequence.hpp"

class ArraySeqFixture : public testing::Test {
protected:
    int data[4] = {10, 20, 30, 40};
    const char* str_data[2] = {"Hello", "World"};
    MutableArraySequence<int>* seq_int;
    MutableArraySequence<const char*>* seq_str;
    void SetUp() override {
        seq_int = new MutableArraySequence<int>(data, 4);
        seq_str = new MutableArraySequence<const char*>(str_data, 2);
    }
    void TearDown() override {
        delete seq_int;
        delete seq_str;
    }
};

TEST_F(ArraySeqFixture, default_constructor) {
    MutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор по умолчанию.\n";
        std::cerr << "Ожидаемый размер: 0\n";
        std::cerr << "Полученный размер: " << seq.GetLength() << "\n\n";
    }
}

TEST_F(ArraySeqFixture, array_constructor) {
    MutableArraySequence<int> seq(data, 4);
    EXPECT_EQ(seq.GetLength(), 4);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(seq.Get(i), data[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от массива.\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 4; ++i) std::cerr << data[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < seq.GetLength(); ++i) std::cerr << seq.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, linked_list_constructor) {
    LinkedList<int> list(data, 4);
    MutableArraySequence<int> seq(list);
    EXPECT_EQ(seq.GetLength(), 4);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(seq.Get(i), data[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от LinkedList.\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 4; ++i) std::cerr << data[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < seq.GetLength(); ++i) std::cerr << seq.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, copy_constructor) {
    MutableArraySequence<int> copy(*seq_int);
    EXPECT_EQ(copy.GetLength(), seq_int->GetLength());
    for (size_t i = 0; i < copy.GetLength(); ++i) {
        EXPECT_EQ(copy.Get(i), seq_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор копирования.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < seq_int->GetLength(); ++i)
            std::cerr << seq_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetLength(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, get_first) {
    EXPECT_EQ(seq_int->GetFirst(), data[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetFirst.\n";
        std::cerr << "Ожидалось " << data[0] << ", получено " << seq_int->GetFirst() << "\n\n";
    }
}

TEST_F(ArraySeqFixture, get_first_empty) {
    MutableArraySequence<int> empty;
    EXPECT_THROW(empty.GetFirst(), OutOfRangeException);
}

TEST_F(ArraySeqFixture, get_last) {
    EXPECT_EQ(seq_int->GetLast(), data[3]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLast.\n";
        std::cerr << "Ожидалось " << data[3] << ", получено " << seq_int->GetLast() << "\n\n";
    }
}

TEST_F(ArraySeqFixture, get_last_empty) {
    MutableArraySequence<int> empty;
    EXPECT_THROW(empty.GetLast(), OutOfRangeException);
}

TEST_F(ArraySeqFixture, get_valid) {
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(seq_int->Get(i), data[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Get (корректные индексы).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 4; ++i) std::cerr << data[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < seq_int->GetLength(); ++i)
            std::cerr << seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, get_out_of_range) {
    EXPECT_THROW(seq_int->Get(10), OutOfRangeException);
    EXPECT_THROW(seq_int->Get(4), OutOfRangeException);
}

TEST_F(ArraySeqFixture, get_length) {
    EXPECT_EQ(seq_int->GetLength(), 4);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLength.\n";
        std::cerr << "Ожидалось 4, получено " << seq_int->GetLength() << "\n\n";
    }
}

TEST_F(ArraySeqFixture, append) {
    seq_int->Append(50);
    EXPECT_EQ(seq_int->GetLength(), 5);
    EXPECT_EQ(seq_int->Get(4), 50);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Append.\n";
        std::cerr << "Массив после добавления: ";
        for (size_t i = 0; i < seq_int->GetLength(); ++i)
            std::cerr << seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, prepend) {
    seq_int->Prepend(5);
    EXPECT_EQ(seq_int->GetLength(), 5);
    EXPECT_EQ(seq_int->Get(0), 5);
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_EQ(seq_int->Get(i), data[i-1]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Prepend.\n";
        std::cerr << "Массив после добавления в начало: ";
        for (size_t i = 0; i < seq_int->GetLength(); ++i)
            std::cerr << seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, insert_at) {
    seq_int->InsertAt(25, 2);
    EXPECT_EQ(seq_int->GetLength(), 5);
    EXPECT_EQ(seq_int->Get(2), 25);
    EXPECT_EQ(seq_int->Get(0), data[0]);
    EXPECT_EQ(seq_int->Get(1), data[1]);
    EXPECT_EQ(seq_int->Get(3), data[2]);
    EXPECT_EQ(seq_int->Get(4), data[3]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt.\n";
        std::cerr << "Массив после вставки: ";
        for (size_t i = 0; i < seq_int->GetLength(); ++i)
            std::cerr << seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, insert_at_end) {
    seq_int->InsertAt(50, 4);
    EXPECT_EQ(seq_int->GetLength(), 5);
    EXPECT_EQ(seq_int->Get(4), 50);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt в конец.\n";
        std::cerr << "Массив после вставки: ";
        for (size_t i = 0; i < seq_int->GetLength(); ++i)
            std::cerr << seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, insert_at_out_of_range) {
    EXPECT_THROW(seq_int->InsertAt(99, 10), OutOfRangeException);
}

TEST_F(ArraySeqFixture, get_subsequence) {
    Sequence<int>* sub = seq_int->GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 2);
    EXPECT_EQ(sub->Get(0), data[1]);
    EXPECT_EQ(sub->Get(1), data[2]);
    delete sub;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetSubsequence (1,3).\n";
        std::cerr << "Ожидалось: " << data[1] << " " << data[2] << "\n";
        if (sub) {
            std::cerr << "Получено: ";
            for (size_t i = 0; i < sub->GetLength(); ++i)
                std::cerr << sub->Get(i) << " ";
            std::cerr << "\n";
        }
        std::cerr << "\n";
    }
}

TEST_F(ArraySeqFixture, get_subsequence_invalid) {
    EXPECT_THROW(seq_int->GetSubsequence(2, 1), OutOfRangeException);
    EXPECT_THROW(seq_int->GetSubsequence(0, 5), OutOfRangeException);
    EXPECT_THROW(seq_int->GetSubsequence(4, 5), OutOfRangeException);
}

TEST_F(ArraySeqFixture, concat) {
    MutableArraySequence<int> other(data, 4);
    Sequence<int>* concat = seq_int->Concat(other);
    EXPECT_EQ(concat->GetLength(), 8);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(concat->Get(i), data[i]);
        EXPECT_EQ(concat->Get(i+4), data[i]);
    }
    delete concat;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Concat.\n";
        std::cerr << "Ожидаемая длина: 8, получена: " << concat->GetLength() << "\n";
        std::cerr << "Последовательность: ";
        for (size_t i = 0; i < concat->GetLength(); ++i)
            std::cerr << concat->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, assignment_operator) {
    MutableArraySequence<int> copy;
    copy = *seq_int;
    EXPECT_EQ(copy.GetLength(), seq_int->GetLength());
    for (size_t i = 0; i < copy.GetLength(); ++i) {
        EXPECT_EQ(copy.Get(i), seq_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем operator=.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < seq_int->GetLength(); ++i)
            std::cerr << seq_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetLength(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ArraySeqFixture, self_assignment) {
    *seq_int = *seq_int;
    EXPECT_EQ(seq_int->GetLength(), 4);
    EXPECT_EQ(seq_int->Get(0), data[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем самоприсваивание.\n";
        std::cerr << "Размер = " << seq_int->GetLength()
                  << ", элемент[0] = " << seq_int->Get(0) << "\n\n";
    }
}

TEST_F(ArraySeqFixture, iterator_begin_end) {
    auto it = seq_int->begin();
    auto end = seq_int->end();
    EXPECT_NE(it, end);
    EXPECT_EQ(*it, data[0]);
    size_t idx = 0;
    for (auto it2 = seq_int->begin(); it2 != seq_int->end(); ++it2) {
        EXPECT_EQ(*it2, data[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 4);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор (обход).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 4; ++i) std::cerr << data[i] << " ";
        std::cerr << "\nПройдено элементов: " << idx << "\n\n";
    }
}

TEST_F(ArraySeqFixture, iterator_empty) {
    MutableArraySequence<int> empty;
    auto it = empty.begin();
    auto end = empty.end();
    EXPECT_EQ(it, end);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор для пустой последовательности.\n";
        std::cerr << "begin == end? " << (it == end) << "\n\n";
    }
}

TEST_F(ArraySeqFixture, immutable_append) {
    ImmutableArraySequence<int> immut(data, 4);
    Sequence<int>* newSeq = immut.Append(50);
    EXPECT_EQ(immut.GetLength(), 4);
    EXPECT_EQ(newSeq->GetLength(), 5);
    EXPECT_EQ(newSeq->Get(4), 50);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableArraySequence::Append.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n";
    }
}

TEST_F(ArraySeqFixture, immutable_prepend) {
    ImmutableArraySequence<int> immut(data, 4);
    Sequence<int>* newSeq = immut.Prepend(5);
    EXPECT_EQ(immut.GetLength(), 4);
    EXPECT_EQ(newSeq->GetLength(), 5);
    EXPECT_EQ(newSeq->Get(0), 5);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableArraySequence::Prepend.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n";
    }
}

TEST_F(ArraySeqFixture, immutable_insert_at) {
    ImmutableArraySequence<int> immut(data, 4);
    Sequence<int>* newSeq = immut.InsertAt(25, 2);
    EXPECT_EQ(immut.GetLength(), 4);
    EXPECT_EQ(newSeq->GetLength(), 5);
    EXPECT_EQ(newSeq->Get(2), 25);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableArraySequence::InsertAt.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n";
    }
}