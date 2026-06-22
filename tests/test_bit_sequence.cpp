#include "fixtures.hpp"

TEST_F(BitSequenceFixture, constructors_and_getters) {
    BitSequence<uint8_t> empty;
    EXPECT_EQ(empty.GetLength(), 0) << "Инициализированная пустая битовая последовательность должна иметь длину 0";
    EXPECT_THROW(empty.GetFirst(), std::out_of_range) << "Вызов GetFirst() на пустой битовой последовательности должен генерировать std::out_of_range";
    EXPECT_THROW(empty.GetLast(), std::out_of_range) << "Вызов GetLast() на пустой битовой последовательности должен генерировать std::out_of_range";
    Bit<uint8_t> bits_arr[3] = { Bit<uint8_t>(true), Bit<uint8_t>(false), Bit<uint8_t>(true) };
    BitSequence<uint8_t> seq_from_bits(bits_arr, 3);
    EXPECT_EQ(seq_from_bits.GetLength(), 3) << "Конструктор по массиву Bit установил неверную длину последовательности";
    EXPECT_TRUE(static_cast<bool>(seq_from_bits.Get(0))) << "Значение бита по индексу 0 искажено при инициализации";
    EXPECT_FALSE(static_cast<bool>(seq_from_bits.Get(1))) << "Значение бита по индексу 1 искажено при инициализации";
    BitSequence<uint8_t> copy(seq_from_bits);
    EXPECT_EQ(copy.GetLength(), 3) << "Длина скопированной битовой последовательности не совпадает с оригинальной";
}

TEST_F(BitSequenceFixture, modification_operations) {
    BitSequence<uint8_t> bs;
    auto* s1 = bs.Append(Bit<uint8_t>(true));
    ASSERT_NE(s1, nullptr) << "Метод Append вернул nullptr";
    EXPECT_EQ(s1->GetLength(), 1) << "После добавления бита длина последовательности не увеличилась";
    EXPECT_TRUE(static_cast<bool>(s1->GetLast())) << "Добавленный в конец бит имеет неверное логическое значение";
    auto* s2 = s1->Prepend(Bit<uint8_t>(false));
    ASSERT_NE(s2, nullptr) << "Метод Prepend вернул nullptr";
    EXPECT_EQ(s2->GetLength(), 2) << "После добавления бита в начало общая длина структуры не увеличилась";
    EXPECT_FALSE(static_cast<bool>(s2->GetFirst())) << "Добавленный в начало бит имеет ошибочное значение";
    auto* s3 = s2->InsertAt(Bit<uint8_t>(true), 1);
    ASSERT_NE(s3, nullptr) << "Метод InsertAt вернул nullptr";
    EXPECT_EQ(s3->GetLength(), 3) << "Длина битовой структуры после вставки некорректна";
    EXPECT_TRUE(static_cast<bool>(s3->Get(1))) << "Вставленный бит не обнаруживается по целевому индексу 1";
    BitSequence<uint8_t> other;
    other.Append(Bit<uint8_t>(false));
    auto* s4 = s3->Concat(other);
    ASSERT_NE(s4, nullptr) << "Метод Concat для битовых последовательностей вернул nullptr";
    EXPECT_EQ(s4->GetLength(), 4) << "Итоговая длина после слияния последовательностей вычислена неверно";
    EXPECT_FALSE(static_cast<bool>(s4->GetLast())) << "Элементы присоединенной битовой структуры повреждены";
    delete s1;
    delete s2;
    delete s3;
    delete s4;
}

TEST_F(BitSequenceFixture, logic_operators_and_exceptions) {
    BitSequence<uint8_t> a;
    a.Append(Bit<uint8_t>(true))->Append(Bit<uint8_t>(false));
    BitSequence<uint8_t> b;
    b.Append(Bit<uint8_t>(false))->Append(Bit<uint8_t>(false));
    auto res_and = a & b;
    EXPECT_FALSE(static_cast<bool>(res_and.Get(0))) << "Операция побитового И (&) над последовательностями дала неверный результат";
    auto res_or = a | b;
    EXPECT_TRUE(static_cast<bool>(res_or.Get(0))) << "Операция побитового ИЛИ (|) над последовательностями дала неверный результат";
    auto res_xor = a ^ b;
    EXPECT_TRUE(static_cast<bool>(res_xor.Get(0))) << "Операция побитового ИСКЛЮЧАЮЩЕЕ ИЛИ (^) над последовательностями дала неверный результат";
    auto res_not = ~a;
    EXPECT_FALSE(static_cast<bool>(res_not.Get(0))) << "Инверсия бита (~) на позиции 0 отработала некорректно";
    EXPECT_TRUE(static_cast<bool>(res_not.Get(1))) << "Инверсия бита (~) на позиции 1 отработала некорректно";
    b.Append(Bit<uint8_t>(true));
    EXPECT_THROW(a & b, std::length_error) << "Разная длина операндов в операторе & должна возбуждать std::length_error";
    EXPECT_THROW(a | b, std::length_error) << "Разная длина операндов в операторе | должна возбуждать std::length_error";
    EXPECT_THROW(a ^ b, std::length_error) << "Разная длина операндов в операторе ^ должна возбуждать std::length_error";
}

TEST_F(BitSequenceFixture, subsequence) {
    BitSequence<uint8_t> seq;
    seq.Append(Bit<uint8_t>(true))->Append(Bit<uint8_t>(false))->Append(Bit<uint8_t>(true));
    auto* sub = seq.GetSubsequence(1, 2);
    ASSERT_NE(sub, nullptr) << "Метод GetSubsequence вернул nullptr";
    ASSERT_EQ(sub->GetLength(), 2) << "Размер выделенной битовой подпоследовательности неверен";
    EXPECT_FALSE(static_cast<bool>(sub->Get(0))) << "Данные внутри выделенной подпоследовательности смещены или искажены";
    delete sub;
}

TEST_F(BitSequenceFixture, mutable_vs_immutable_bit_sequence) {
    ImmutableBitSequence<uint8_t> immut;
    auto* immut_res = immut.Append(Bit<uint8_t>(true));
    ASSERT_NE(immut_res, nullptr) << "Append на ImmutableBitSequence вернул nullptr";
    EXPECT_NE(&immut, immut_res) << "Неизменяемая битовая последовательность должна возвращать указатель на новый экземпляр";
    EXPECT_EQ(immut.GetLength(), 0) << "Размер исходного ImmutableBitSequence объекта изменился вопреки логике";
    MutableBitSequence<uint8_t> mut;
    auto* mut_res = mut.Append(Bit<uint8_t>(true));
    ASSERT_NE(mut_res, nullptr) << "Append на MutableBitSequence вернул nullptr";
    EXPECT_EQ(&mut, mut_res) << "Изменяемая битовая последовательность обязана возвращать указатель на саму себя (this)";
    EXPECT_EQ(mut.GetLength(), 1) << "Размер MutableBitSequence не обновился на месте";
    delete immut_res;
}