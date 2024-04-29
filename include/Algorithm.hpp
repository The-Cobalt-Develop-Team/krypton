// The algorithm header of Krypton Project.
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

#include "Hash.hpp"
#include "Utilities.hpp"
#include "range/v3/algorithm.hpp"
#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view.hpp"
#include "range/v3/view/slice.hpp"
#include "range/v3/view/subrange.hpp"
#include "range/v3/view/transform.hpp"
#include <exception>
#include <iostream>
#include <random>
#include <utility>

namespace Krypton {

namespace Detail {
    using namespace ranges;
    struct MGF1Impl {
        template <typename HashCtx = SHA1Context>
        static ByteArray mgf(const ByteArray& seed, uint32_t len)
        {
            size_t slen = seed.size();
            HashCtx ctx;
            int n = (len + HashCtx::digestLen - 1) / HashCtx::digestLen;
            auto hs = views::iota(0, n) | views::transform([&](int x) {
                return ctx.hash(views::concat(seed, views::iota(0, 4) | views::transform([&](int i) { return x >> (8 * (3 - i)); })) | ranges::to<ByteArray>());
            });
            auto res = views::join(hs) | views::take_exactly(len) | ranges::to<ByteArray>();
            return res;
        }
    };
    // TODO: fix unnecessary copies
    template <typename HashCtx = SHA1Context, typename MGF = MGF1Impl>
    ByteArray OAEPEncodeImpl(const ByteArray& msg, size_t k, const ByteArray& label = ""_ba)
    {
        HashCtx ctx;
        auto lhash = ctx.hash(label);
        auto hlen = HashCtx::digestLen;
        auto mlen = msg.size();

        size_t pslen = k - msg.size() - 2 * hlen - 2;
        std::random_device rd;
        std::mt19937_64 rng(rd());
        std::uniform_int_distribution<int> dist(0, 255);
        auto seed = views::generate([&]() -> byte { return static_cast<byte>(dist(rng)); }) | views::take(hlen) | to<ByteArray>();
        auto db = views::concat(lhash, views::repeat(0x00) | views::take(k - mlen - 2 * hlen - 2), views::single(0x01), msg);
        auto msk1 = MGF::mgf(seed, k - hlen - 1);
        auto mskdb = views::zip(db, msk1) | views::transform([](std::pair<byte, byte> p) -> byte { return p.first ^ p.second; });
        auto msk2 = MGF::mgf(mskdb | to<ByteArray>(), hlen);
        auto mskseed = views::zip(seed, msk2) | views::transform([](std::pair<byte, byte> p) -> byte { return p.first ^ p.second; });
        auto resrng = views::concat(views::single(0x00), mskseed, mskdb);

        return resrng | to<ByteArray>();
    };

    struct OAEPException : std::exception {
        [[nodiscard]] const char* what() const noexcept override
        {
            return "Invalid OAEP ByteArray";
        }
    };

    // TODO: exception
    template <typename HashCtx = SHA1Context, typename MGF = MGF1Impl>
    ByteArray OAEPDecodeImpl(const ByteArray& cipher, const ByteArray& label = ""_ba)
    {
        ByteArray res;
        if (cipher[0] != 0)
            throw OAEPException();
        HashCtx ctx;
        auto lhash = ctx.hash(label);
        auto hlen = HashCtx::digestLen;
        auto k = cipher.size();
        auto mskseed = cipher | views::slice(1_sz, hlen + 1);
        auto mskdb = cipher | views::slice(hlen + 1, k);
        auto seedmsk = MGF::mgf(mskdb | to<ByteArray>(), hlen);
        auto seed = views::zip(mskseed, seedmsk) | views::transform([](std::pair<byte, byte> p) -> byte { return p.first ^ p.second; });
        auto dbmsk = MGF::mgf(seed | to<ByteArray>(), k - hlen - 1);
        auto db = views::zip(mskdb, dbmsk) | views::transform([](std::pair<byte, byte> p) -> byte { return p.first ^ p.second; });
        auto l = db | views::slice(0_sz, hlen) | to<ByteArray>();
        if (l != lhash)
            throw OAEPException();
        auto pos = find_if(db.begin() + hlen, db.end(), [](byte x) { return x == 0x01; });
        if (pos == db.end())
            throw OAEPException();
        auto msg = ranges::subrange(pos + 1, db.end());
        return msg | to<ByteArray>();
    }

    struct PKCS1Exception : std::exception {
        [[nodiscard]] const char* what() const noexcept override
        {
            return "Invalid PKCS #1 Text";
        }
    };

    ByteArray PKCS1EncodeImpl(const ByteArray& plain, size_t k);
    ByteArray PKCS1DecodeImpl(const ByteArray& cipher);
};

template <typename Plain, typename Size>
struct OAEPEncode {
    template <typename... Args>
    ByteArray operator()(Args&&... args)
    {
        return Detail::OAEPEncodeImpl(Plain {}(std::forward<Args>(args)...), Size {}(std::forward<Args>(args)...));
    }
};

template <typename Plain, typename Size>
struct PKCS1Encode {
    template <typename... Args>
    ByteArray operator()(Args&&... args)
    {
        return Detail::PKCS1EncodeImpl(Plain {}(std::forward<Args>(args)...), Size {}(std::forward<Args>(args)...));
    }
};

template <typename Prev>
struct OAEPDecode {
    template <typename... Args>
    ByteArray operator()(Args&&... args)
    {
        return Detail::OAEPDecodeImpl(Prev {}(std::forward<Args>(args)...));
    }
};

template <typename Prev>
struct PKCS1Decode {
    template <typename... Args>
    ByteArray operator()(Args&&... args)
    {
        return Detail::PKCS1DecodeImpl(Prev {}(std::forward<Args>(args)...));
    }
};

}