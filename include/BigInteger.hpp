/*
    Big Integer Library for Krypton Project.
    Warning: This file is discarded. Please rechecked.
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
#include <random>
#include <vector>

namespace Krypton {

namespace BigIntBases {
    class Base10BigInt {
    private:
        std::vector<uint8_t> _data;
        bool isNega = false;
        inline void removeZero()
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
        inline void addbit(uint8_t x)
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
        // Converting Functions
        [[nodiscard]] inline std::string toString() const
        {
            std::string ans;
            if (isNega) {
                ans.push_back('-');
            }
            for (size_t i = _data.size() - 1; i + 1 >= 1; i--) {
                ans.push_back(char(_data[i] + '0'));
            }
            return ans;
        }
    }; // Only for Bin to Oct converter, So we only provide add operator.

    template <size_t N>
    class Base2BigInt {
    private:
        static const size_t len = N;
        std::bitset<len> _data;

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
        // Operator Functions
        static Base2BigInt add_op(const Base2BigInt& lhs, const Base2BigInt& rhs)
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

        static Base2BigInt mul_op(Base2BigInt& lhs, Base2BigInt& rhs)
        {
            Base2BigInt ans {};
            for (int i = 0; i < len; i++) {
                if (rhs._data[i]) {
                    ans = ans + (lhs << i);
                }
            }
            return ans;
        }

        Base2BigInt operator+(Base2BigInt& rhs)
        {
            return add_op(*this, rhs);
        }

        inline Base2BigInt operator-() const
        {
            Base2BigInt tmp = *this;
            tmp._data.flip();
            return tmp + Base2BigInt<len>(1);
        }

        inline Base2BigInt operator<<(size_t x) const
        {
            Base2BigInt tmp(*this);
            tmp._data <<= 1;
            return tmp;
        }
        // Comparison Operators
        inline bool operator==(const Base2BigInt& rhs) const
        {
            for (int i = 0; i < len; i++) {
                if (this->_data[i] ^ rhs._data[i]) {
                    return false;
                }
            }
            return true;
        }
        inline bool operator!=(const Base2BigInt& rhs) const
        {
            return !(*this == rhs);
        }
        inline bool operator<(const Base2BigInt& rhs) const
        {
            for (int i = len - 1; i + 1 > 1; i--) {
                if (this->_data[i] >= rhs._data[i]) {
                    return false;
                }
            }
            return true;
        }
        inline bool operator<=(const Base2BigInt& rhs) const
        {
            return (*this < rhs) || (*this == rhs);
        }
        inline bool operator>(const Base2BigInt& rhs) const
        {
            for (int i = len - 1; i + 1 > 1; i--) {
                if (this->_data[i] <= rhs._data[i]) {
                    return false;
                }
            }
            return true;
        }
        inline bool operator>=(const Base2BigInt& rhs) const
        {
            return (*this > rhs) || (*this == rhs);
        }

        // Converting Function
        [[nodiscard]] inline long long toll() const
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

        template <typename RNG>
        static Base2BigInt<N> random(RNG& rng, const Base2BigInt<N>& min, const Base2BigInt<N>& max)
        {
            // TODO: Change another better random algorithm
            std::uniform_int_distribution<unsigned int> distr(0, 2);
            auto diff = Base2BigInt<N>::add_op(max, -min);
            auto res = Base2BigInt<N>(0);
            // std::cout << diff.toll() << std::endl;
            for (size_t i = 0; i < N; ++i) {
                res._data[i] = distr(rng);
            }
            res._data = res._data & diff._data;
            return Base2BigInt<N>::add_op(res, min);
        }
    }; // Use Bin, Contain most of methods.
}

template <size_t N>
class BigInt {
private:
    static const size_t len = N;
    BigIntBases::Base2BigInt<N> _data;

public:
    // Construction Functions
    BigInt() = default;
    explicit BigInt(BigIntBases::Base2BigInt<N> x)
    {
        _data = x;
    }
    explicit BigInt(long long x)
    {
        _data = BigIntBases::Base2BigInt<len>(x);
    }
    // Output Functions
    void show()
    {
        _data.show();
    }
    // Getters
    [[nodiscard]] inline size_t size() const
    {
        return len;
    }
    // Unary Operators
    inline BigInt operator-() const
    {
        return BigInt(-_data);
    }

    // Binary Operators
    inline BigInt operator+(const BigInt& rhs)
    {
        return BigInt(BigIntBases::Base2BigInt<len>::add_op(this->_data, rhs._data));
    }
    inline BigInt operator-(const BigInt& rhs)
    {
        return BigInt(BigIntBases::Base2BigInt<len>::add_op(*this, -rhs));
    }
    inline BigInt operator*(const BigInt& rhs)
    {
        return BigInt(BigIntBases::Base2BigInt<len>::mul_op(*this, rhs));
    }
    inline BigInt operator<<(size_t x) const
    {
        return BigInt(_data << x);
    }
    inline BigInt operator==(const BigInt& rhs)
    {
        return this->_data == rhs._data;
    }
    inline BigInt operator!=(const BigInt& rhs)
    {
        return this->_data != rhs._data;
    }
    inline BigInt operator<(const BigInt& rhs)
    {
        return this->_data < rhs._data;
    }
    inline BigInt operator<=(const BigInt& rhs)
    {
        return this->_data <= rhs._data;
    }
    inline BigInt operator>(const BigInt& rhs)
    {
        return this->_data > rhs._data;
    }
    inline BigInt operator>=(const BigInt& rhs)
    {
        return this->_data >= rhs._data;
    }

    inline BigInt operator%(const BigInt& rhs) const; // TODO: Should be finished
    // Converting Functions
    [[nodiscard]] inline long long toll() const
    {
        return _data.toll();
    }
    [[nodiscard]] inline std::string toString() const
    {
        return _data.toBase10().toString();
    }
    template <typename RNG>
    static BigInt<N> random(RNG& rng, BigInt<N> min, BigInt<N> max)
    {
        return BigInt<N>(decltype(_data)::random(rng, min._data, max._data));
    }
};
}

// #endif
