// The Utilities header of Krypton Project.
// Copyright (C) 2024  Renatus Madrigal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Krypton {

using byte = char;
using ByteArray = std::basic_string<byte>;
using ByteArrayView = std::basic_string_view<byte>;
// using ByteArrayStream = std::basic_stringstream<byte>;

std::string toBase64(const ByteArray&);
ByteArray fromBase64(const std::string&);
std::string toHex(const ByteArray&);
ByteArray fromHex(const std::string&);
inline ByteArray toBuffer(const std::string& str) { return static_cast<ByteArray>(str); }

inline ByteArray operator""_ba(const char* ptr, size_t len) { return fromHex(std::string(ptr, len)); }

// Note: lhs.length() must equals to rhs.length()
inline ByteArray baxor(const ByteArray& lhs, const ByteArray& rhs)
{
    ByteArray res;
    res.resize(lhs.length());
    for (size_t i = 0; i < lhs.length(); ++i)
        res[i] = lhs[i] ^ rhs[i];
    return res;
}

// TODO: refactor ByteArrayStream
class ByteArrayStream {
private:
    ByteArray buf;
    size_t cur = 0;

public:
    ByteArrayStream()
        : buf()
    {
    }
    ByteArrayStream(const ByteArray& ba)
        : buf(ba)
    {
    }
    ByteArrayStream(size_t bufs)
        : buf(bufs, static_cast<byte>(0))
    {
    }
    void reset() { cur = 0; }
    ByteArrayStream& operator<<(byte c)
    {
        buf.push_back(c);
        return *this;
    }
    ByteArrayStream& operator<<(const ByteArray& ba)
    {
        buf.append(ba);
        return *this;
    }
    ByteArrayStream& operator>>(byte& c)
    {
        c = buf[cur++];
        return *this;
    }
    ByteArrayStream& operator>>(ByteArray& other)
    {
        auto len = other.length();
        for (size_t i = 0; i < len && cur < buf.size(); ++i, ++cur)
            other[i] = buf[cur];
        return *this;
    }
    // TODO: Refactor
    ByteArrayStream& get(ByteArray& other, size_t len)
    {
        other.resize(len);
        for (size_t i = 0; i < len && cur < buf.size(); ++i, ++cur)
            other[i] = buf[cur];
        return *this;
    }
    byte getc() { return buf[cur++]; }
    size_t get(const void*& buf, size_t len)
    {
        auto remain = this->buf.length() - this->cur;
        if (remain < len)
            len = remain;
        buf = this->buf.data() + this->cur;
        cur += len;
        return len;
    }
    bool eof() const
    {
        return cur >= this->buf.length();
    }
    size_t offset() const { return cur; }
    ByteArray str() { return this->buf; }
};

template <typename PrevFunctor>
class Base64Encode {
public:
    template <typename... Args>
    std::string operator()(Args&&... rest)
    {
        return toBase64(PrevFunctor {}(std::forward<Args>(rest)...));
    }
};

template <typename PrevFunctor>
class Base64Decode {
public:
    template <typename... Args>
    ByteArray operator()(Args&&... rest)
    {
        return fromBase64(PrevFunctor {}(std::forward<Args>(rest)...));
    }
};

namespace Detail {
    // from https://stackoverflow.com/questions/15904288/how-to-reverse-the-order-of-arguments-of-a-variadic-template-function
    template <typename, typename>
    struct append_to_type_seq { };

    template <typename T, typename... Ts>
    struct append_to_type_seq<T, std::tuple<Ts...>> {
        using type = std::tuple<Ts..., T>;
    };

    template <typename... Ts>
    struct revert_type_seq {
        using type = std::tuple<>;
    };

    template <typename T, typename... Ts>
    struct revert_type_seq<T, Ts...> {
        using type = typename append_to_type_seq<
            T,
            typename revert_type_seq<Ts...>::type>::type;
    };
    template <int... Is>
    struct index_list { };

    template <int MIN, int N, int... Is>
    struct range_builder;

    template <int MIN, int... Is>
    struct range_builder<MIN, MIN, Is...> {
        typedef index_list<Is...> type;
    };

    template <int MIN, int N, int... Is>
    struct range_builder : public range_builder<MIN, N - 1, N - 1, Is...> { };

    template <int MIN, int MAX>
    using index_range = typename range_builder<MIN, MAX>::type;

    template <typename... Args, int... Is>
    typename revert_type_seq<Args...>::type
    revert_tuple(std::tuple<Args...>&& t, index_list<Is...>)
    {
        using reverted_tuple = typename revert_type_seq<Args...>::type;

        // Forwarding machinery that handles both lvalues and rvalues...
        auto rt = std::forward_as_tuple(
            std::forward<
                typename std::conditional<
                    std::is_lvalue_reference<
                        typename std::tuple_element<Is, reverted_tuple>::type>::value,
                    typename std::tuple_element<Is, reverted_tuple>::type,
                    typename std::remove_reference<
                        typename std::tuple_element<Is, reverted_tuple>::type>::type>::type>(std::get<sizeof...(Args) - Is - 1>(t))...);

        return rt;
    }

    template <typename... Args>
    typename revert_type_seq<Args...>::type
    revert_tuple(std::tuple<Args...>&& t)
    {
        return revert_tuple(std::move(t), index_range<0, sizeof...(Args)>());
    }

    template <typename... Args>
    typename revert_type_seq<Args...>::type
    make_revert(Args&&... args)
    {
        auto t = std::forward_as_tuple(std::forward<Args>(args)...);
        return revert_tuple(std::move(t));
    }

    template <typename F, typename... Args>
    using ReturnType = std::result_of_t<F(Args...)>;

    template <typename F, typename... Args, int... Is>
    ReturnType<F, Args...> revert_call(F&& f, index_list<Is...>, Args&&... args)
    {
        auto rt = make_revert(std::forward<Args>(args)...);
        return f(std::get<Is>(rt)...);
    }

    template <typename F, typename... Args>
    ReturnType<F, Args...> revert_call(F&& f, Args&&... args)
    {
        return revert_call(f, index_range<0, sizeof...(Args)>(),
            std::forward<Args>(args)...);
    }

    template <typename F>
    struct FunctorReverter {
        template <typename... Args>
        ReturnType<F, Args...> operator()(Args&&... args) { return revert_call(F {}, std::forward<Args>(args)...); }
    };
}

struct FunctorFactory {
    template <typename T>
    struct Proxy {
        template <template <typename> typename N>
        using Next = Proxy<N<T>>;
        // using Result = T;
        using Result = Detail::FunctorReverter<T>;
    };
    template <typename T>
    using Next = Proxy<T>;
};

}