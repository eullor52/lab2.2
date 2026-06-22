#include "fixtures.hpp"

TEST_F(LinkedListFixture, default_constructor) {
    LinkedList<int> empty;
    EXPECT_EQ(empty.GetLength(), 0) << "Дефолтный конструктор должен инициализировать пустой список с длиной 0";
    EXPECT_THROW(empty.GetFirst(), std::out_of_range) << "Вызов GetFirst() на пустом списке обязан генерировать std::out_of_range";
    EXPECT_THROW(empty.GetLast(), std::out_of_range) << "Вызов GetLast() на пустом списке обязан генерировать std::out_of_range";
}

TEST_F(LinkedListFixture, array_constructor) {
    ASSERT_NE(linked_int, nullptr) << "Фикстура linked_int не инициализирована";
    EXPECT_EQ(linked_int->GetLength(), 5) << "Конструктор по массиву определил неверную длину списка";
    EXPECT_EQ(linked_int->GetFirst(), 1) << "Первый элемент списка не соответствует первому элементу массива";
    EXPECT_EQ(linked_int->GetLast(), 5) << "Последний элемент списка не соответствует последнему элементу массива";
}

TEST_F(LinkedListFixture, copy_constructor) {
    LinkedList<int> copy(*linked_int);
    EXPECT_EQ(copy.GetLength(), 5) << "Длина списка-копии должна совпадать с длиной оригинала";
    EXPECT_EQ(copy.Get(2), 3) << "Данные в узлах копии списка не совпадают с оригиналом";
}

TEST_F(LinkedListFixture, move_constructor) {
    LinkedList<int> source(list_data_int, 3);
    LinkedList<int> moved(std::move(source));
    EXPECT_EQ(moved.GetLength(), 3) << "Перемещенный список должен содержать элементы исходного объекта";
    EXPECT_EQ(moved.GetFirst(), 1) << "Данные перемещенного списка повреждены";
    EXPECT_EQ(source.GetLength(), 0) << "После перемещения (move-конструктор) старый список должен быть очищен";
}

TEST_F(LinkedListFixture, copy_assignment) {
    LinkedList<int> other;
    other = *linked_int;
    EXPECT_EQ(other.GetLength(), 5) << "Оператор присваивания скопировал неверное количество элементов";
    EXPECT_EQ(other.GetLast(), 5) << "Элементы в хвосте списка после копирования не совпадают";
}

TEST_F(LinkedListFixture, move_assignment) {
    LinkedList<int> other;
    LinkedList<int> source(list_data_int, 2);
    other = std::move(source);
    EXPECT_EQ(other.GetLength(), 2) << "Оператор перемещающего присваивания не забрал элементы из источника";
}

TEST_F(LinkedListFixture, getters) {
    EXPECT_EQ(linked_int->GetFirst(), 1) << "GetFirst() вернул ошибочное значение";
    EXPECT_EQ(linked_int->GetLast(), 5) << "GetLast() вернул ошибочное значение";
    EXPECT_EQ(linked_int->Get(3), 4) << "Get(3) вернул неверный элемент из середины списка";
    EXPECT_THROW(linked_int->Get(10), std::out_of_range) << "Запрос элемента за пределами списка должен вызывать std::out_of_range";
}

TEST_F(LinkedListFixture, append_and_prepend) {
    linked_int->Append(10);
    EXPECT_EQ(linked_int->GetLast(), 10) << "Append() должен добавлять элемент строго в хвост списка";
    EXPECT_EQ(linked_int->GetLength(), 6) << "После добавления элемента в конец длина должна увеличиться на 1";

    linked_int->Prepend(-10);
    EXPECT_EQ(linked_int->GetFirst(), -10) << "Prepend() должен добавлять элемент строго в голову списка";
    EXPECT_EQ(linked_int->GetLength(), 7) << "После добавления элемента в начало длина должна увеличиться на 1";
}

TEST_F(LinkedListFixture, insert_at) {
    linked_int->InsertAt(99, 2);
    EXPECT_EQ(linked_int->Get(2), 99) << "Элемент не был успешно вставлен на позицию 2";
    EXPECT_EQ(linked_int->GetLength(), 6) << "После вставки элемента в середину длина должна увеличиться на 1";
    
    linked_int->InsertAt(11, 0);
    EXPECT_EQ(linked_int->GetFirst(), 11) << "Вставка по индексу 0 должна работать аналогично Prepend";

    linked_int->InsertAt(22, linked_int->GetLength());
    EXPECT_EQ(linked_int->GetLast(), 22) << "Вставка по индексу, равному длине списка, должна работать аналогично Append";

    EXPECT_THROW(linked_int->InsertAt(0, 100), std::out_of_range) << "Вставка по индексу за пределами списка должна кидать std::out_of_range";
}

TEST_F(LinkedListFixture, get_sub_list) {
    LinkedList<int> sub = linked_int->GetSubList(1, 3);
    EXPECT_EQ(sub.GetLength(), 3) << "Длина подсписка из диапазона [1, 3] должна составлять 3 элемента";
    EXPECT_EQ(sub.GetFirst(), 2) << "Первый элемент подсписка не совпадает со значением оригинального списка на индексе 1";
    EXPECT_EQ(sub.GetLast(), 4) << "Последний элемент подсписка не совпадает со значением оригинального списка на индексе 3";
    EXPECT_THROW(linked_int->GetSubList(3, 1), std::out_of_range) << "Перевернутый диапазон индексов (startIndex > endIndex) должен генерировать исключение";
}

TEST_F(LinkedListFixture, concat) {
    LinkedList<int> other;
    other.Append(100);
    LinkedList<int> result = linked_int->Concat(other);
    EXPECT_EQ(result.GetLength(), 6) << "Длина результирующего списка после Concat не равна сумме длин исходных";
    EXPECT_EQ(result.GetLast(), 100) << "Хвостом объединенного списка должен быть последний элемент присоединенного списка";
}