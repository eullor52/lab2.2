#include "fixtures.hpp"

TEST_F(BitFixture, constructors_and_getters) {
    Bit<uint8_t> empty;
    EXPECT_EQ(empty.GetValue(), 0) << "Дефолтный конструктор Bit должен инициализировать поле нулевым значением";
    EXPECT_FALSE(static_cast<bool>(empty)) << "Приведение пустого Bit к типу bool должно возвращать false";
    ASSERT_NE(bit_u8, nullptr) << "Фикстура bit_u8 равна nullptr";
    EXPECT_EQ(bit_u8->GetValue(), 0b10110010) << "Конструктор со значением записал неверный набор бит";
    EXPECT_TRUE(static_cast<bool>(*bit_u8)) << "Приведение ненулевого Bit к bool должно возвращать true";
    Bit<uint8_t> copy(*bit_u8);
    EXPECT_EQ(copy.GetValue(), 0b10110010) << "Конструктор копирования скопировал неверное битовое поле";
}

TEST_F(BitFixture, bit_count) {
    EXPECT_EQ(bit_u8->GetBitCount(), 8) << "Количество бит в типе uint8_t должно быть равно строго 8";
    EXPECT_EQ(bit_u16->GetBitCount(), 16) << "Количество бит в типе uint16_t должно быть равно строго 16";
}

TEST_F(BitFixture, set_value) {
    Bit<uint8_t> b;
    b.SetValue(0xAA);
    EXPECT_EQ(b.GetValue(), 0xAA) << "Метод SetValue записал некорректное значение внутрь контейнера";
}

TEST_F(BitFixture, operator_brackets_and_proxy) {
    const Bit<uint8_t> const_bit(0b00000010);
    EXPECT_FALSE(const_bit[0]) << "Константный оператор [] вернул true для нулевого бита";
    EXPECT_TRUE(const_bit[1]) << "Константный оператор [] вернул false для единичного бита";
    (*bit_u8)[0] = true;
    EXPECT_TRUE((*bit_u8)[0]) << "BitProxy не смог установить 0-й бит в состояние true";
    EXPECT_EQ(bit_u8->GetValue(), 0b10110011) << "Общее значение битовой маски изменилось неверно после записи через прокси";
    (*bit_u8)[1] = false;
    EXPECT_FALSE((*bit_u8)[1]) << "BitProxy не смог сбросить 1-й бит в состояние false";
    EXPECT_EQ(bit_u8->GetValue(), 0b10110001) << "Общее значение битовой маски изменилось неверно после сброса бита";
    auto proxy1 = (*bit_u8)[4];
    auto proxy2 = (*bit_u8)[5];
    proxy1 = proxy2; 
    EXPECT_EQ(static_cast<bool>(proxy1), static_cast<bool>(proxy2)) << "Присваивание одного BitProxy другому сработало некорректно";
    EXPECT_THROW((*bit_u8)[100], std::out_of_range) << "Запрос индекса бита за пределами размера базового типа должен вызывать std::out_of_range";
}

TEST_F(BitFixture, assignment_operators) {
    Bit<uint8_t> b;
    b = *bit_u8;
    EXPECT_EQ(b.GetValue(), bit_u8->GetValue()) << "Оператор присваивания скопировал неверное битовое значение";
}

TEST_F(BitFixture, bitwise_operations) {
    Bit<uint8_t> a(0b1100);
    Bit<uint8_t> b(0b1010);
    EXPECT_EQ((a & b).GetValue(), 0b1000) << "Побитовый оператор И (&) вернул неверный результат";
    EXPECT_EQ((a | b).GetValue(), 0b1110) << "Побитовый оператор ИЛИ (|) вернул неверный результат";
    EXPECT_EQ((a ^ b).GetValue(), 0b0110) << "Побитовый оператор ИСКЛЮЧАЮЩЕЕ ИЛИ (^) вернул неверный результат";
    Bit<uint8_t> n(0b00001111);
    EXPECT_EQ((~n).GetValue(), 0b11110000) << "Побитовое отрицание (~) выполнило инверсию некорректно";
}

TEST_F(BitFixture, comparison_operators) {
    Bit<uint8_t> a(5);
    Bit<uint8_t> b(5);
    Bit<uint8_t> c(10);
    EXPECT_TRUE(a == b) << "Оператор проверки равенства (==) вернул false для одинаковых объектов";
    EXPECT_FALSE(a != b) << "Оператор проверки неравенства (!=) вернул true для одинаковых объектов";
    EXPECT_TRUE(a != c) << "Оператор проверки неравенства (!=) вернул false для отличающихся объектов";
    EXPECT_FALSE(a == c) << "Оператор проверки равенства (==) вернул true для отличающихся объектов";
}