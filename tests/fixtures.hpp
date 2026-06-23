#pragma once

#include <gtest/gtest.h>
#include "../headers/array.hpp"
#include "../headers/list.hpp"
#include "../headers/arraysequence.hpp"
#include "../headers/listsequence.hpp"
#include "../headers/bit.hpp"
#include "../headers/bitsequence.hpp"
#include <string>

class DynamicArrayFixture : public testing::Test {
protected:
    int data_int[6] = {112, -2, 322, 23, -32};
    const char* data_str[6] = {"petrichor", "Afterglow", "Dragonfly", "Cardigan", "Agile", "Bombinate"};
    DynamicArray<int>* array_int;
    DynamicArray<const char*>* array_str;
    void SetUp() override {
        array_int = new DynamicArray<int>(data_int, 6);
        array_str = new DynamicArray<const char*>(data_str, 6);
    }
    void TearDown() override {
        delete array_int;
        delete array_str;
    }
};

class LinkedListFixture : public testing::Test {
protected:
    int list_data_int[5] = {1, 2, 3, 4, 5};
    const char* list_data_str[3] = {"One", "Two", "Three"};
    LinkedList<int>* linked_int;
    LinkedList<const char*>* linked_str;
    void SetUp() override {
        linked_int = new LinkedList<int>(list_data_int, 5);
        linked_str = new LinkedList<const char*>(list_data_str, 3);
    }
    void TearDown() override {
        delete linked_int;
        delete linked_str;
    }
};

class ListSequenceFixture : public testing::Test {
protected:
    int list_data_int[6] = {10, 2023, 23, 4340, 50, 60};
    const char* list_data_str[4] = {"Alpha", "Beta", "Gamma", "Delta"};
    LinkedList<int>* linked_int;
    LinkedList<const char*>* linked_str;
    MutableListSequence<int>* list_seq_int;
    MutableListSequence<const char*>* list_seq_str;
    void SetUp() override {
        linked_int = new LinkedList<int>(list_data_int, 6);
        linked_str = new LinkedList<const char*>(list_data_str, 4);
        list_seq_int = new MutableListSequence<int>(*linked_int);
        list_seq_str = new MutableListSequence<const char*>(*linked_str);
    }
    void TearDown() override {
        delete linked_int;
        delete linked_str;
        delete list_seq_int;
        delete list_seq_str;
    }
};

class ArraySequenceFixture : public testing::Test {
protected:
    int arr_data[4] = {132, 23, -56, 98};
    const char* arr_str[2] = {"Hello", "World"};
    ImmutableArraySequence<int>* immut_seq_int;
    ImmutableArraySequence<const char*>* immut_seq_str;
    MutableArraySequence<int>* mut_seq_int;
    MutableArraySequence<const char*>* mut_seq_str;
    void SetUp() override {
        mut_seq_str = new MutableArraySequence<const char*>(arr_str, 2);
        immut_seq_str = new ImmutableArraySequence<const char*>(arr_str, 2);
        immut_seq_int = new ImmutableArraySequence<int>(arr_data, 4);
        mut_seq_int = new MutableArraySequence<int>(arr_data, 4);
    }
    void TearDown() override {
        delete mut_seq_int;
        delete mut_seq_str;
        delete immut_seq_int;
        delete immut_seq_str;
    }
};

class BitFixture : public testing::Test {
protected:
    uint8_t raw_u8 = 0b10110010;
    uint16_t raw_u16 = 0b1111000011110000;
    Bit<uint8_t>* bit_u8;
    Bit<uint16_t>* bit_u16;
    void SetUp() override {
        bit_u8 = new Bit<uint8_t>(raw_u8);
        bit_u16 = new Bit<uint16_t>(raw_u16);
    }
    void TearDown() override {
        delete bit_u8;
        delete bit_u16;
    }
};

class MutableBitSeqFixture : public testing::Test {
protected:
    Bit<uint8_t> bits[5] = {
        Bit<uint8_t>(1), Bit<uint8_t>(0), Bit<uint8_t>(1),
        Bit<uint8_t>(1), Bit<uint8_t>(0)
    };
    MutableBitSequence<uint8_t>* seq;
    void SetUp() override {
        seq = new MutableBitSequence<uint8_t>(bits, 5);
    }
    void TearDown() override {
        delete seq;
    }
};