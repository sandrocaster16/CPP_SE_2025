#pragma once
#include <cinttypes>
#include <iostream>


struct uint239_t {
    uint8_t data[35]{};

    /*
    max shift:      34359738367
    max shift len:  11
    max bin:        883423532389192164791648750371459257913741948437809479060803100646309887
    max bin len:    72
    */
};

static_assert(sizeof(uint239_t) == 35, "Size of uint239_t must be no higher than 35 bytes");

uint239_t FromInt(uint32_t value, uint32_t shift);

uint239_t FromString(const char* str, uint32_t shift);

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs);

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs);

uint239_t operator*(const uint239_t& lhs, const uint239_t& rhs);

uint239_t operator/(const uint239_t& lhs, const uint239_t& rhs);

bool operator==(const uint239_t& lhs, const uint239_t& rhs);

bool operator!=(const uint239_t& lhs, const uint239_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint239_t& value);

uint64_t GetShift(const uint239_t& value);

uint239_t ShiftUp(const uint239_t& value);

uint239_t SetShift0(const uint239_t& value);

bool operator>(const uint239_t& lhs, const uint239_t& rhs);

bool operator>=(const uint239_t& lhs, const uint239_t& rhs);