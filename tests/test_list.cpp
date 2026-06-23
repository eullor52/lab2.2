#include "fixtures.hpp"
#include <gtest/gtest.h>
#include "../headers/list.hpp"

TEST_F(LinkedListFixture, default_constructor) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetLength(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор по умолчанию.\n";
        std::cerr << "Ожидаемая длина: 0\n";
        std::cerr << "Полученная длина: " << list.GetLength() << "\n\n";
    }
}

TEST_F(LinkedListFixture, array_constructor_valid) {
    LinkedList<int> list(list_data_int, 5);
    EXPECT_EQ(list.GetLength(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(list.Get(i), list_data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от массива.\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 5; ++i) std::cerr << list_data_int[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < list.GetLength(); ++i) std::cerr << list.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(LinkedListFixture, array_constructor_zero_count) {
    LinkedList<int> list(nullptr, 0);
    EXPECT_EQ(list.GetLength(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор с count=0.\n";
        std::cerr << "Ожидаемая длина: 0, получено: " << list.GetLength() << "\n\n";
    }
}

TEST_F(LinkedListFixture, array_constructor_null_throws) {
    EXPECT_THROW(LinkedList<int> list(nullptr, 5), InvalidArgumentException);
}

TEST_F(LinkedListFixture, copy_constructor) {
    LinkedList<int> copy(*linked_int);
    EXPECT_EQ(copy.GetLength(), linked_int->GetLength());
    for (size_t i = 0; i < copy.GetLength(); ++i) {
        EXPECT_EQ(copy.Get(i), linked_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор копирования.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < linked_int->GetLength(); ++i)
            std::cerr << linked_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetLength(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(LinkedListFixture, assignment_operator) {
    LinkedList<int> copy;
    copy = *linked_int;
    EXPECT_EQ(copy.GetLength(), linked_int->GetLength());
    for (size_t i = 0; i < copy.GetLength(); ++i) {
        EXPECT_EQ(copy.Get(i), linked_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем operator=.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < linked_int->GetLength(); ++i)
            std::cerr << linked_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetLength(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(LinkedListFixture, self_assignment) {
    *linked_int = *linked_int;
    EXPECT_EQ(linked_int->GetLength(), 5);
    EXPECT_EQ(linked_int->Get(0), list_data_int[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем самоприсваивание.\n";
        std::cerr << "Размер = " << linked_int->GetLength()
                  << ", элемент[0] = " << linked_int->Get(0) << "\n\n";
    }
}

TEST_F(LinkedListFixture, get_length) {
    EXPECT_EQ(linked_int->GetLength(), 5);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLength.\n";
        std::cerr << "Ожидалось 5, получено " << linked_int->GetLength() << "\n\n";
    }
}

TEST_F(LinkedListFixture, get_first) {
    EXPECT_EQ(linked_int->GetFirst(), list_data_int[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetFirst.\n";
        std::cerr << "Ожидалось " << list_data_int[0] << ", получено " << linked_int->GetFirst() << "\n\n";
    }
}

TEST_F(LinkedListFixture, get_first_empty) {
    LinkedList<int> empty;
    EXPECT_THROW(empty.GetFirst(), OutOfRangeException);
}

TEST_F(LinkedListFixture, get_last) {
    EXPECT_EQ(linked_int->GetLast(), list_data_int[4]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetLast.\n";
        std::cerr << "Ожидалось " << list_data_int[4] << ", получено " << linked_int->GetLast() << "\n\n";
    }
}

TEST_F(LinkedListFixture, get_last_empty) {
    LinkedList<int> empty;
    EXPECT_THROW(empty.GetLast(), OutOfRangeException);
}

TEST_F(LinkedListFixture, get_valid) {
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(linked_int->Get(i), list_data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Get (корректные индексы).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 5; ++i) std::cerr << list_data_int[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < linked_int->GetLength(); ++i)
            std::cerr << linked_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(LinkedListFixture, get_out_of_range) {
    EXPECT_THROW(linked_int->Get(5), OutOfRangeException);
    EXPECT_THROW(linked_int->Get(100), OutOfRangeException);
}

TEST_F(LinkedListFixture, append) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    EXPECT_EQ(list.GetLength(), 2);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Append.\n";
        std::cerr << "Ожидаемые элементы: 10 20\n";
        std::cerr << "Полученные: " << list.Get(0) << " " << list.Get(1) << "\n\n";
    }
}

TEST_F(LinkedListFixture, prepend) {
    LinkedList<int> list;
    list.Prepend(20);
    list.Prepend(10);
    EXPECT_EQ(list.GetLength(), 2);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Prepend.\n";
        std::cerr << "Ожидаемые элементы: 10 20\n";
        std::cerr << "Полученные: " << list.Get(0) << " " << list.Get(1) << "\n\n";
    }
}

TEST_F(LinkedListFixture, insert_at_beginning) {
    LinkedList<int> list;
    list.Append(20);
    list.Append(30);
    list.InsertAt(10, 0);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в начало).\n";
        std::cerr << "Ожидаемые элементы: 10 20 30\n";
        std::cerr << "Полученные: " << list.Get(0) << " " << list.Get(1) << " " << list.Get(2) << "\n\n";
    }
}

TEST_F(LinkedListFixture, insert_at_middle) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(30);
    list.InsertAt(20, 1);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в середину).\n";
        std::cerr << "Ожидаемые элементы: 10 20 30\n";
        std::cerr << "Полученные: " << list.Get(0) << " " << list.Get(1) << " " << list.Get(2) << "\n\n";
    }
}

TEST_F(LinkedListFixture, insert_at_end) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.InsertAt(30, 2);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем InsertAt (в конец).\n";
        std::cerr << "Ожидаемые элементы: 10 20 30\n";
        std::cerr << "Полученные: " << list.Get(0) << " " << list.Get(1) << " " << list.Get(2) << "\n\n";
    }
}

TEST_F(LinkedListFixture, insert_at_out_of_range) {
    LinkedList<int> list;
    list.Append(10);
    EXPECT_THROW(list.InsertAt(20, 5), OutOfRangeException);
}

TEST_F(LinkedListFixture, get_sublist_valid) {
    LinkedList<int> sub = linked_int->GetSubList(1, 3);
    EXPECT_EQ(sub.GetLength(), 3);
    EXPECT_EQ(sub.Get(0), list_data_int[1]);
    EXPECT_EQ(sub.Get(1), list_data_int[2]);
    EXPECT_EQ(sub.Get(2), list_data_int[3]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetSubList (1,3).\n";
        std::cerr << "Ожидаемые элементы: " << list_data_int[1] << " " << list_data_int[2] << " " << list_data_int[3] << "\n";
        std::cerr << "Полученные: " << sub.Get(0) << " " << sub.Get(1) << " " << sub.Get(2) << "\n\n";
    }
}

TEST_F(LinkedListFixture, get_sublist_empty) {
    LinkedList<int> empty;
    LinkedList<int> sub = empty.GetSubList(0, 0);
    EXPECT_EQ(sub.GetLength(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetSubList для пустого списка с (0,0).\n";
        std::cerr << "Длина результата = " << sub.GetLength() << ", ожидалось 0\n\n";
    }
}

TEST_F(LinkedListFixture, get_sublist_invalid) {
    EXPECT_THROW(linked_int->GetSubList(2, 1), OutOfRangeException);
    EXPECT_THROW(linked_int->GetSubList(0, 5), OutOfRangeException);
    EXPECT_THROW(linked_int->GetSubList(5, 6), OutOfRangeException);
    LinkedList<int> empty;
    EXPECT_THROW(empty.GetSubList(0, 1), OutOfRangeException);
}

TEST_F(LinkedListFixture, concat) {
    LinkedList<int> list1;
    list1.Append(1);
    list1.Append(2);
    LinkedList<int> list2;
    list2.Append(3);
    list2.Append(4);
    LinkedList<int> result = list1.Concat(&list2);
    EXPECT_EQ(result.GetLength(), 4);
    EXPECT_EQ(result.Get(0), 1);
    EXPECT_EQ(result.Get(1), 2);
    EXPECT_EQ(result.Get(2), 3);
    EXPECT_EQ(result.Get(3), 4);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Concat.\n";
        std::cerr << "Ожидаемые элементы: 1 2 3 4\n";
        std::cerr << "Полученные: " << result.Get(0) << " " << result.Get(1) << " " << result.Get(2) << " " << result.Get(3) << "\n\n";
    }
}

TEST_F(LinkedListFixture, concat_null) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    LinkedList<int> result = list.Concat(nullptr);
    EXPECT_EQ(result.GetLength(), 2);
    EXPECT_EQ(result.Get(0), 1);
    EXPECT_EQ(result.Get(1), 2);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Concat с nullptr.\n";
        std::cerr << "Ожидаемые элементы: 1 2\n";
        std::cerr << "Полученные: " << result.Get(0) << " " << result.Get(1) << "\n\n";
    }
}

TEST_F(LinkedListFixture, iterator_begin_end) {
    auto it = linked_int->begin();
    auto end = linked_int->end();
    EXPECT_NE(it, end);
    EXPECT_EQ(*it, list_data_int[0]);
    size_t idx = 0;
    for (auto it2 = linked_int->begin(); it2 != linked_int->end(); ++it2) {
        EXPECT_EQ(*it2, list_data_int[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 5);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор (обход).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int i = 0; i < 5; ++i) std::cerr << list_data_int[i] << " ";
        std::cerr << "\nПройдено элементов: " << idx << "\n\n";
    }
}

TEST_F(LinkedListFixture, iterator_empty) {
    LinkedList<int> empty;
    auto it = empty.begin();
    auto end = empty.end();
    EXPECT_EQ(it, end);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор для пустого списка.\n";
        std::cerr << "begin == end? " << (it == end) << "\n\n";
    }
}