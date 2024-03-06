#pragma once

#include "Utilities.hpp"
#include "asn1/ber/common.h"
#include "asn1/ber/decoder.h"
#include "asn1/ber/encoder.h"
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <inttypes.h>

#ifdef _WIN32
#include <Windows.h>
#include <winsock.h>
#define timegm _mkgmtime
#define gmtime_r(x, y) gmtime_s(y, x)
#endif

namespace Krypton {
namespace ASN1Wrapper {
    using namespace asn1;

    class ASN1Constructor {
    public:
        ASN1Constructor() = default;
        ~ASN1Constructor() = default;
        bool start_constructed(asn1::ber::tag_class tc, asn1::ber::tag_number tn, uint64_t valuelen, uint64_t totallen);
        bool end_constructed(asn1::ber::tag_class tc, asn1::ber::tag_number tn, uint64_t totallen);
        bool boolean(const void* buf, uint64_t len, bool val);
        bool integer(const void* buf, uint64_t len, int64_t val);
        bool null();
        bool oid(const void* buf, uint64_t len, const uint64_t* oid, size_t ncomponents);
        bool real(const void* buf, uint64_t len, double val);
        bool enumerated(const void* buf, uint64_t len, int64_t val);
        bool utc_time(const void* buf, uint64_t len, time_t val);
        bool generalized_time(const void* buf, uint64_t len, const struct timeval& val);
        bool primitive(asn1::ber::tag_class tc, asn1::ber::tag_number tn, const void* buf, uint64_t len, uint64_t valueoff, uint64_t valuelen);
        void error(asn1::ber::error e, uint64_t offset, const char* msg = nullptr);

    private:
        int depth;
        asn1::ber::tag_class tc;
    };

    class asn1_object {
    public:
        // Constructor.
        asn1_object() = default;

        // Destructor.
        ~asn1_object() = default;

        // Set initial offset.
        void initial_offset(size_t offset)
        {
            _M_initial_offset = offset;
        }

        // Start constructed.
        bool start_constructed(asn1::ber::tag_class tc,
            asn1::ber::tag_number tn,
            uint64_t valuelen,
            uint64_t totallen)
        {
            indent();

            if (valuelen != asn1::ber::decoder::indefinite_length) {
                // Universal class?
                if (tc == asn1::ber::tag_class::Universal) {
                    printf("%s: %s, header length (TL): %llu, total length (TLV): %llu\n",
                        to_string(tc),
                        to_string(static_cast<asn1::ber::universal_class>(tn)),
                        totallen - valuelen,
                        totallen);
                } else {
                    printf("%s: %llu, header length (TL): %llu, total length (TLV): %llu\n",
                        to_string(tc),
                        tn,
                        totallen - valuelen,
                        totallen);
                }
            } else {
                // Universal class?
                if (tc == asn1::ber::tag_class::Universal) {
                    printf("%s: %s, indefinite length\n",
                        to_string(tc),
                        to_string(static_cast<asn1::ber::universal_class>(tn)));
                } else {
                    printf("%s: %llu, indefinite length\n", to_string(tc), tn);
                }
            }

            indent();
            printf("{\n");

            _M_depth++;

            return true;
        }

        // End constructed.
        bool end_constructed(asn1::ber::tag_class tc,
            asn1::ber::tag_number tn,
            uint64_t totallen)
        {
            if (_M_depth > 0) {
                _M_depth--;

                indent();
                printf("} /* total length (TLV): %llu */\n", totallen);

                return true;
            } else {
                fprintf(stderr, "End-of-constructed while having depth 0.\n");
                return false;
            }
        }

        // Boolean.
        bool boolean(const void* buf, uint64_t len, bool val)
        {
            print("Boolean", buf, len, "%s", val ? "true" : "false");

            return true;
        }

        // Integer.
        bool integer(const void* buf, uint64_t len, int64_t val)
        {
            print("Integer", buf, len, "%ld", val);

            return true;
        }

        // Null.
        bool null()
        {
            indent();
            printf("[Null]\n");

            return true;
        }

        // Object identifier.
        bool oid(const void* buf,
            uint64_t len,
            const uint64_t* oid,
            size_t ncomponents)
        {
            indent();

            for (size_t i = 0; i < ncomponents; i++) {
                printf("%s%" PRIu64, (i > 0) ? "." : "", oid[i]);
            }

            printf("\n");

            return true;
        }

        // Real.
        bool real(const void* buf, uint64_t len, double val)
        {
            print("Real", buf, len, "%e", val);

            return true;
        }

        // Enumerated.
        bool enumerated(const void* buf, uint64_t len, int64_t val)
        {
            print("Enumerated", buf, len, "%ld", val);

            return true;
        }

        // UTC time.
        bool utc_time(const void* buf, uint64_t len, time_t val)
        {
            struct tm tm;
            gmtime_r(&val, &tm);

            print("UTC time",
                buf,
                len,
                "%04u/%02u/%02u %02u:%02u:%02u",
                1900 + tm.tm_year,
                1 + tm.tm_mon,
                tm.tm_mday,
                tm.tm_hour,
                tm.tm_min,
                tm.tm_sec);

            return true;
        }

        // Generalized time.
        bool generalized_time(const void* buf,
            uint64_t len,
            const struct timeval& val)
        {
            // struct tm tm;
            // gmtime_r(&val.tv_sec, &tm);

            // print("Generalized time",
            //     buf,
            //     len,
            //     "%04u/%02u/%02u %02u:%02u:%02u.%u",
            //     1900 + tm.tm_year,
            //     1 + tm.tm_mon,
            //     tm.tm_mday,
            //     tm.tm_hour,
            //     tm.tm_min,
            //     tm.tm_sec,
            //     val.tv_usec);

            // return true;
        }

        // Primitive.
        bool primitive(asn1::ber::tag_class tc,
            asn1::ber::tag_number tn,
            const void* buf,
            uint64_t len,
            uint64_t valueoff,
            uint64_t valuelen)
        {
            // Beginning of the primitive value?
            if (valueoff == 0) {
                indent();

                // Universal class?
                if (tc == asn1::ber::tag_class::Universal) {
                    printf("[Primitive] Tag class: %s, tag: %s\n",
                        to_string(tc),
                        to_string(static_cast<asn1::ber::universal_class>(tn)));
                } else {
                    printf("[Primitive] Tag class: %s, tag number: %llu\n",
                        to_string(tc),
                        tn);
                }
            } else {
                indent();
                spaces(indent_size);
                printf("=================================================\n");
            }

            indent();
            spaces(indent_size);

            printf("Value %llu-%llu/%llu:\n", valueoff + 1, valueoff + len, valuelen);

            ascii_dump(buf, len);

            printf("\n");

            indent();
            spaces(indent_size);

            printf("Hexadecimal %llu-%llu/%llu:\n",
                valueoff + 1,
                valueoff + len,
                valuelen);

            hexdump(buf, len);

            return true;
        }

        // Error.
        void error(asn1::ber::error e, uint64_t offset, const char* msg = nullptr)
        {
            if (msg) {
                fprintf(stderr,
                    "Error: %s, at offset: %llu, message: '%s'.\n",
                    to_string(e),
                    _M_initial_offset + offset,
                    msg);
            } else {
                fprintf(stderr,
                    "Error: %s, at offset: %llu.\n",
                    to_string(e),
                    _M_initial_offset + offset);
            }
        }

    private:
        static const size_t indent_size = 2;

        static const size_t number_hex_chars_per_line = 16;
        static const size_t
            number_ascii_chars_per_line
            = (number_hex_chars_per_line * 3) - 1;

        size_t _M_depth = 0;

        size_t _M_initial_offset;

        // Print.
        void print(const char* type,
            const void* buf,
            uint64_t len,
            const char* format,
            ...) const
        {
            indent();
            printf("[%s] Length: %llu\n", type, len);

            indent();
            spaces(indent_size);
            printf("Value:\n");

            indent();
            spaces(2 * indent_size);

            va_list ap;
            va_start(ap, format);

            vprintf(format, ap);

            va_end(ap);

            printf("\n\n");

            indent();
            spaces(indent_size);
            printf("Hexadecimal:\n");

            hexdump(buf, len);
        }

        // Hexadecimal dump.
        void hexdump(const void* buf, size_t len) const
        {
            const uint8_t* const b = static_cast<const uint8_t*>(buf);

            for (size_t i = 0; i < len; i++) {
                if ((i % number_hex_chars_per_line) == 0) {
                    if (i > 0) {
                        printf("\n");
                    }

                    indent();
                    spaces(2 * indent_size);

                    printf("%02x", b[i]);
                } else {
                    printf(" %02x", b[i]);
                }
            }

            printf("\n");
        }

        // ASCII dump.
        void ascii_dump(const void* buf, size_t len) const
        {
            const char* const b = static_cast<const char*>(buf);

            for (size_t i = 0; i < len; i++) {
                if ((i % number_ascii_chars_per_line) == 0) {
                    if (i > 0) {
                        printf("\n");
                    }

                    indent();
                    spaces(2 * indent_size);
                }

                if (isprint(b[i])) {
                    printf("%c", b[i]);
                } else {
                    printf(".");
                }
            }

            printf("\n");
        }

        // Indent.
        void indent() const
        {
            spaces(_M_depth * indent_size);
        }

        // Write spaces.
        static void spaces(size_t count)
        {
            for (size_t i = 0; i < count; i++) {
                printf(" ");
            }
        }
    };
}
using ASN1Object = ASN1Wrapper::asn1_object;
using ASN1Decoder = asn1::ber::decoder;
template <size_t number_static_values = 128, size_t max_values = ULONG_MAX>
using ASN1Encoder = asn1::ber::encoder<number_static_values, max_values>;

}
