/*
 * md5.cpp
 *
 *  Created on: 10.06.2012
 *      Author: kreyl
 */

#include "md5.h"
#include <stdint.h>


struct MD5Context {
    uint32_t buf[4];
    uint32_t bits[2];
    unsigned char in[64];
};

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void MD5Init(ctx)
    struct MD5Context *ctx;
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;

    ctx->bits[0] = 0;
    ctx->bits[1] = 0;
}




void md5(char *S) {

}


