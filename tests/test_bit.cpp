#include "fixtures.hpp"
#include <gtest/gtest.h>
#include "../headers/bit.hpp"

TEST_F(BitFixture, default_constructor) {
    Bit<uint8_t> b;
    EXPECT_EQ(b.GetValue(), 0);
    EXPECT_FALSE(static_cast<bool>(b));
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор по умолчанию.\n";
        std::cerr << "Ожидаемое значение: 0, получено: " << (int)b.GetValue() << "\n";
        std::cerr << "Ожидаемый operator bool: false, получено: " << static_cast<bool>(b) << "\n\n";
    }
}

TEST_F(BitFixture, value_constructor) {
    Bit<uint8_t> b(0xAA);
    EXPECT_EQ(b.GetValue(), 0xAA);
    EXPECT_TRUE(static_cast<bool>(b));
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор со значением.\n";
        std::cerr << "Ожидаемое значение: 170 (0xAA), получено: " << (int)b.GetValue() << "\n";
        std::cerr << "Ожидаемый operator bool: true, получено: " << static_cast<bool>(b) << "\n\n";
    }
}

TEST_F(BitFixture, copy_constructor) {
    Bit<uint8_t> copy(*bit_u8);
    EXPECT_EQ(copy.GetValue(), bit_u8->GetValue());
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем конструктор копирования.\n";
        std::cerr << "Ожидаемое значение: " << (int)bit_u8->GetValue()
                  << ", получено: " << (int)copy.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, get_bit_count) {
    EXPECT_EQ(bit_u8->GetBitCount(), 8);
    EXPECT_EQ(bit_u16->GetBitCount(), 16);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetBitCount.\n";
        std::cerr << "Для uint8_t ожидалось 8, получено " << bit_u8->GetBitCount() << "\n";
        std::cerr << "Для uint16_t ожидалось 16, получено " << bit_u16->GetBitCount() << "\n\n";
    }
}

TEST_F(BitFixture, get_value_set_value) {
    Bit<uint8_t> b;
    b.SetValue(0x55);
    EXPECT_EQ(b.GetValue(), 0x55);
    b.SetValue(0);
    EXPECT_EQ(b.GetValue(), 0);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем GetValue/SetValue.\n";
        std::cerr << "После SetValue(0x55) получено: " << (int)b.GetValue() << "\n";
        std::cerr << "После SetValue(0) получено: " << (int)b.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, operator_brackets_const) {
    Bit<uint8_t> b(0b10101010);
    EXPECT_TRUE(b[7]);
    EXPECT_FALSE(b[6]);
    EXPECT_TRUE(b[5]);
    EXPECT_FALSE(b[4]);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем константный operator[].\n";
        std::cerr << "Ожидалось: бит 7 = 1, бит 6 = 0, бит 5 = 1, бит 4 = 0\n";
        std::cerr << "Получено: бит 7 = " << b[7] << ", бит 6 = " << b[6]
                  << ", бит 5 = " << b[5] << ", бит 4 = " << b[4] << "\n\n";
    }
}

TEST_F(BitFixture, operator_brackets_proxy_set) {
    Bit<uint8_t> b(0);
    b[0] = true;
    EXPECT_TRUE(b[0]);
    EXPECT_EQ(b.GetValue(), 1);
    b[1] = true;
    EXPECT_TRUE(b[1]);
    EXPECT_EQ(b.GetValue(), 3);
    b[0] = false;
    EXPECT_FALSE(b[0]);
    EXPECT_EQ(b.GetValue(), 2);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем установку/сброс битов через прокси.\n";
        std::cerr << "После b[0]=true, b[1]=true, b[0]=false значение = " << (int)b.GetValue()
                  << ", ожидалось 2\n\n";
    }
}

TEST_F(BitFixture, proxy_assignment) {
    Bit<uint8_t> b(0b10100000);
    auto proxy1 = b[5];
    auto proxy2 = b[3];
    proxy1 = proxy2;
    EXPECT_EQ(static_cast<bool>(b[5]), static_cast<bool>(b[3]));
    EXPECT_EQ(b.GetValue(), 0b10000000);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем присваивание BitProxy друг другу.\n";
        std::cerr << "Ожидаемое значение после proxy1 = proxy2: 128 (0b10000000), получено: "
                  << (int)b.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, proxy_out_of_range) {
    Bit<uint8_t> b;
    EXPECT_THROW(b[8], OutOfRangeException);
    EXPECT_THROW(b[100], OutOfRangeException);
}

TEST_F(BitFixture, operator_bool) {
    Bit<uint8_t> zero(0);
    Bit<uint8_t> nonzero(1);
    EXPECT_FALSE(static_cast<bool>(zero));
    EXPECT_TRUE(static_cast<bool>(nonzero));
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем operator bool.\n";
        std::cerr << "Для нулевого значения ожидалось false, получено " << static_cast<bool>(zero) << "\n";
        std::cerr << "Для ненулевого значения ожидалось true, получено " << static_cast<bool>(nonzero) << "\n\n";
    }
}

TEST_F(BitFixture, assignment_operator) {
    Bit<uint8_t> b;
    b = *bit_u8;
    EXPECT_EQ(b.GetValue(), bit_u8->GetValue());
    Bit<uint8_t> self(5);
    self = self;
    EXPECT_EQ(self.GetValue(), 5);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем оператор присваивания.\n";
        std::cerr << "После b = *bit_u8 ожидалось " << (int)bit_u8->GetValue()
                  << ", получено " << (int)b.GetValue() << "\n";
        std::cerr << "После самоприсваивания self = self ожидалось 5, получено " << (int)self.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, bitwise_and) {
    Bit<uint8_t> a(0b1100);
    Bit<uint8_t> b(0b1010);
    Bit<uint8_t> res = a & b;
    EXPECT_EQ(res.GetValue(), 0b1000);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем побитовое И (&).\n";
        std::cerr << "Ожидалось 8 (0b1000), получено " << (int)res.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, bitwise_or) {
    Bit<uint8_t> a(0b1100);
    Bit<uint8_t> b(0b1010);
    Bit<uint8_t> res = a | b;
    EXPECT_EQ(res.GetValue(), 0b1110);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем побитовое ИЛИ (|).\n";
        std::cerr << "Ожидалось 14 (0b1110), получено " << (int)res.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, bitwise_xor) {
    Bit<uint8_t> a(0b1100);
    Bit<uint8_t> b(0b1010);
    Bit<uint8_t> res = a ^ b;
    EXPECT_EQ(res.GetValue(), 0b0110);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем побитовое XOR (^).\n";
        std::cerr << "Ожидалось 6 (0b0110), получено " << (int)res.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, bitwise_not) {
    Bit<uint8_t> a(0b00001111);
    Bit<uint8_t> res = ~a;
    EXPECT_EQ(res.GetValue(), 0b11110000);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем побитовое NOT (~).\n";
        std::cerr << "Ожидалось 240 (0b11110000), получено " << (int)res.GetValue() << "\n\n";
    }
}

TEST_F(BitFixture, comparison_equal) {
    Bit<uint8_t> a(5);
    Bit<uint8_t> b(5);
    Bit<uint8_t> c(10);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем оператор ==.\n";
        std::cerr << "a == b ожидалось true, получено " << (a == b) << "\n";
        std::cerr << "a == c ожидалось false, получено " << (a == c) << "\n\n";
    }
}

TEST_F(BitFixture, comparison_not_equal) {
    Bit<uint8_t> a(5);
    Bit<uint8_t> b(5);
    Bit<uint8_t> c(10);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
    if (testing::Test::HasFailure()) {
        std::cerr << "\nТестируем оператор !=.\n";
        std::cerr << "a != b ожидалось false, получено " << (a != b) << "\n";
        std::cerr << "a != c ожидалось true, получено " << (a != c) << "\n\n";
    }
}