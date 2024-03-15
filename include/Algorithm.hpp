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
#include <exception>
#include <random>

namespace Krypton {

template <typename Prev>
struct OAEPEncode { };

namespace Detail {
    struct MGF1Impl {
        template <typename HashCtx = SHA1Context>
        static ByteArray mgf(ByteArray seed, uint32_t len)
        {
            ByteArray res(len, static_cast<byte>(0));
            ByteArray h;
            size_t slen = seed.length();
            HashCtx ctx;
            seed.append(4, static_cast<char>(0));
            res.resize(len);
            for (uint32_t i = 0, cur = 0; cur < len; ++i) {
                for (int s = 0; s < 4; ++s)
                    seed[slen + s] = static_cast<char>(i >> (8 * (3 - s)));
                h = ctx.hash(seed);
                for (size_t s = 0; s < h.length(); ++s, ++cur) {
                    if (cur >= len)
                        break;
                    res[cur] = h[s];
                }
            }
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
        auto mlen = msg.length();

        size_t pslen = k - msg.length() - 2 * hlen - 2;
        ByteArray res(k, static_cast<byte>(0));
        auto ptr = res.data();

        memcpy(ptr + hlen + 1, lhash.data(), hlen);
        *(ptr + k - mlen - 1) = 0x01;
        memcpy(ptr + k - mlen, msg.data(), mlen);

        ByteArray seed(hlen, static_cast<byte>(0));
        std::random_device rd;
        std::mt19937_64 rng(rd());
        std::uniform_int_distribution<int> dist(0, 255);
        for (auto& s : seed)
            s = dist(rng);

        auto mask = MGF::mgf(seed, k - hlen - 1);
        for (size_t i = hlen + 1; i < k; ++i)
            res[i] ^= mask[i - hlen - 1];

        ByteArray maskedDB(ptr + hlen + 1, k - hlen - 1);
        mask = MGF::mgf(maskedDB, hlen);
        memcpy(ptr + 1, seed.data(), hlen);
        for (size_t i = 1; i < hlen + 1; ++i)
            res[i] ^= mask[i - 1];

        return res;
    };

    struct OAEPException : std::exception {
        char const* what() const override
        {
            return "Invalid OAEP Text";
        }
    };

    // TODO: exception
    template <typename HashCtx = SHA1Context, typename MGF = MGF1Impl>
    ByteArray OAEPDecode(const ByteArray& cipher, const ByteArray& label = ""_ba)
    {
        ByteArray res;
        if (cipher[0] != 0)
            throw OAEPException();
        HashCtx ctx;
        auto lhash = ctx.hash(label);
        auto ptr = cipher.data();
        auto hlen = HashCtx::digestLen;
        auto k = cipher.length();
        ByteArray seed(ptr + 1, hlen);
        ByteArray db(ptr + hlen + 1, k - hlen - 1);
        auto seedmsk = MGF::mgf(db, hlen);
        for (size_t i = 0; i < hlen; ++i)
            seed[i] ^= seedmsk[i];
        auto dbmsk = MGF::mgf(seed, k - hlen - 1);
        for (size_t i = 0; i < k - hlen - 1; ++i)
            db[i] ^= dbmsk[i];

        ByteArray l(db.data(), hlen);
        if (l != lhash)
            throw OAEPException();
        size_t idx = hlen;
        while (db[idx] == 0x00)
            ++idx;
        if (db[idx] != 0x01)
            throw OAEPException();
        ++idx;
        res = ByteArray(db.data() + idx, db.length() - idx);
        return res;
    }
};
}