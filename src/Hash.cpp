// The Hash algorithms implementation of Krypton Project.
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
#include "Hash.hpp"
#include "Utilities.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>

#define F_MD5(x, y, z) ((x & y) | (~x & z))
#define G_MD5(x, y, z) ((x & z) | (y & ~z))
#define H_MD5(x, y, z) (x ^ y ^ z)
#define I_MD5(x, y, z) (y ^ (x | ~z))
#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define FF_MD5(a, b, c, d, m, s, t) (a = b + (ROTL((a + F_MD5(b, c, d) + m + t), s)))
#define GG_MD5(a, b, c, d, m, s, t) (a = b + (ROTL((a + G_MD5(b, c, d) + m + t), s)))
#define HH_MD5(a, b, c, d, m, s, t) (a = b + (ROTL((a + H_MD5(b, c, d) + m + t), s)))
#define II_MD5(a, b, c, d, m, s, t) (a = b + (ROTL((a + I_MD5(b, c, d) + m + t), s)))

namespace Krypton::Detail {

void MD5HashContext::init()
{
    context->count[0] = 0;
    context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
}

void MD5HashContext::update(const uint8_t* input, uint32_t inputlen)
{
    uint32_t i = 0, index = 0, partlen = 0;
    index = (context->count[0] >> 3) & 0x3F;
    partlen = 64 - index;
    context->count[0] += inputlen << 3;
    if (context->count[0] < (inputlen << 3))
        context->count[1]++;
    context->count[1] += inputlen >> 29;

    if (inputlen >= partlen) {
        memcpy(&context->buffer[index], input, partlen);
        transform(context->state, context->buffer);
        for (i = partlen; i + 64 <= inputlen; i += 64)
            transform(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }
    memcpy(&context->buffer[index], &input[i], inputlen - i);
}

void MD5HashContext::final()
{
    uint32_t index = 0, padlen = 0;
    uint8_t bits[8];
    index = (context->count[0] >> 3) & 0x3F;
    padlen = (index < 56) ? (56 - index) : (120 - index);
    encode(bits, context->count, 8);
    update(kPadding, padlen);
    update(bits, 8);
    encode(digest, context->state, 16);
}

void MD5HashContext::encode(uint8_t* output, const uint32_t* input, uint32_t len)
{
    uint32_t i = 0, j = 0;
    while (j < len) {
        output[j] = input[i] & 0xFF;
        output[j + 1] = (input[i] >> 8) & 0xFF;
        output[j + 2] = (input[i] >> 16) & 0xFF;
        output[j + 3] = (input[i] >> 24) & 0xFF;
        i++;
        j += 4;
    }
}

void MD5HashContext::decode(uint32_t* output, const uint8_t* input, uint32_t len)
{
    uint32_t i = 0, j = 0;
    while (j < len) {
        output[i] = (input[j]) | (input[j + 1] << 8) | (input[j + 2] << 16) | (input[j + 3] << 24);
        i++;
        j += 4;
    }
}

void MD5HashContext::transform(uint32_t state[4], const uint8_t block[64])
{
    uint32_t a[4];
    a[0] = state[3];
    a[1] = state[2];
    a[2] = state[1];
    a[3] = state[0];
    uint32_t x[16];
    int kinit[4] = { 0, 1, 5, 0 };

    decode(x, block, 64);
    for (int i = 0; i < 4; i++) {
        int k = kinit[i];
        for (int j = 0; j < 16; j++) {
            switch (i) {
            case 0:
                FF_MD5(a[(j + 3) % 4], a[(j + 2) % 4], a[(j + 1) % 4], a[j % 4],
                    x[k], s[i][j % 4], t[i * 16 + j]);
                k = (k + 1) % 16;
                break;
            case 1:
                GG_MD5(a[(j + 3) % 4], a[(j + 2) % 4], a[(j + 1) % 4], a[j % 4],
                    x[k], s[i][j % 4], t[i * 16 + j]);
                k = (k + 5) % 16;
                break;
            case 2:
                HH_MD5(a[(j + 3) % 4], a[(j + 2) % 4], a[(j + 1) % 4], a[j % 4],
                    x[k], s[i][j % 4], t[i * 16 + j]);
                k = (k + 3) % 16;
                break;
            case 3:
                II_MD5(a[(j + 3) % 4], a[(j + 2) % 4], a[(j + 1) % 4], a[j % 4],
                    x[k], s[i][j % 4], t[i * 16 + j]);
                k = (k + 7) % 16;
                break;
            default:
                break;
            }
        }
    }
    state[0] += a[3];
    state[1] += a[2];
    state[2] += a[1];
    state[3] += a[0];
}

ByteArray MD5HashContext::hash(const ByteArray& inp)
{
    this->init();
    this->update(reinterpret_cast<const uint8_t*>(inp.data()), inp.size());
    this->final();
    // return { reinterpret_cast<byte*>(this->digest), 16 };
    return { this->digest, this->digest + 16 };
}

uint32_t SHA1Context::f(size_t t, uint32_t b, uint32_t c, uint32_t d)
{
    switch (t / 20) {
    case 0:
        return (b & c) | ((~b) & d);
    case 1:
        return b ^ c ^ d;
    case 2:
        return (b & c) | (b & d) | (c & d);
    case 3:
        return b ^ c ^ d;
    default:
        return -1;
    }
}

void SHA1Context::init()
{
    h[0] = 0x67452301;
    h[1] = 0xEFCDAB89;
    h[2] = 0x98BADCFE;
    h[3] = 0x10325476;
    h[4] = 0xC3D2E1F0;
}

void SHA1Context::updateBlock(const uint8_t* block)
{
    static uint32_t w[80];
    // memcpy(w, ptr, 4 * 16);
    for (size_t t = 0; t < 16; ++t) {
        w[t] = (block[4 * t] << 24) | (block[4 * t + 1] << 16) | (block[4 * t + 2] << 8) | block[4 * t + 3];
    }
    for (size_t t = 16; t < 80; ++t) {
        w[t] = ROTL(w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16], 1);
    }
    auto a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
    uint32_t tmp = 0;
    for (size_t t = 0; t < 80; ++t) {
        tmp = ROTL(a, 5) + f(t, b, c, d) + e + w[t] + k[t / 20];
        e = d;
        d = c;
        c = ROTL(b, 30);
        b = a;
        a = tmp;
    }
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

void SHA1Context::initBuffer(const ByteArray& inp)
{
    uint64_t msglen = inp.size();
    if (msglen % 64 < 56) {
        bufsize = (msglen / 64 + 1) * 64;
    } else {
        bufsize = (msglen / 64 + 2) * 64;
    }
    this->buffer = new uint8_t[bufsize];
    memset(buffer, 0, bufsize);
    memcpy(this->buffer, inp.data(), msglen);
    this->buffer[msglen] = 0x80;
    msglen *= 8;
    for (size_t i = 0; i < 8; ++i) {
        this->buffer[bufsize - i - 1] = (msglen >> (i * 8)) & 0xff;
    }
}

void SHA1Context::update(const uint8_t* buf, size_t len)
{
    for (size_t i = 0; i + 63 < len; i += 64) {
        this->updateBlock(buf + i);
    }
}

// TODO: improve performance of sha1
ByteArray SHA1Context::hash(const ByteArray& inp)
{
    this->init();
    this->initBuffer(inp);
    this->update(this->buffer, this->bufsize);
    for (size_t i = 0; i < 20; ++i) {
        this->digest[i] = (this->h[i / 4] >> ((3 - i % 4) * 8)) & 0xff;
    }
    // return { reinterpret_cast<byte*>(this->digest), 20 };
    return { this->digest, this->digest + 20 };
}

#define CH(x, y, z) (((x) & (y)) ^ ((~(x)) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define BSIG0_SHA256(x) (ROTR((x), 2) ^ ROTR((x), 13) ^ ROTR((x), 22))
#define BSIG1_SHA256(x) (ROTR((x), 6) ^ ROTR((x), 11) ^ ROTR((x), 25))
#define SSIG0_SHA256(x) (ROTR((x), 7) ^ ROTR((x), 18) ^ ((x) >> 3))
#define SSIG1_SHA256(x) (ROTR((x), 17) ^ ROTR((x), 19) ^ ((x) >> 10))

void SHA256Context::init()
{
    h[0] = 0x6a09e667;
    h[1] = 0xbb67ae85;
    h[2] = 0x3c6ef372;
    h[3] = 0xa54ff53a;
    h[4] = 0x510e527f;
    h[5] = 0x9b05688c;
    h[6] = 0x1f83d9ab;
    h[7] = 0x5be0cd19;
}

void SHA256Context::updateBlock(const uint8_t* block)
{
    static uint32_t w[64];
    for (size_t t = 0; t < 16; ++t) {
        w[t] = (block[4 * t] << 24) | (block[4 * t + 1] << 16) | (block[4 * t + 2] << 8) | block[4 * t + 3];
    }
    for (size_t t = 16; t < 64; ++t) {
        w[t] = SSIG1_SHA256(w[t - 2]) + w[t - 7] + SSIG0_SHA256(w[t - 15]) + w[t - 16];
    }
    uint32_t b[8];
    for (size_t i = 0; i < 8; ++i)
        b[i] = h[i];
    uint32_t t1 = 0, t2 = 0;
    for (size_t t = 0; t < 64; ++t) {
        t1 = b[7] + BSIG1_SHA256(b[4]) + CH(b[4], b[5], b[6]) + k[t] + w[t];
        t2 = BSIG0_SHA256(b[0]) + MAJ(b[0], b[1], b[2]);
        for (size_t i = 7; i > 0; --i)
            b[i] = b[i - 1];
        b[4] += t1;
        b[0] = t1 + t2;
    }
    for (size_t i = 0; i < 8; ++i)
        h[i] += b[i];
}

void SHA256Context::initBuffer(const ByteArray& inp)
{
    uint64_t msglen = inp.size();
    if (msglen % 64 < 56) {
        bufsize = (msglen / 64 + 1) * 64;
    } else {
        bufsize = (msglen / 64 + 2) * 64;
    }
    this->buffer = new uint8_t[bufsize];
    memset(buffer, 0, bufsize);
    memcpy(this->buffer, inp.data(), msglen);
    this->buffer[msglen] = 0x80;
    msglen *= 8;
    for (size_t i = 0; i < 8; ++i) {
        this->buffer[bufsize - i - 1] = (msglen >> (i * 8)) & 0xff;
    }
}

void SHA256Context::update(const uint8_t* buf, size_t len)
{
    for (size_t i = 0; i + 63 < len; i += 64) {
        this->updateBlock(buf + i);
    }
}

// TODO: improve performance of sha256
ByteArray SHA256Context::hash(const ByteArray& inp)
{
    this->init();
    this->initBuffer(inp);
    this->update(this->buffer, this->bufsize);
    for (size_t i = 0; i < digestLen; ++i) {
        this->digest[i] = (this->h[i / 4] >> ((3 - i % 4) * 8)) & 0xff;
    }
    // return { reinterpret_cast<byte*>(this->digest), digestLen };
    return { this->digest, this->digest + digestLen };
}

}