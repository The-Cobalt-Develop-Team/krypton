#pragma once

#include "Hash.hpp"
#include "Utilities.hpp"

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
    template <typename HashCtx, typename MGF>
    ByteArray OAEPEncodeImpl() {};
};
}