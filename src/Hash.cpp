#include "Hash.hpp"
#include "Utilities.hpp"

#define F_MD5(x, y, z) ((x & y) | (~x & z))
#define G_MD5(x, y, z) ((x & z) | (y & ~z))
#define H_MD5(x, y, z) (x ^ y ^ z)
#define I_MD5(x, y, z) (y ^ (x | ~z))
#define ROTATE_LEFT_MD5(x, n) ((x << n) | (x >> (32 - n)))

#define FF_MD5(a, b, c, d, m, s, t) (a = b + (ROTATE_LEFT_MD5((a + F_MD5(b, c, d) + m + t), s)))
#define GG_MD5(a, b, c, d, m, s, t) (a = b + (ROTATE_LEFT_MD5((a + G_MD5(b, c, d) + m + t), s)))
#define HH_MD5(a, b, c, d, m, s, t) (a = b + (ROTATE_LEFT_MD5((a + H_MD5(b, c, d) + m + t), s)))
#define II_MD5(a, b, c, d, m, s, t) (a = b + (ROTATE_LEFT_MD5((a + I_MD5(b, c, d) + m + t), s)))

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
    // context: 已初始化的结构体
    // input: 需加密的信息
    // inputlen: 需加密的信息的长度
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
    // context: 加密好的 MD5结构体
    // digest: 最终保存位置
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
    // state: MD5结构体里的 state数组
    // block: 要加密的 512bits数据块
    uint32_t a[4];
    // 注意一下，这里 a[0],a[1],a[2],a[3]分别代表 d,c,b,a, 顺序是反的, 方便后面操作
    a[0] = state[3];
    a[1] = state[2];
    a[2] = state[1];
    a[3] = state[0];
    uint32_t x[16];
    int kinit[4] = { 0, 1, 5, 0 }; // k的初始值

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
    return { reinterpret_cast<byte*>(this->digest), 16 };
}

}