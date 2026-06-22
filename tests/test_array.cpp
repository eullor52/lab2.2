#include "fixtures.hpp"

TEST_F(DynamicArrayFixture, default_constructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0) << "Дефолтный конструктор должен создавать пустой массив с размером 0";
}

TEST_F(DynamicArrayFixture, array_constructor) {
    ASSERT_NE(array_int, nullptr) << "Фикстура array_int не должна быть nullptr";
    EXPECT_EQ(array_int->GetSize(), 6) << "Размер созданного из массива объекта должен быть равен 6";
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(array_int->Get(i), data_int[i]) << "Элемент на индексе " << i << " не совпадает с исходным массивом данных";
    }
    EXPECT_THROW(DynamicArray<int>(nullptr, 5), std::invalid_argument) << "Передача nullptr при ненулевом размере должна бросать std::invalid_argument";
}

TEST_F(DynamicArrayFixture, size_constructor) {
    DynamicArray<int> arr(5);
    EXPECT_EQ(arr.GetSize(), 5) << "Конструктор размера должен инициализировать массив строго заданной длины";
}

TEST_F(DynamicArrayFixture, copy_constructor) {
    DynamicArray<int> copy(*array_int);
    EXPECT_EQ(copy.GetSize(), array_int->GetSize()) << "Размер скопированного массива должен полностью совпадать с оригиналом";
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(copy.Get(i), array_int->Get(i)) << "Элемент копии на индексе " << i << " отличается от оригинального элемента";
    }
}

TEST_F(DynamicArrayFixture, get_and_set) {
    EXPECT_EQ(array_int->Get(2), 322) << "Метод Get(2) вернул неверное начальное значение";
    array_int->Set(2, 888);
    EXPECT_EQ(array_int->Get(2), 888) << "Метод Get(2) должен вернуть новое значение 888 после вызова Set";
    EXPECT_THROW(array_int->Get(10), std::out_of_range) << "Вызов Get() с индексом за границами массива должен бросать std::out_of_range";
    EXPECT_THROW(array_int->Set(10, 0), std::out_of_range) << "Вызов Set() с индексом за границами массива должен бросать std::out_of_range";
}

TEST_F(DynamicArrayFixture, get_size) {
    EXPECT_EQ(array_int->GetSize(), 6) << "Метод GetSize() вернул неверное количество элементов";
}

TEST_F(DynamicArrayFixture, resize) {
    array_int->Resize(10);
    EXPECT_EQ(array_int->GetSize(), 10) << "После Resize(10) размер массива должен увеличиться до 10";
    EXPECT_EQ(array_int->Get(0), 112) << "Старые данные на индексе 0 должны сохраняться при увеличении размера";
    
    array_int->Resize(3);
    EXPECT_EQ(array_int->GetSize(), 3) << "После Resize(3) размер массива должен уменьшиться до 3";
    EXPECT_EQ(array_int->Get(2), 322) << "Данные внутри усеченной границы (индекс 2) должны остаться нетронутыми";
}

TEST_F(DynamicArrayFixture, operator_brackets) {
    EXPECT_EQ((*array_int)[1], -2) << "Оператор [] вернул неверное значение для чтения";
    (*array_int)[1] = -50;
    EXPECT_EQ((*array_int)[1], -50) << "Неконстантный оператор [] должен позволять изменять данные по индексу";
    
    const auto* const_arr = array_int;
    EXPECT_EQ((*const_arr)[1], -50) << "Константный оператор [] должен корректно считывать данные";
}

TEST_F(DynamicArrayFixture, operator_copy_assignment) {
    DynamicArray<int> other;
    other = *array_int;
    EXPECT_EQ(other.GetSize(), array_int->GetSize()) << "Оператор копирующего присваивания изменил размер на неверный";
    EXPECT_EQ(other.Get(0), array_int->Get(0)) << "Данные на индексе 0 не перенеслись при копирующем присваивании";
}

TEST_F(DynamicArrayFixture, operator_move_assignment) {
    DynamicArray<int> other;
    DynamicArray<int> source(data_int, 3);
    other = std::move(source);
    EXPECT_EQ(other.GetSize(), 3) << "Перемещающее присваивание должно корректно забирать размер временного объекта";
    EXPECT_EQ(other.Get(0), 112) << "Данные перемещенного объекта утеряны или повреждены";
}