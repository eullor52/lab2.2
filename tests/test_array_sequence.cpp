#include "fixtures.hpp"

TEST_F(ArraySequenceFixture, constructors) {
    ArraySequence<int> empty;
    EXPECT_EQ(empty.GetLength(), 0) << "Дефолтный конструктор ArraySequence должен создавать пустую последовательность";
    LinkedList<int> list(arr_data, 4);
    ArraySequence<int> from_list(list);
    EXPECT_EQ(from_list.GetLength(), 4) << "Конструктор на основе LinkedList создал последовательность неверной длины";
    EXPECT_EQ(from_list.GetFirst(), 132) << "Элементы последовательности, созданной из LinkedList, искажены";
    ArraySequence<int> copy(*seq_int);
    EXPECT_EQ(copy.GetLength(), 4) << "Конструктор копирования создал последовательность неверного размера";
}

TEST_F(ArraySequenceFixture, getters) {
    ASSERT_NE(seq_int, nullptr) << "Фикстура seq_int не проинициализирована";
    EXPECT_EQ(seq_int->GetFirst(), 132) << "GetFirst() вернул некорректный элемент";
    EXPECT_EQ(seq_int->GetLast(), 98) << "GetLast() вернул некорректный элемент";
    EXPECT_EQ(seq_int->Get(1), 23) << "Get(1) вернул неверный элемент из середины структуры";
    EXPECT_THROW(seq_int->Get(10), std::out_of_range) << "Запрос элемента по индексу вне границ должен выбрасывать std::out_of_range";
}

TEST_F(ArraySequenceFixture, append_lvalue_rvalue) {
    int val = 500;
    auto* res1 = seq_int->Append(val); 
    ASSERT_NE(res1, nullptr) << "Метод Append(const T&) вернул nullptr";
    EXPECT_EQ(res1->GetLast(), 500) << "Элемент lvalue не добавился в конец последовательности";
    auto* res2 = res1->Append(600); 
    ASSERT_NE(res2, nullptr) << "Метод Append(T&&) вернул nullptr";
    EXPECT_EQ(res2->GetLast(), 600) << "Элемент rvalue не добавился в конец последовательности";
    delete res1;
    delete res2;
}

TEST_F(ArraySequenceFixture, prepend_lvalue_rvalue) {
    int val = 500;
    auto* res1 = seq_int->Prepend(val); 
    ASSERT_NE(res1, nullptr) << "Метод Prepend(const T&) вернул nullptr";
    EXPECT_EQ(res1->GetFirst(), 500) << "Элемент lvalue не добавился в начало последовательности";
    auto* res2 = res1->Prepend(600); 
    ASSERT_NE(res2, nullptr) << "Метод Prepend(T&&) вернул nullptr";
    EXPECT_EQ(res2->GetFirst(), 600) << "Элемент rvalue не добавился в начало последовательности";
    delete res1;
    delete res2;
}

TEST_F(ArraySequenceFixture, insert_at_lvalue_rvalue) {
    int val = 777;
    auto* res1 = seq_int->InsertAt(val, 2); 
    ASSERT_NE(res1, nullptr) << "Метод InsertAt(const T&, idx) вернул nullptr";
    EXPECT_EQ(res1->Get(2), 777) << "Элемент lvalue не был вставлен на указанную позицию";
    auto* res2 = res1->InsertAt(888, 0); 
    ASSERT_NE(res2, nullptr) << "Метод InsertAt(T&&, 0) вернул nullptr";
    EXPECT_EQ(res2->GetFirst(), 888) << "Элемент rvalue не был вставлен в начало структуры";
    EXPECT_THROW(seq_int->InsertAt(1, 20), std::out_of_range) << "Вставка за границы последовательности должна вызывать std::out_of_range";
    delete res1;
    delete res2;
}

TEST_F(ArraySequenceFixture, subsequence_and_concat) {
    auto* sub = seq_int->GetSubsequence(1, 2);
    ASSERT_NE(sub, nullptr) << "GetSubsequence вернул nullptr";
    EXPECT_EQ(sub->GetLength(), 2) << "Неверная длина выделенной подпоследовательности";
    EXPECT_EQ(sub->GetFirst(), 23) << "Содержимое подпоследовательности не соответствует ожиданиям";
    ArraySequence<int> other(arr_data, 2);
    auto* concat_res = seq_int->Concat(other);
    ASSERT_NE(concat_res, nullptr) << "Метод Concat вернул nullptr";
    EXPECT_EQ(concat_res->GetLength(), 6) << "Длина объединенной последовательности не равна сумме исходных длин";
    delete sub;
    delete concat_res;
}

TEST_F(ArraySequenceFixture, mutable_vs_immutable_behavior) {
    ImmutableArraySequence<int> immut(arr_data, 4);
    auto* immut_res = immut.Append(999);
    ASSERT_NE(immut_res, nullptr) << "Append на Immutable объекте вернул nullptr";
    EXPECT_NE(&immut, immut_res) << "Immutable объект при мутации должен возвращать указатель на совершенно новый экземпляр";
    EXPECT_EQ(immut.GetLength(), 4) << "Исходное состояние Immutable объекта должно оставаться неизменным";
    EXPECT_EQ(immut_res->GetLast(), 999) << "Новый экземпляр Immutable последовательности не содержит добавленный элемент";
    MutableArraySequence<int> mut(arr_data, 4);
    auto* mut_res = mut.Append(999);
    ASSERT_NE(mut_res, nullptr) << "Append на Mutable объекте вернул nullptr";
    EXPECT_EQ(&mut, mut_res) << "Mutable объект должен возвращать указатель на самого себя (this)";
    EXPECT_EQ(mut.GetLength(), 5) << "Размер Mutable объекта должен увеличиваться непосредственно на месте";
    EXPECT_EQ(mut.GetLast(), 999) << "Элемент не обновился в Mutable структуре";
    delete immut_res;
}

TEST_F(ArraySequenceFixture, high_order_functions_map_reduce) {
    ArraySequence<int> seq(arr_data, 3);
    ArraySequence<int>* mapped = Map(&seq, [](int x) { return x * 2; });
    ASSERT_NE(mapped, nullptr) << "Функция Map вернула nullptr";
    ASSERT_EQ(mapped->GetLength(), 3) << "Функция Map вернула последовательность неверной длины";
    EXPECT_EQ(mapped->Get(0), 264) << "Элемент на позиции 0 некорректно преобразован функцией Map";
    EXPECT_EQ(mapped->Get(1), 46) << "Элемент на позиции 1 некорректно преобразован функцией Map";
    EXPECT_EQ(mapped->Get(2), -112) << "Элемент на позиции 2 некорректно преобразован функцией Map";
    int reduced = Reduce(&seq, [](int acc, int x) { return acc + x; }, 0);
    EXPECT_EQ(reduced, 132 + 23 - 56) << "Функция Reduce посчитала свертку последовательности неверно";
    ArraySequence<int>* whered = Where(&seq, [](int x) { return x > 100; });
    EXPECT_EQ(whered->GetLength(), 1) << "Функция Where вернула последовательность неверной длины";
    EXPECT_EQ(whered->GetFirst(), 132) << "Функция Where преобразовала последовательность неверно";
    delete mapped;
}