#include "ASN1.hpp"
#include "Utilities.hpp"
#include <gtest/gtest.h>

using namespace Krypton;

TEST(ASN1Test, ASN1WrapperTest1)
{
    // this is a test private key of RSA
    ByteArray ba = fromBase64("MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAMapphwPexvwcO91oxbaGThx0tqEIicnlIEaEm7RK1gBbJQ5lIfzqcEg3dABrLrExwUDgshWHLcaNNWcjAZZg/mza12U5jvuES2OD2Z+JA5qztKMCPJixBMtcH6K+ERYyyQbFX3IUFyzM55YBKpFeUNgFhqlj8GKhY5AH5cFf2IXAgMBAAECgYARIghlWyBu5GmY/Tg3fE+4gniAx+x6G/u+tpToYbmvOVA36E9a+jSlp0P58GsJIHdZg1gAFw0a7XOpCHs7APVUqKkJRZso3j4PIxIAiajjKvFG3c+DP6G8svnniEI1UIQhct5SLHylowCJlT8/RSvSKE1r0c4ZtcloPOpfwFwxYQJBAOuiITqL3JcU1lKfDaoyktlHlRKTa0kq7MWLzoY+mxiwU2xY5TAk4OiGOjx2RKt/+gHJiepKXFTORkWzZYI0FCMCQQDX1XfW+1MjN+KBI7pQtUuQmi56cDnG8BlUNWC/Qabz8iScoTEENBVxnzuf9Rl9NUkufO76O6JFVkLM1Fa2Bo99AkEAu2hrvUDX5WGxRG00VRV8Lkv8iJWvUrZNMqELVUId4Pj8oMPjJPNv3gBD+SqnWy+iDCHbCvixjtGwmmcgaPo6uQJAdQSbsMcaUCHggDuqrE9e5MtzH5bw/Fb4pJs8FPW3WtOR3gnW1XyXk77NSPBOHYBiaP37Y1gppoYaOYt2Z8Iu0QJBANPw9RG5ygJSl3LK2FtB1IiZZycFTsaIFbd1I7GGtIgatboccugUgpVh641WjBW7dxLoAddBO5pygGj6A53xqvg=");
    ByteArrayStream bas(ba);
    auto res = *ASN1::ASN1Decode(bas);
    // res.dump(std::cout);
    ASSERT_EQ(res[1][0].bytes(), fromHex("2A864886F70D010101"));
}
