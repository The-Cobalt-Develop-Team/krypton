#pragma once

#include "Utilities.hpp"
#include <cstddef>
#include <memory>
#include <variant>
#include <vector>

namespace Krypton::ASN1 {

enum ASN1Type {
    // TODO: ASN1Type enum
};

struct ASN1Tag {
    byte classTag;
    byte constructed;
    size_t number;
};

class ASN1Node : public std::enable_shared_from_this<ASN1Node> {
public:
    ASN1Node() = default;
    ASN1Node(ASN1Tag t, size_t l, const ByteArray& b)
        : tag(t)
        , len(l)
        , content(b)
    {
    }
    ASN1Node(ASN1Tag t, size_t l)
        : tag(t)
        , len(l)
        , content()
    {
    }
    using ContainerType = std::vector<std::shared_ptr<ASN1Node>>;
    ASN1Node(ASN1Tag t, size_t l, ContainerType c)
        : tag(t)
        , len(l)
        , content(c)
    {
    }
    ASN1Tag tag {};
    size_t len = 0;
    std::variant<ByteArray, ContainerType> content;
};
using ASN1NodePtr = std::shared_ptr<ASN1Node>;

namespace Detail {
    constexpr const size_t kIndefiniteLen = static_cast<size_t>(-1);
    inline size_t decodeLen(ByteArrayStream& bs)
    {
        size_t buf = bs.getc(), len = buf & 0x7f;
        if (len == buf)
            return len;
        if (len == 0)
            return kIndefiniteLen;
        buf = 0;
        for (auto i = 0; i < len; ++i)
            buf = (buf << 8) + bs.getc();
        return buf;
    }
    inline ASN1Tag decodeTag(ByteArrayStream& bs)
    {
        auto buf = bs.getc();
        ASN1Tag res {};
        res.classTag = buf >> 6;
        res.constructed = ((buf & 0x20) != 0);
        res.number = buf & 0x1f;
        // TODO: long tag
        /*
        if (this.tagNumber == 0x1F) { // long tag
            let n = new Int10();
            do {
                buf = stream.get();
                n.mulAdd(128, buf & 0x7F);
            } while (buf & 0x80);
            this.tagNumber = n.simplify();
        }
        */
        return res;
    }
    bool isUniversal(const ASN1Tag& tag) { return tag.classTag == 0x00; }
    bool isEOC(const ASN1Tag& tag) { return tag.classTag == 0x00 && tag.number == 0x00; }
}

ASN1NodePtr ASN1Decode(ByteArrayStream& bs)
{
    auto start = bs.offset();
    auto tag = Detail::decodeTag(bs);
    auto taglen = bs.offset() - start;
    auto len = Detail::decodeLen(bs);
    auto content_start = bs.offset();
    ASN1Node::ContainerType container;
    ByteArray buf;
    auto getSub = [&]() {
        if (len != Detail::kIndefiniteLen) {
            // TODO: check if stream has len elements
            while (bs.offset() - start < len) {
                auto res = ASN1Decode(bs);
                container.emplace_back(res);
            }
        } else {
            while (true) {
                auto res = ASN1Decode(bs);
                if (Detail::isEOC(res->tag))
                    break;
                container.emplace_back(res);
            }
        }
    };
    if (tag.constructed) {
        getSub();
    } else {
        bs.get(buf, len);
    }
    if (container.empty())
        return std::make_shared<ASN1Node>(tag, len, container);
    else
        return std::make_shared<ASN1Node>(tag, len, buf);
    // else if (Detail::isUniversal(tag) && (tag.number == 0x03 || tag.number == 0x04)) {
    // if (!(tag.number == 0x03 && bs.getc() != 0))
    // getSub();
    // }
}

}
