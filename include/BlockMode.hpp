#pragma once

#include "AES.hpp"
#include <array>
#include <exception>

namespace Krypton {
namespace Detail {
    struct BlockLengthError : std::exception {
        [[nodiscard]] const char* what() const noexcept override
        {
            return "BlockLenthError Occurred";
        }
    };
    template <typename Derived, typename CIPHCtx = AES256Context>
    class BaseBlockCipherMode {
    public:
        static constexpr const int kBlockSize = CIPHCtx::kBlockSize;

        ByteArray getBuffer() { return buffer_; }
        ByteArray encrypt(const ByteArray& buf)
        {
            if (buf.length() % kBlockSize != 0)
                throw BlockLengthError();
            auto ptr = reinterpret_cast<const uint8_t*>(buf.data());
            for (size_t offset = 0; offset < buf.length(); offset += kBlockSize) {
                static_cast<Derived*>(this)->encryptUpdate(ptr + offset);
            }
            return buffer_;
        }
        ByteArray decrypt(const ByteArray& buf)
        {
            if (buf.length() % kBlockSize != 0)
                throw BlockLengthError();
            auto ptr = reinterpret_cast<const uint8_t*>(buf.data());
            for (size_t offset = 0; offset < buf.length(); offset += kBlockSize) {
                static_cast<Derived*>(this)->decryptUpdate(ptr + offset);
            }
            return buffer_;
        }
        void setKey(const ByteArray& key) { ctx_.setKey(key); }

    protected:
        ByteArray buffer_;
        CIPHCtx ctx_;
    };
    template <typename CIPHCtx>
    class ECBContext : public BaseBlockCipherMode<ECBContext<CIPHCtx>, CIPHCtx> {
    public:
        // Note: memory between ptr and ptr+kBlockSize must be allocated
        void encryptUpdate(const uint8_t* ptr)
        {
            this->ctx_.setPlain(ByteArray(reinterpret_cast<const byte*>(ptr), this->kBlockSize));
            this->ctx_.encrypt();
            this->buffer_.append(this->ctx_.getCipher());
        }
        void decryptUpdate(const uint8_t* ptr)
        {
            this->ctx_.setCipher(ByteArray(reinterpret_cast<const byte*>(ptr), this->kBlockSize));
            this->ctx_.decrypt();
            this->buffer_.append(this->ctx_.getPlain());
        }
    };
    template <typename CIPHCtx>
    class CBCContext : public BaseBlockCipherMode<CBCContext<CIPHCtx>, CIPHCtx> {
    public:
        void setIV(const ByteArray& iv)
        {
            iv_ = iv;
            temp_ = iv_;
        }

        void encryptUpdate(const uint8_t* ptr)
        {
            if (temp_.length() != this->kBlockSize)
                throw BlockLengthError();
            auto pl = baxor(temp_, ByteArray(reinterpret_cast<const byte*>(ptr), this->kBlockSize));
            this->ctx_.setPlain(pl);
            this->ctx_.encrypt();
            temp_ = this->ctx_.getCipher();
            this->buffer_.append(temp_);
        }

        void decryptUpdate(const uint8_t* ptr)
        {
            auto ciph = ByteArray(reinterpret_cast<const byte*>(ptr), this->kBlockSize);
            this->ctx_.setCipher(ciph);
            this->ctx_.decrypt();
            auto pl = baxor(temp_, this->ctx_.getPlain());
            temp_ = ciph;
            this->buffer_.append(pl);
        }

    private:
        ByteArray iv_;
        ByteArray temp_;
    };
}
}