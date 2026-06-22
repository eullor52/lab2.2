#include "fixtures.hpp"

TEST_F(ListSequenceFixture, constructors) {
    ListSequence<int> empty;
    EXPECT_EQ(empty.GetLength(), 0) << "Дефолтный конструктор ListSequence должен создавать пустую структуру";
    LinkedList<int> list;
    list.Append(10);
    ListSequence<int> from_list(list);
    EXPECT_EQ(from_list.GetLength(), 1) << "Последовательность из LinkedList инициализирована с ошибочным размером";
    ListSequence<int> copy(*list_seq_int);
    EXPECT_EQ(copy.GetLength(), 6) << "Конструктор копирования ListSequence скопировал не все элементы";
}

TEST_F(ListSequenceFixture, getters) {
    ASSERT_NE(list_seq_int, nullptr) << "Фикстура list_seq_int равна nullptr";
    EXPECT_EQ(list_seq_int->GetFirst(), 10) << "GetFirst() вернул неверную голову последовательности";
    EXPECT_EQ(list_seq_int->GetLast(), 60) << "GetLast() вернул неверный хвост последовательности";
    EXPECT_EQ(list_seq_int->Get(3), 40) << "Get(3) извлек ошибочное значение из связного списка";
    EXPECT_THROW(list_seq_int->Get(100), std::out_of_range) << "Выход за диапазон индексов должен бросать std::out_of_range";
}

TEST_F(ListSequenceFixture, append_prepend_insert) {
    int x = 99;
    auto* s1 = list_seq_int->Append(x); 
    ASSERT_NE(s1, nullptr) << "Append lvalue вернул nullptr";
    EXPECT_EQ(s1->GetLast(), 99) << "Элемент lvalue не добавился в конец ListSequence";
    auto* s2 = s1->Prepend(11); 
    ASSERT_NE(s2, nullptr) << "Prepend rvalue вернул nullptr";
    EXPECT_EQ(s2->GetFirst(), 11) << "Элемент rvalue не добавился в начало ListSequence";
    auto* s3 = s2->InsertAt(55, 2);
    ASSERT_NE(s3, nullptr) << "InsertAt вернул nullptr";
    EXPECT_EQ(s3->Get(2), 55) << "Вставка элемента по заданному индексу не сработала";
    delete s1;
    delete s2;
    delete s3;
}

TEST_F(ListSequenceFixture, subsequence_and_concat) {
    auto* sub = list_seq_int->GetSubsequence(2, 4);
    ASSERT_NE(sub, nullptr) << "GetSubsequence вернул nullptr";
    EXPECT_EQ(sub->GetLength(), 3) << "Длина сформированной подпоследовательности неверна";
    EXPECT_EQ(sub->GetFirst(), 30) << "Первый элемент подпоследовательности не совпадает со значением из диапазона";
    ListSequence<int> other;
    auto* s_other = other.Append(999);
    auto* conc = list_seq_int->Concat(*s_other);
    ASSERT_NE(conc, nullptr) << "Метод Concat вернул nullptr";
    EXPECT_EQ(conc->GetLast(), 999) << "Конкатенация последовательностей завершилась неудачно (ошибка в хвосте)";
    delete sub;
    delete s_other;
    delete conc;
}

TEST_F(ListSequenceFixture, mutable_vs_immutable_behavior) {
    ImmutableListSequence<int> immut;
    auto* immut_res = immut.Append(10);
    ASSERT_NE(immut_res, nullptr) << "Append на ImmutableListSequence вернул nullptr";
    EXPECT_NE(&immut, immut_res) << "Immutable версия обязана возвращать новый экземпляр при вызове мутирующих методов";
    EXPECT_EQ(immut.GetLength(), 0) << "Исходная неизменяемая последовательность поменяла свой размер";
    MutableListSequence<int> mut;
    auto* mut_res = mut.Append(10);
    ASSERT_NE(mut_res, nullptr) << "Append на MutableListSequence вернул nullptr";
    EXPECT_EQ(&mut, mut_res) << "Mutable версия обязана модифицировать себя на месте и возвращать свой адрес";
    EXPECT_EQ(mut.GetLength(), 1) << "Размер изменяемой последовательности не увеличился";

    delete immut_res;
}

TEST_F(ListSequenceFixture, high_order_functions_map_reduce) {
    ListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3); 
    ListSequence<int>* mapped = Map(&seq, [](int x) { return x + 10; });
    ASSERT_NE(mapped, nullptr) << "Внешний метод Map для ListSequence вернул nullptr";
    ASSERT_EQ(mapped->GetLength(), 3) << "Map сформировал результирующую структуру неверного размера";
    EXPECT_EQ(mapped->Get(0), 11) << "Ошибка трансформации элемента на индексе 0";
    EXPECT_EQ(mapped->Get(2), 13) << "Ошибка трансформации элемента на индексе 2";
    int reduced = Reduce(&seq, [](int acc, int x) { return acc * x; }, 1);
    EXPECT_EQ(reduced, 6) << "Вычисление свертки (Reduce) для ListSequence привело к неверному значению";
    ArraySequence<int>* whered = Where(&seq, [](int x) { return x > 2; });
    EXPECT_EQ(whered->GetLength(), 1) << "Функция Where вернула последовательность неверной длины";
    EXPECT_EQ(whered->GetFirst(), 3) << "Функция Where преобразовала последовательность неверно";
    delete mapped;
}