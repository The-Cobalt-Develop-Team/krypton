#pragma once

#include "AES.hpp"
#include "Utilities.hpp"
#include "range/v3/action/push_back.hpp"
#include "range/v3/iterator/unreachable_sentinel.hpp"
#include "range/v3/view/subrange.hpp"
#include <array>
#include <exception>
#include <range/v3/action.hpp>
#include <range/v3/view.hpp>

namespace Krypton {
namespace Detail {
    using namespace ::ranges;

    struct BlocksizeError : std::exception {
        [[nodiscard]] const char* what() const noexcept override
        {
            return "BlockLenthError Occurred";
        }
    };
    template <typename Derived, typename CIPHCtx = AES256Context>
    class BaseBlockCipherMode {
    public:
        static constexpr const size_t kBlockSize = CIPHCtx::kBlockSize;

        ByteArray getBuffer() { return buffer_; }
        ByteArray encrypt(const ByteArray& buf)
        {
            if (buf.size() % kBlockSize != 0)
                throw BlocksizeError();
            auto ptr = reinterpret_cast<const uint8_t*>(buf.data());
            for (size_t offset = 0; offset < buf.size(); offset += kBlockSize) {
                static_cast<Derived*>(this)->encryptUpdate(ptr + offset);
            }
            return buffer_;
        }
        ByteArray decrypt(const ByteArray& buf)
        {
            if (buf.size() % kBlockSize != 0)
                throw BlocksizeError();
            auto ptr = reinterpret_cast<const uint8_t*>(buf.data());
            for (size_t offset = 0; offset < buf.size(); offset += kBlockSize) {
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
            this->ctx_.setPlain(ByteArray(ptr, ptr + this->kBlockSize));
            this->ctx_.encrypt();
            ranges::actions::push_back(this->buffer_, this->ctx_.getCipher());
        }
        void decryptUpdate(const uint8_t* ptr)
        {
            this->ctx_.setCipher(ByteArray(ptr, ptr + this->kBlockSize));
            this->ctx_.decrypt();
            ranges::actions::push_back(this->buffer_, this->ctx_.getPlain());
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
            if (temp_.size() != this->kBlockSize)
                throw BlocksizeError();
            auto pl = baxor(temp_, ByteArray(ptr, ptr + this->kBlockSize));
            this->ctx_.setPlain(pl);
            this->ctx_.encrypt();
            temp_ = this->ctx_.getCipher();
            actions::push_back(this->buffer_, temp_);
        }

        void decryptUpdate(const uint8_t* ptr)
        {
            auto ciph = ByteArray(ptr, ptr + this->kBlockSize);
            this->ctx_.setCipher(ciph);
            this->ctx_.decrypt();
            auto pl = baxor(temp_, this->ctx_.getPlain());
            temp_ = ciph;
            actions::push_back(this->buffer_, pl);
        }

    private:
        ByteArray iv_;
        ByteArray temp_;
    };

    // TODO: CFB with segment size of 1bit
    template <typename CIPHCtx>
    class CFBContext : public BaseBlockCipherMode<CFBContext<CIPHCtx>, CIPHCtx> {
    public:
        void setSegment(size_t seg) { seg_ = seg; }
        void setIV(const ByteArray& iv)
        {
            iv_ = iv;
            temp_ = iv_;
        }
        void encryptUpdate(const uint8_t* ptr)
        {
            auto ciph = this->ctx_.setPlain(temp_).encrypt().getCipher();
            auto bptr = reinterpret_cast<const byte*>(ptr);
            ciph = baxor(ByteArray(ciph.data(), ciph.data() + seg_), ByteArray(bptr, bptr + seg_));
            actions::push_back(this->buffer_, ciph);
            temp_ = views::concat(temp_ | views::slice(seg_, this->kBlockSize), ciph) | ranges::to<ByteArray>();
        }
        void decryptUpdate(const uint8_t* ptr)
        {
            auto ciph = this->ctx_.setPlain(temp_).encrypt().getCipher();
            auto bptr = reinterpret_cast<const byte*>(ptr);
            ciph = baxor(ByteArray(ciph.data(), ciph.data() + seg_), ByteArray(bptr, bptr + seg_));
            actions::push_back(this->buffer_, ciph);
            temp_ = views::concat(temp_ | views::slice(seg_, this->kBlockSize), ranges::subrange(bptr, ranges::unreachable_sentinel_t {}) | views::take(seg_)) | ranges::to<ByteArray>();
        }
        ByteArray encrypt(const ByteArray& buf)
        {
            auto ptr = reinterpret_cast<const uint8_t*>(buf.data());
            for (size_t offset = 0; offset < buf.size(); offset += seg_) {
                encryptUpdate(ptr + offset);
            }
            return this->buffer_;
        }
        ByteArray decrypt(const ByteArray& buf)
        {
            auto ptr = reinterpret_cast<const uint8_t*>(buf.data());
            for (size_t offset = 0; offset < buf.size(); offset += seg_) {
                decryptUpdate(ptr + offset);
            }
            return this->buffer_;
        }

    private:
        size_t seg_;
        ByteArray iv_;
        ByteArray temp_;
    };
    template <typename CIPHCtx>
    class OFBContext : public BaseBlockCipherMode<OFBContext<CIPHCtx>, CIPHCtx> {
    public:
        void setIV(const ByteArray& iv)
        {
            iv_ = iv;
            temp_ = iv_;
        }

        void encryptUpdate(const uint8_t* ptr)
        {
            temp_ = this->ctx_.setPlain(temp_).encrypt().getCipher();
            actions::push_back(this->buffer_, baxor(temp_, ByteArray(ptr, ptr + this->kBlockSize)));
        }

        void decryptUpdate(const uint8_t* ptr)
        {
            temp_ = this->ctx_.setPlain(temp_).encrypt().getCipher();
            actions::push_back(this->buffer_, baxor(temp_, ByteArray(ptr, ptr + this->kBlockSize)));
        }

    private:
        ByteArray iv_;
        ByteArray temp_;
    };

    template <typename CIPHCtx>
    class CTRContext : public BaseBlockCipherMode<CTRContext<CIPHCtx>, CIPHCtx> {
    public:
        void setNonce(const ByteArray& nonce) { counter_ = nonce; }
        void encryptUpdate(const uint8_t* ptr)
        {
            temp_ = this->ctx_.setPlain(counter_).encrypt().getCipher();
            // this->buffer_.append(baxor(temp_, ByteArray(ptr, ptr + this->kBlockSize)));
            // this->buffer_ = ranges::views::single(this->buffer_) | ranges::views::join(baxor(temp_, ByteArray(ptr, ptr + this->kBlockSize))) | ranges::to<ByteArray>;
            actions::push_back(this->buffer_, baxor(temp_, ByteArray(ptr, ptr + this->kBlockSize)));
            this->incCounter();
        }

        void decryptUpdate(const uint8_t* ptr)
        {
            temp_ = this->ctx_.setPlain(counter_).encrypt().getCipher();
            actions::push_back(this->buffer_, baxor(temp_, ByteArray(ptr, ptr + this->kBlockSize)));
            this->incCounter();
        }

    private:
        ByteArray counter_;
        ByteArray temp_;
        static constexpr const size_t kCtrSize = CIPHCtx::kBlockSize / 2;
        void incCounter()
        {
            for (size_t i = 0; i < kCtrSize; ++i) {
                ++this->counter_[counter_.size() - i - 1];
                if (this->counter_[counter_.size() - i - 1] != 0x00)
                    break;
            }
        }
    };
}
}