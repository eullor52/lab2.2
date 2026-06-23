#include "fixtures.hpp"
#include <gtest/gtest.h>
#include "../headers/array.hpp"

TEST_F(DynamicArrayFixture, default_constructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор по умолчанию.\n";
        std::cerr << "Ожидаемый размер: 0\n";
        std::cerr << "Полученный размер: " << arr.GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, array_constructor_valid) {
    DynamicArray<int> arr(data_int, 6);
    ASSERT_EQ(arr.GetSize(), 6)
        << "\nТестируем конструктор от массива.\n"
        << "Ожидаемый размер 6, получен размер " << arr.GetSize() << "\n\n";
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(arr.Get(i), data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от массива.\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (int j = 0; j < 6; ++j) std::cerr << data_int[j] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < arr.GetSize(); ++i) std::cerr << arr.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, array_constructor_zero_count) {
    DynamicArray<int> arr(nullptr, 0);
    EXPECT_EQ(arr.GetSize(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от массива с count=0.\n";
        std::cerr << "Размер = " << arr.GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, array_constructor_null_throws) {
    EXPECT_THROW(DynamicArray<int> arr(nullptr, 5), InvalidArgumentException);
}

TEST_F(DynamicArrayFixture, copy_constructor_copy) {
    DynamicArray<int> copy(*array_int);
    ASSERT_EQ(copy.GetSize(), array_int->GetSize())
        << "\nТестируем конструктор копирования.\n"
        << "Размер оригинала: " << array_int->GetSize()
        << ", размер копии: " << copy.GetSize() << "\n\n";
    for (size_t i = 0; i < copy.GetSize(); ++i) {
        EXPECT_EQ(copy.Get(i), array_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор копирования.\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << array_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetSize(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, copy_constructor_independence) {
    DynamicArray<int> copy(*array_int);
    copy.Set(0, 999);
    EXPECT_NE(copy.Get(0), array_int->Get(0));
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем независимость копии.\n";
        std::cerr << "Оригинал[0] = " << array_int->Get(0)
                  << ", копия[0] = " << copy.Get(0) << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, copy_constructor_empty) {
    DynamicArray<int> empty;
    DynamicArray<int> copy(empty);
    EXPECT_EQ(copy.GetSize(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор копирования пустого массива.\n";
        std::cerr << "Размер копии = " << copy.GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, size_constructor) {
    DynamicArray<int> arr(5);
    EXPECT_EQ(arr.GetSize(), 5);
    for (size_t i = 0; i < arr.GetSize(); ++i) {
        EXPECT_EQ(arr.Get(i), 0);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор от размера (5).\n";
        std::cerr << "Полученная последовательность: ";
        for (size_t i = 0; i < arr.GetSize(); ++i)
            std::cerr << arr.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, size_constructor_zero) {
    DynamicArray<int> arr(0);
    EXPECT_EQ(arr.GetSize(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор с размером 0.\n";
        std::cerr << "Размер = " << arr.GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, get_size_int) {
    EXPECT_EQ(array_int->GetSize(), 6);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetSize (int).\n";
        std::cerr << "Ожидалось 6, получено " << array_int->GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, get_size_str) {
    EXPECT_EQ(array_str->GetSize(), 6);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetSize (const char*).\n";
        std::cerr << "Ожидалось 6, получено " << array_str->GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, get_valid) {
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(array_int->Get(i), data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Get (корректные индексы).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (size_t i = 0; i < 6; ++i) std::cerr << data_int[i] << " ";
        std::cerr << "\nПолученная: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << array_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, get_out_of_range) {
    EXPECT_THROW(array_int->Get(100), OutOfRangeException);
    EXPECT_THROW(array_int->Get(6), OutOfRangeException);
}

TEST_F(DynamicArrayFixture, set_valid) {
    array_int->Set(0, 999);
    EXPECT_EQ(array_int->Get(0), 999);
    array_int->Set(5, -77);
    EXPECT_EQ(array_int->Get(5), -77);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Set (корректные индексы).\n";
        std::cerr << "Массив после изменений: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << array_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, set_out_of_range) {
    EXPECT_THROW(array_int->Set(100, 42), OutOfRangeException);
}

TEST_F(DynamicArrayFixture, resize_increase) {
    size_t old = array_int->GetSize();
    array_int->Resize(10);
    EXPECT_EQ(array_int->GetSize(), 10);
    for (size_t i = 0; i < old; ++i) {
        EXPECT_EQ(array_int->Get(i), data_int[i]);
    }
    for (size_t i = old; i < 10; ++i) {
        EXPECT_EQ(array_int->Get(i), 0);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Resize (увеличение до 10).\n";
        std::cerr << "Ожидаемые старые элементы: ";
        for (size_t i = 0; i < old; ++i) std::cerr << data_int[i] << " ";
        std::cerr << "\nПолученная последовательность: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << array_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, resize_decrease) {
    array_int->Resize(3);
    EXPECT_EQ(array_int->GetSize(), 3);
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(array_int->Get(i), data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Resize (уменьшение до 3).\n";
        std::cerr << "Ожидаемые первые 3 элемента: ";
        for (size_t i = 0; i < 3; ++i) std::cerr << data_int[i] << " ";
        std::cerr << "\nПолученная последовательность: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << array_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, resize_to_zero) {
    array_int->Resize(0);
    EXPECT_EQ(array_int->GetSize(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Resize до 0.\n";
        std::cerr << "Размер = " << array_int->GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, resize_same_size) {
    size_t old = array_int->GetSize();
    array_int->Resize(old);
    EXPECT_EQ(array_int->GetSize(), old);
    for (size_t i = 0; i < old; ++i) {
        EXPECT_EQ(array_int->Get(i), data_int[i]);
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем Resize с тем же размером.\n";
        std::cerr << "Массив после Resize: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << array_int->Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, assignment_operator_copy) {
    DynamicArray<int> copy;
    copy = *array_int;
    ASSERT_EQ(copy.GetSize(), array_int->GetSize())
        << "\nТестируем operator= (копирование).\n"
        << "Размер оригинала: " << array_int->GetSize()
        << ", копии: " << copy.GetSize() << "\n\n";
    for (size_t i = 0; i < copy.GetSize(); ++i) {
        EXPECT_EQ(copy.Get(i), array_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем operator= (копирование).\n";
        std::cerr << "Оригинал: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << array_int->Get(i) << " ";
        std::cerr << "\nКопия: ";
        for (size_t i = 0; i < copy.GetSize(); ++i)
            std::cerr << copy.Get(i) << " ";
        std::cerr << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, assignment_operator_empty) {
    DynamicArray<int> empty;
    empty = *array_int;
    EXPECT_EQ(empty.GetSize(), array_int->GetSize());
    for (size_t i = 0; i < empty.GetSize(); ++i) {
        EXPECT_EQ(empty.Get(i), array_int->Get(i));
    }
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем operator= (пустой массив).\n";
        std::cerr << "Размер пустого = " << empty.GetSize()
                  << ", ожидалось " << array_int->GetSize() << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, assignment_operator_self) {
    *array_int = *array_int;
    EXPECT_EQ(array_int->GetSize(), 6);
    EXPECT_EQ(array_int->Get(0), data_int[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем самоприсваивание.\n";
        std::cerr << "Размер = " << array_int->GetSize()
                  << ", элемент[0] = " << array_int->Get(0) << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, iterator_begin) {
    auto it = array_int->begin();
    EXPECT_NE(it, array_int->end());
    EXPECT_EQ(*it, data_int[0]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор begin.\n";
        std::cerr << "Ожидаемое значение: " << data_int[0]
                  << ", получено: " << *it << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, iterator_traversal) {
    size_t idx = 0;
    for (auto it = array_int->begin(); it != array_int->end(); ++it) {
        EXPECT_EQ(*it, data_int[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, array_int->GetSize());
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор (обход).\n";
        std::cerr << "Ожидаемая последовательность: ";
        for (size_t i = 0; i < array_int->GetSize(); ++i)
            std::cerr << data_int[i] << " ";
        std::cerr << "\nПройдено элементов: " << idx << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, iterator_comparison_increment) {
    auto it1 = array_int->begin();
    auto it2 = array_int->begin();
    EXPECT_EQ(it1, it2);
    ++it2;
    EXPECT_NE(it1, it2);
    auto it3 = array_int->begin();
    EXPECT_EQ(*it3, data_int[0]);
    ++it3;
    EXPECT_EQ(*it3, data_int[1]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем сравнение и инкремент итераторов.\n";
        std::cerr << "it1 == it2? " << (it1 == it2) << "\n";
        std::cerr << "*it3 (после ++) = " << *it3 << ", ожидалось " << data_int[1] << "\n\n";
    }
}

TEST_F(DynamicArrayFixture, iterator_empty) {
    DynamicArray<int> empty;
    auto it = empty.begin();
    auto end = empty.end();
    EXPECT_EQ(it, end);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем итератор для пустого массива.\n";
        std::cerr << "begin == end? " << (it == end) << "\n\n";
    }
}