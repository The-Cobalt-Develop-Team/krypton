#include "ASN1.hpp"

namespace Krypton::ASN1Wrapper {

bool ASN1Constructor::start_constructed(asn1::ber::tag_class tc, asn1::ber::tag_number tn, uint64_t valuelen, uint64_t totallen)
{
    return true;
}

bool ASN1Constructor::end_constructed(asn1::ber::tag_class tc, asn1::ber::tag_number tn, uint64_t totallen)
{
    return true;
}

bool ASN1Constructor::boolean(const void* buf, uint64_t len, bool val)
{
    return true;
}

bool ASN1Constructor::integer(const void* buf, uint64_t len, int64_t val)
{
    return true;
}

bool ASN1Constructor::null()
{
    return true;
}

bool ASN1Constructor::oid(const void* buf, uint64_t len, const uint64_t* oid, size_t ncomponents)
{
    return true;
}

bool ASN1Constructor::real(const void* buf, uint64_t len, double val)
{
    return true;
}

bool ASN1Constructor::enumerated(const void* buf, uint64_t len, int64_t val)
{
    return true;
}

bool ASN1Constructor::utc_time(const void* buf, uint64_t len, time_t val)
{
    return true;
}

bool ASN1Constructor::generalized_time(const void* buf, uint64_t len, const struct timeval& val)
{
    return true;
}

bool ASN1Constructor::primitive(asn1::ber::tag_class tc, asn1::ber::tag_number tn, const void* buf, uint64_t len, uint64_t valueoff, uint64_t valuelen)
{
    return true;
}

void ASN1Constructor::error(asn1::ber::error e, uint64_t offset, const char* msg = nullptr) { }

}
