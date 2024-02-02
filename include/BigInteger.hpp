/*
    Big Integer Library for Krypton Project.
    Copyright (C) 2024  Andy Shen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*

    We use vector<bool> for base container. LowBit will at head, HighBit will be at tail
    For basic use, here're only some methods:
        operator+
        operator-
        operator*
        operator%
        operator<<
        operator>>
        quick_pow
*/
// #ifndef BIGINTEGER_INC
// #define BIGINTEGER_INC
#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>
#include <vector>

namespace BigIntBases {
class Base10BigInt {
private:
    std::vector<uint8_t> _data;
    bool isNega = false;
    void removeZero()
    {
        for (size_t i = _data.size() - 1; i >= 0; i--) {
            if (_data[i] != 0) {
                break;
            }
            _data.pop_back();
        }
        if (_data.empty()) {
            _data.push_back(0);
        }
    }
    void addbit(uint8_t x)
    {
        _data.push_back(x);
    }
    static Base10BigInt add_op(const Base10BigInt& lhs, const Base10BigInt& rhs)
    {
        uint8_t carry = 0;
        Base10BigInt ans(std::max(lhs.size(), rhs.size() + 1), lhs.isNega); // Since it's only for converting, so lhs*rhs>0!
        for (size_t i = 0; i < ans.size(); i++) {
            uint8_t cur = carry + lhs[i] + rhs[i];
            ans[i] = cur % 10;
            carry = cur / 10;
        }
        ans.removeZero();
        return ans;
    }

public:
    // Initialization Functions
    Base10BigInt() = default;
    Base10BigInt(size_t x, bool nega)
    {
        _data.resize(x);
        isNega = nega;
    }
    explicit Base10BigInt(long long x)
    {
        if (x == 0) {
            addbit(0);
        }
        if (x < 0) {
            isNega = true;
            x = -x;
        }
        while (x > 0) {
            addbit(x % 10);
            x /= 10;
        }
    }
    // Debug Functions
    void show()
    {
        if (isNega) {
            std::cout << '-';
        }
        for (size_t i = _data.size() - 1; i + 1 >= 1; i--) {
            std::cout << int(_data[i]);
        }
        std::cout << std::endl;
    }
    // Argument Functions
    [[nodiscard]] inline size_t size() const
    {
        return _data.size();
    }
    // Operators
    inline uint8_t& operator[](size_t x)
    {
        return _data.at(x);
    }
    inline uint8_t operator[](size_t x) const
    {
        return x >= size() ? 0 : _data.at(x);
    }
    inline Base10BigInt operator+(const Base10BigInt& rhs) const
    {
        return add_op(*this, rhs);
    }
}; // Only for Bin to Oct converter, So we only provide add operator.

template <size_t N>
class Base2BigInt {
private:
    static const size_t len = N;
    std::bitset<len> _data;
    Base2BigInt add_op(const Base2BigInt& lhs, const Base2BigInt& rhs)
    {
        bool carry = false;
        Base2BigInt<len> ans;
        for (size_t i = 0; i < len; i++) {
            uint8_t cur = carry + lhs._data[i] + rhs._data[i];
            ans._data[i] = cur & 1;
            carry = cur >> 1;
        }
        return ans;
    }

    Base2BigInt mul_op(Base2BigInt& lhs, Base2BigInt& rhs)
    {
        Base2BigInt ans {};
        for (int i = 0; i < len; i++) {
            if (rhs._data[i]) {
                ans = ans + (lhs << i);
            }
        }
        return ans;
    }

public:
    // Initialization Functions
    Base2BigInt()
        = default;
    explicit Base2BigInt(long long x)
    {
        _data.reset();
        for (int i = 0; i < len; i++) {
            if (x == 0) {
                break;
            }
            _data[i] = x & 1;
            x >>= 1;
        }
    }
    // Debug Functions
    void show()
    {
        for (size_t i = _data.size() - 1; i + 1 >= 1; i--) {
            std::cout << int(_data[i]);
        }
        std::cout << std::endl;
    }
    // Argument Functions
    [[nodiscard]] inline size_t size() const
    {
        return len;
    }
    // Operators
    inline Base2BigInt operator+(const Base2BigInt& rhs)
    {
        return add_op(*this, rhs);
    }
    inline Base2BigInt operator-() const
    {
        Base2BigInt tmp = *this;
        tmp._data.flip();
        return tmp + Base2BigInt<len>(1);
    }
    inline Base2BigInt operator-(Base2BigInt& rhs)
    {
        return add_op(*this, -rhs);
    }
    inline Base2BigInt operator*(Base2BigInt& rhs)
    {
        return mul_op(*this, rhs);
    }
    inline Base2BigInt operator<<(size_t x) const
    {
        Base2BigInt tmp(*this);
        tmp._data <<= 1;
        return tmp;
    }
    // Converting Functions
    [[nodiscard]] long long toll() const
    {
        long long base = 1;
        long long ans = 0;
        for (int i = 0; i < len; i++) {
            ans += base * _data[i];
            base <<= 1;
        }
        return ans;
    }
    [[nodiscard]] Base10BigInt toBase10() const
    {
        bool isNega = _data[0];
        Base2BigInt<len> tmp(*this);
        if (isNega) {
            tmp = -tmp;
        }
        Base10BigInt base(1);
        Base10BigInt ans(1, isNega);
        for (int i = 0; i < len; i++) {
            if (tmp._data[i]) {
                ans = ans + base;
            }
            base = base + base;
        }
        return ans;
    }

}; // Use Bin, Contain most of methods.
}

template <size_t N>
class BigInt {
private:
    static const size_t len = N;
    BigIntBases::Base2BigInt<N> _data;

public:
};

// #endif
