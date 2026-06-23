#include "fixtures.hpp"
#include <gtest/gtest.h>
#include "../headers/listsequence.hpp"

TEST_F(ListSequenceFixture, default_constructor) {
    MutableListSequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор по умолчанию.\n";
        std::cerr << "Ожидаемая длина: 0\n";
        std::cerr << "Полученная длина: " << seq.GetLength() << "\n\n";
    }
}

TEST_F(ListSequenceFixture, array_constructor) {
    MutableListSequence<int> seq(list_data_int, 6);
    EXPECT_EQ(seq.GetLength(), 6);
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(seq.Get(i), list_data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от массива.\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 6; ++i) std::cerr << list_data_int[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < seq.GetLength(); ++i) std::cerr << seq.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ListSequenceFixture, linked_list_constructor) {
    LinkedList<int> lst(list_data_int, 6);
    MutableListSequence<int> seq(lst);
    EXPECT_EQ(seq.GetLength(), 6);
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(seq.Get(i), list_data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от LinkedList.\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 6; ++i) std::cerr << list_data_int[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < seq.GetLength(); ++i) std::cerr << seq.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ListSequenceFixture, copy_constructor) {
    MutableListSequence<int> copy(*list_seq_int);
    EXPECT_EQ(copy.GetLength(), list_seq_int->GetLength());
    for (size_t i = 0; i < copy.GetLength(); ++i) {
        EXPECT_EQ(copy.Get(i), list_seq_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор копирования.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < list_seq_int->GetLength(); ++i)
            std::cerr << list_seq_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetLength(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ListSequenceFixture, get_first) {
    EXPECT_EQ(list_seq_int->GetFirst(), list_data_int[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetFirst.\n";
        std::cerr << "Ожидалось " << list_data_int[0] << ", получено " << list_seq_int->GetFirst() << "\n\n";
    }
}

TEST_F(ListSequenceFixture, get_first_empty) {
    MutableListSequence<int> empty;
    EXPECT_THROW(empty.GetFirst(), OutOfRangeException);
}

TEST_F(ListSequenceFixture, get_last) {
    EXPECT_EQ(list_seq_int->GetLast(), list_data_int[5]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLast.\n";
        std::cerr << "Ожидалось " << list_data_int[5] << ", получено " << list_seq_int->GetLast() << "\n\n";
    }
}

TEST_F(ListSequenceFixture, get_last_empty) {
    MutableListSequence<int> empty;
    EXPECT_THROW(empty.GetLast(), OutOfRangeException);
}

TEST_F(ListSequenceFixture, get_valid) {
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(list_seq_int->Get(i), list_data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Get (корректные индексы).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 6; ++i) std::cerr << list_data_int[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < list_seq_int->GetLength(); ++i)
            std::cerr << list_seq_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ListSequenceFixture, get_out_of_range) {
    EXPECT_THROW(list_seq_int->Get(6), OutOfRangeException);
    EXPECT_THROW(list_seq_int->Get(100), OutOfRangeException);
}

TEST_F(ListSequenceFixture, get_length) {
    EXPECT_EQ(list_seq_int->GetLength(), 6);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLength.\n";
        std::cerr << "Ожидалось 6, получено " << list_seq_int->GetLength() << "\n\n";
    }
}

TEST_F(ListSequenceFixture, append) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Append.\n";
        std::cerr << "Ожидаемые элементы: 10 20\n";
        std::cerr << "Полученные: " << seq.Get(0) << " " << seq.Get(1) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, prepend) {
    MutableListSequence<int> seq;
    seq.Prepend(20);
    seq.Prepend(10);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Prepend.\n";
        std::cerr << "Ожидаемые элементы: 10 20\n";
        std::cerr << "Полученные: " << seq.Get(0) << " " << seq.Get(1) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, insert_at_beginning) {
    MutableListSequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.InsertAt(10, 0);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в начало).\n";
        std::cerr << "Ожидаемые элементы: 10 20 30\n";
        std::cerr << "Полученные: " << seq.Get(0) << " " << seq.Get(1) << " " << seq.Get(2) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, insert_at_middle) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(30);
    seq.InsertAt(20, 1);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в середину).\n";
        std::cerr << "Ожидаемые элементы: 10 20 30\n";
        std::cerr << "Полученные: " << seq.Get(0) << " " << seq.Get(1) << " " << seq.Get(2) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, insert_at_end) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.InsertAt(30, 2);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в конец).\n";
        std::cerr << "Ожидаемые элементы: 10 20 30\n";
        std::cerr << "Полученные: " << seq.Get(0) << " " << seq.Get(1) << " " << seq.Get(2) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, insert_at_out_of_range) {
    MutableListSequence<int> seq;
    seq.Append(10);
    EXPECT_THROW(seq.InsertAt(20, 5), OutOfRangeException);
}

TEST_F(ListSequenceFixture, get_subsequence) {
    Sequence<int>* sub = list_seq_int->GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), list_data_int[1]);
    EXPECT_EQ(sub->Get(1), list_data_int[2]);
    EXPECT_EQ(sub->Get(2), list_data_int[3]);
    delete sub;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetSubsequence (1,3).\n";
        std::cerr << "Ожидаемые элементы: " << list_data_int[1] << " " << list_data_int[2] << " " << list_data_int[3] << "\n";
        std::cerr << "Полученные: " << sub->Get(0) << " " << sub->Get(1) << " " << sub->Get(2) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, get_subsequence_invalid) {
    EXPECT_THROW(list_seq_int->GetSubsequence(2, 1), OutOfRangeException);
    EXPECT_THROW(list_seq_int->GetSubsequence(0, 10), OutOfRangeException);
    EXPECT_THROW(list_seq_int->GetSubsequence(6, 7), OutOfRangeException);
}

TEST_F(ListSequenceFixture, concat) {
    MutableListSequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    MutableListSequence<int> seq2;
    seq2.Append(3);
    seq2.Append(4);
    Sequence<int>* result = seq1.Concat(seq2);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);
    delete result;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Concat.\n";
        std::cerr << "Ожидаемые элементы: 1 2 3 4\n";
        std::cerr << "Полученные: " << result->Get(0) << " " << result->Get(1) << " " << result->Get(2) << " " << result->Get(3) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, assignment_operator) {
    MutableListSequence<int> copy;
    copy = *list_seq_int;
    EXPECT_EQ(copy.GetLength(), list_seq_int->GetLength());
    for (size_t i = 0; i < copy.GetLength(); ++i) {
        EXPECT_EQ(copy.Get(i), list_seq_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем operator=.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < list_seq_int->GetLength(); ++i)
            std::cerr << list_seq_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetLength(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(ListSequenceFixture, self_assignment) {
    *list_seq_int = *list_seq_int;
    EXPECT_EQ(list_seq_int->GetLength(), 6);
    EXPECT_EQ(list_seq_int->Get(0), list_data_int[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем самоприсваивание.\n";
        std::cerr << "Размер = " << list_seq_int->GetLength()
                  << ", элемент[0] = " << list_seq_int->Get(0) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, iterator_begin_end) {
    auto it = list_seq_int->begin();
    auto end = list_seq_int->end();
    EXPECT_NE(it, end);
    EXPECT_EQ(*it, list_data_int[0]);
    size_t idx = 0;
    for (auto it2 = list_seq_int->begin(); it2 != list_seq_int->end(); ++it2) {
        EXPECT_EQ(*it2, list_data_int[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 6);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор (обход).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 6; ++i) std::cerr << list_data_int[i] << " ";
        std::cerr << "\nПройдено элементов: " << idx << "\n\n";
    }
}

TEST_F(ListSequenceFixture, iterator_empty) {
    MutableListSequence<int> empty;
    auto it = empty.begin();
    auto end = empty.end();
    EXPECT_EQ(it, end);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор для пустой последовательности.\n";
        std::cerr << "begin == end? " << (it == end) << "\n\n";
    }
}

TEST_F(ListSequenceFixture, immutable_append) {
    ImmutableListSequence<int> immut;
    immut.Append(1);
    immut.Append(2);
    Sequence<int>* newSeq = immut.Append(3);
    EXPECT_EQ(immut.GetLength(), 2);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0), 1);
    EXPECT_EQ(newSeq->Get(1), 2);
    EXPECT_EQ(newSeq->Get(2), 3);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableListSequence::Append.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n\n";
    }
}

TEST_F(ListSequenceFixture, immutable_prepend) {
    ImmutableListSequence<int> immut;
    immut.Append(2);
    Sequence<int>* newSeq = immut.Prepend(1);
    EXPECT_EQ(immut.GetLength(), 1);
    EXPECT_EQ(newSeq->GetLength(), 2);
    EXPECT_EQ(newSeq->Get(0), 1);
    EXPECT_EQ(newSeq->Get(1), 2);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableListSequence::Prepend.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n\n";
    }
}

TEST_F(ListSequenceFixture, immutable_insert_at) {
    ImmutableListSequence<int> immut;
    immut.Append(1);
    immut.Append(3);
    Sequence<int>* newSeq = immut.InsertAt(2, 1);
    EXPECT_EQ(immut.GetLength(), 2);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0), 1);
    EXPECT_EQ(newSeq->Get(1), 2);
    EXPECT_EQ(newSeq->Get(2), 3);
    delete newSeq;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем ImmutableListSequence::InsertAt.\n";
        std::cerr << "Исходная длина: " << immut.GetLength()
                  << ", новая длина: " << newSeq->GetLength() << "\n\n";
    }
}

TEST_F(ListSequenceFixture, map) {
    auto double_func = [](int x) { return x * 2; };
    Sequence<int>* mapped = Map<int, int>(*list_seq_int, double_func);
    EXPECT_EQ(mapped->GetLength(), 6);
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(mapped->Get(i), list_data_int[i] * 2);
    }
    delete mapped;
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Map (удвоение).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 6; ++i) std::cerr << list_data_int[i]*2 << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < mapped->GetLength(); ++i)
            std::cerr << mapped->Get(i) << " ";
        std::cerr << "\n\n";
    }
}