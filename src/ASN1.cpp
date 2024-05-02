#include "ASN1.hpp"

namespace Krypton::ASN1 {

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
    if (!container.empty())
        return std::make_shared<ASN1Node>(tag, len, container);
    else
        return std::make_shared<ASN1Node>(tag, len, buf);
    // else if (Detail::isUniversal(tag) && (tag.number == 0x03 || tag.number == 0x04)) {
    // if (!(tag.number == 0x03 && bs.getc() != 0))
    // getSub();
    // }
}

void ASN1Node::dump(std::ostream& os)
{
    os << std::format("Tag: {:#04x} {:#04x} {:#04x}", tag.classTag, tag.constructed, tag.number) << std::endl;
    os << std::format("Len: {}", len) << std::endl;
    if (content.index() == 0) {
        os << toHex(this->bytes()) << std::endl;
    } else {
        os << '[' << std::endl;
        for (const auto& ptr : this->sub()) {
            os << '{' << std::endl;
            ptr->dump(os);
            os << "}, " << std::endl;
        }
        os << "]" << std::endl;
    }
}

}
