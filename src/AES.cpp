#include "AES.hpp"

#include <cstdlib>
#include <cstring>

namespace Krypton::Detail {

void AES128Impl::clear()
{
    memset(w, 0, sizeof(w));
    memset(state, 0, sizeof(state));
}

void AES128Impl::init(uint8_t* in, uint8_t* in_key, bool type)
{
    memset(key, 0, sizeof(key));
    memset(plain, 0, sizeof(plain));
    memset(cipher, 0, sizeof(cipher));
    clear();
    if (type) {
        for (int i = 0; i < 4 * Nb; ++i) {
            plain[i] = in[i];
        }
    } else {
        for (int i = 0; i < 4 * Nb; ++i) {
            cipher[i] = in[i];
        }
    }
    for (int i = 0; i < 4 * Nk; ++i) {
        key[i] = in_key[i];
    }
}

void AES192Impl::clear()
{
    memset(w, 0, sizeof(w));
    memset(state, 0, sizeof(state));
}

void AES192Impl::init(uint8_t* in, uint8_t* in_key, bool type)
{
    memset(key, 0, sizeof(key));
    memset(plain, 0, sizeof(plain));
    memset(cipher, 0, sizeof(cipher));
    clear();
    if (type) {
        for (int i = 0; i < 4 * Nb; ++i) {
            plain[i] = in[i];
        }
    } else {
        for (int i = 0; i < 4 * Nb; ++i) {
            cipher[i] = in[i];
        }
    }
    for (int i = 0; i < 4 * Nk; ++i) {
        key[i] = in_key[i];
    }
}

void AES256Impl::clear()
{
    memset(w, 0, sizeof(w));
    memset(state, 0, sizeof(state));
}

void AES256Impl::init(uint8_t* in, uint8_t* in_key, bool type)
{
    memset(key, 0, sizeof(key));
    memset(plain, 0, sizeof(plain));
    memset(cipher, 0, sizeof(cipher));
    clear();
    if (type) {
        for (int i = 0; i < 4 * Nb; ++i) {
            plain[i] = in[i];
        }
    } else {
        for (int i = 0; i < 4 * Nb; ++i) {
            cipher[i] = in[i];
        }
    }
    for (int i = 0; i < 4 * Nk; ++i) {
        key[i] = in_key[i];
    }
}

uint32_t BaseAESImpl::toWord(uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4)
{
    uint32_t res;
    res = (k1 << 24) | (k2 << 16) | (k3 << 8) | (k4);
    return res;
}

void BaseAESImpl::toBytes(uint32_t in, uint8_t* res)
{
    uint32_t tmp = in;
    for (int i = 3; i >= 0; --i) {
        res[i] = tmp % 256;
        tmp /= 256;
    }
}

uint8_t BaseAESImpl::GFMul(uint8_t a, uint8_t b)
{
    switch (a) {
    case 0x01:
        return Mul_01[b];
    case 0x02:
        return Mul_02[b];
    case 0x03:
        return Mul_03[b];
    case 0x09:
        return Mul_09[b];
    case 0x0b:
        return Mul_0b[b];
    case 0x0d:
        return Mul_0d[b];
    case 0x0e:
        return Mul_0e[b];
    }
}

void BaseAESImpl::ByteSub()
{
    for (auto& i : state) {
        i = S_Box[i];
    }
}

void BaseAESImpl::ShiftRow()
{
    for (int i = 1; i < Nb; ++i) {
        for (int j = 0; j < i; ++j) {
            uint8_t tmp = state[i];
            state[i + 0 * 4] = state[i + (0 + 1) * 4];
            state[i + 1 * 4] = state[i + (1 + 1) * 4];
            state[i + 2 * 4] = state[i + (2 + 1) * 4];
            state[i + 3 * 4] = tmp;
        }
    }
}

void BaseAESImpl::MixColumn()
{
    const uint8_t y[16] = { 0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02 };
    uint8_t arr[4];
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
            arr[k] = 0;
            for (int j = 0; j < 4; ++j) {
                arr[k] = arr[k] ^ GFMul(y[k * 4 + j], state[i * 4 + j]);
            }
        }
        for (int k = 0; k < 4; ++k) {
            state[i * 4 + k] = arr[k];
        }
    }
}

void BaseAESImpl::AddRoundKey(uint32_t* ExpandedKey, int i)
{
    toBytes(ExpandedKey[Nb * i + 0], RoundKey + 0 * Nb);
    toBytes(ExpandedKey[Nb * i + 1], RoundKey + 1 * Nb);
    toBytes(ExpandedKey[Nb * i + 2], RoundKey + 2 * Nb);
    toBytes(ExpandedKey[Nb * i + 3], RoundKey + 3 * Nb);
    for (int k = 0; k < Nb * 4; ++k) {
        state[k] = state[k] ^ RoundKey[k];
    }
}

void BaseAESImpl::InvByteSub()
{
    for (auto& i : state) {
        i = Inv_S_Box[i];
    }
}

void BaseAESImpl::InvShiftRow()
{
    for (int i = 1; i < Nb; i++) {
        for (int j = 0; j < Nb - i; ++j) {
            uint8_t tmp = state[i];
            state[i + 0 * 4] = state[i + (0 + 1) * 4];
            state[i + 1 * 4] = state[i + (1 + 1) * 4];
            state[i + 2 * 4] = state[i + (2 + 1) * 4];
            state[i + 3 * 4] = tmp;
        }
    }
}

void BaseAESImpl::InvMixColumn()
{
    const uint8_t y[16] = { 0x0e, 0x0b, 0x0d, 0x09,
        0x09, 0x0e, 0x0b, 0x0d,
        0x0d, 0x09, 0x0e, 0x0b,
        0x0b, 0x0d, 0x09, 0x0e };
    uint8_t arr[4];
    for (int i = 0; i < Nb; ++i) {
        for (int k = 0; k < 4; ++k) {
            arr[k] = 0;
            for (int j = 0; j < 4; ++j) {
                arr[k] = arr[k] ^ GFMul(y[k * 4 + j], state[i * 4 + j]);
            }
        }
        for (int k = 0; k < 4; ++k) {
            state[i * 4 + k] = arr[k];
        }
    }
}

uint32_t BaseAESImpl::SubByte(uint32_t in)
{
    uint32_t res;
    uint8_t arr[4];
    toBytes(in, arr);
    res = toWord(S_Box[arr[0]], S_Box[arr[1]], S_Box[arr[2]], S_Box[arr[3]]);
    return res;
}

uint32_t BaseAESImpl::RotByte(uint32_t in)
{
    uint32_t res;
    uint8_t arr[4];
    toBytes(in, arr);
    res = toWord(arr[1], arr[2], arr[3], arr[0]);
    return res;
}

void AES128Impl::KeyExpansion()
{
    for (int i = 0; i < Nk; ++i) {
        w[i] = toWord(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    }
    for (int i = Nk; i < Nb * (Nr + 1); ++i) {
        auto temp = w[i - 1];
        if (i % Nk == 0) {
            temp = SubByte(RotByte(temp)) ^ Rcon[i / Nk];
        }
        w[i] = w[i - Nk] ^ temp;
    }
}

void AES128Impl::Encrypt()
{
    clear();
    for (int i = 0; i < 4 * Nb; ++i) {
        state[i] = plain[i];
    }
    KeyExpansion();
    for (int i = 0; i <= Nr; ++i) {
        if (i > 0) {
            ByteSub();
            ShiftRow();
            if (i < Nr) {
                MixColumn();
            }
        }
        AddRoundKey(w, i);
    }
    for (int i = 0; i < 4 * Nb; ++i) {
        cipher[i] = state[i];
    }
}

void AES128Impl::Decrypt()
{
    clear();
    for (int i = 0; i < 4 * Nb; ++i) {
        state[i] = cipher[i];
    }
    KeyExpansion();
    for (int i = Nr; i >= 0; --i) {
        AddRoundKey(w, i);
        if (i > 0) {
            if (i < Nr) {
                InvMixColumn();
            }
            InvShiftRow();
            InvByteSub();
        }
    }
    for (int i = 0; i < 4 * Nb; ++i) {
        plain[i] = state[i];
    }
}

void AES192Impl::KeyExpansion()
{
    for (int i = 0; i < Nk; ++i) {
        w[i] = toWord(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    }
    for (int i = Nk; i < Nb * (Nr + 1); ++i) {
        auto temp = w[i - 1];
        if (i % Nk == 0) {
            temp = SubByte(RotByte(temp)) ^ Rcon[i / Nk];
        }
        w[i] = w[i - Nk] ^ temp;
    }
}

void AES192Impl::Encrypt()
{
    clear();
    for (int i = 0; i < 4 * Nb; ++i) {
        state[i] = plain[i];
    }
    KeyExpansion();
    for (int i = 0; i <= Nr; ++i) {
        if (i > 0) {
            ByteSub();
            ShiftRow();
            if (i < Nr) {
                MixColumn();
            }
        }
        AddRoundKey(w, i);
    }
    for (int i = 0; i < 4 * Nb; ++i) {
        cipher[i] = state[i];
    }
}

void AES192Impl::Decrypt()
{
    clear();
    for (int i = 0; i < 4 * Nb; ++i) {
        state[i] = cipher[i];
    }
    KeyExpansion();
    for (int i = Nr; i >= 0; --i) {
        AddRoundKey(w, i);
        if (i > 0) {
            if (i < Nr) {
                InvMixColumn();
            }
            InvShiftRow();
            InvByteSub();
        }
    }
    for (int i = 0; i < 4 * Nb; ++i) {
        plain[i] = state[i];
    }
}

void AES256Impl::KeyExpansion()
{
    for (int i = 0; i < Nk; ++i) {
        w[i] = toWord(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    }
    for (int i = Nk; i < Nb * (Nr + 1); ++i) {
        auto temp = w[i - 1];
        if (i % Nk == 0) {
            temp = SubByte(RotByte(temp)) ^ Rcon[i / Nk];
        } else if (i % Nk == 4) {
            temp = SubByte(temp);
        }
        w[i] = w[i - Nk] ^ temp;
    }
}

void AES256Impl::Encrypt()
{
    clear();
    for (int i = 0; i < 4 * Nb; ++i) {
        state[i] = plain[i];
    }
    KeyExpansion();
    for (int i = 0; i <= Nr; ++i) {
        if (i > 0) {
            ByteSub();
            ShiftRow();
            if (i < Nr) {
                MixColumn();
            }
        }
        AddRoundKey(w, i);
    }
    for (int i = 0; i < 4 * Nb; ++i) {
        cipher[i] = state[i];
    }
}

void AES256Impl::Decrypt()
{
    clear();
    for (int i = 0; i < 4 * Nb; ++i) {
        state[i] = cipher[i];
    }
    KeyExpansion();
    for (int i = Nr; i >= 0; --i) {
        AddRoundKey(w, i);
        if (i > 0) {
            if (i < Nr) {
                InvMixColumn();
            }
            InvShiftRow();
            InvByteSub();
        }
    }
    for (int i = 0; i < 4 * Nb; ++i) {
        plain[i] = state[i];
    }
}
}