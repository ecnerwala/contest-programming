#include <bits/stdc++.h>

// Source: https://judge.yosupo.jp/submission/153427

#if 1 // type
// clang-format off
typedef   int8_t      i8;
typedef   int16_t     i16;
typedef   int32_t     i32;
typedef   int64_t     i64;
typedef __int128_t    i128;
typedef   uint8_t     u8;
typedef   uint16_t    u16;
typedef   uint32_t    u32;
typedef   uint64_t    u64;
typedef __uint128_t   u128;
typedef   float       f32;
typedef   double      f64;
typedef struct { i32 a, b; } Pair_i32;
typedef struct { i64 a, b; } Pair_i64;
typedef struct { u32 a, b; } Pair_u32;
typedef struct { u64 a, b; } Pair_u64;
typedef struct { i32 a, b, c; } Triple_i32;
typedef struct { i64 a, b, c; } Triple_i64;
typedef struct { u32 a, b, c; } Triple_u32;
typedef struct { u64 a, b, c; } Triple_u64;
typedef struct { i32 a, b, c, d; } Quadruple_i32;
typedef struct { i64 a, b, c, d; } Quadruple_i64;
typedef struct { u32 a, b, c, d; } Quadruple_u32;
typedef struct { u64 a, b, c, d; } Quadruple_u64;
// clang-format on
#endif // type

#if 1 // bitop
#define ctz32(a) ((a) ? __builtin_ctz((a)) : (32))
#define clz32(a) ((a) ? __builtin_clz((a)) : (32))
#define pct32(a) __builtin_popcount((a))
#define msb32(a) ((a) ? ((31) - __builtin_clz((a))) : (0))
#define bit_width32(a) ((a) ? ((32) - __builtin_clz((a))) : (0))
#define bit_ceil32(a) ((!(a)) ? (1) : ((pct32(a)) == (1) ? ((1u) << ((31) - clz32((a)))) : ((1u) << ((32) - clz32(a)))))
#define bit_floor32(a) ((!(a)) ? (0) : ((1u) << ((31) - clz32((a)))))
#define ctz64(a) ((a) ? __builtin_ctzll((a)) : (64))
#define clz64(a) ((a) ? __builtin_clzll((a)) : (64))
#define pct64(a) __builtin_popcountll((a))
#define msb64(a) ((a) ? ((63) - __builtin_clzll((a))) : (0))
#define bit_width64(a) ((a) ? ((64) - __builtin_clzll((a))) : (0))
#define bit_ceil64(a) ((!(a)) ? (1) : ((pct64(a)) == (1) ? ((1ull) << ((63) - clz64((a)))) : ((1ull) << ((64) - clz64(a)))))
#define bit_floor64(a) ((!(a)) ? (0) : ((1ull) << ((63) - clz64((a)))))
#define flip_Nbit(a, n) ((a) ^ ((1) << (n)))
#define only_lsb(a) ((a) & (-(a)))
#define bit_reverse32(v) ((((((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) & 0x0f0f0f0f) << 4 | ((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) >> 4 & 0x0f0f0f0f)) & 0x00ff00ff) << 8 | ((((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) & 0x0f0f0f0f) << 4 | ((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) >> 4 & 0x0f0f0f0f)) >> 8 & 0x00ff00ff)) & 0x0000ffff) << 16 | ((((((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) & 0x0f0f0f0f) << 4 | ((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) >> 4 & 0x0f0f0f0f)) & 0x00ff00ff) << 8 | ((((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) & 0x0f0f0f0f) << 4 | ((((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) & 0x33333333) << 2 | ((((v)&0x55555555) << 1 | ((v) >> 1 & 0x55555555)) >> 2 & 0x33333333)) >> 4 & 0x0f0f0f0f)) >> 8 & 0x00ff00ff)) >> 16 & 0x0000ffff)
#define bit_reverse64(v) ((((((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) & 0x00ff00ff00ff00ff) << 8 | ((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) >> 8 & 0x00ff00ff00ff00ff)) & 0x0000ffff0000ffff) << 16 | ((((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) & 0x00ff00ff00ff00ff) << 8 | ((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) >> 8 & 0x00ff00ff00ff00ff)) >> 16 & 0x0000ffff0000ffff)) & 0x00000000ffffffff) << 32 | ((((((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) & 0x00ff00ff00ff00ff) << 8 | ((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) >> 8 & 0x00ff00ff00ff00ff)) & 0x0000ffff0000ffff) << 16 | ((((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) & 0x00ff00ff00ff00ff) << 8 | ((((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) & 0x0f0f0f0f0f0f0f0f) << 4 | ((((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) & 0x3333333333333333) << 2 | ((((v)&0x5555555555555555) << 1 | ((v) >> 1 & 0x5555555555555555)) >> 2 & 0x3333333333333333)) >> 4 & 0x0f0f0f0f0f0f0f0f)) >> 8 & 0x00ff00ff00ff00ff)) >> 16 & 0x0000ffff0000ffff)) >> 32 & 0x00000000ffffffff)
#define rotr32(x, r) ((r) < (0) ? (((x) << ((u32)(((u64)(-(r)) % (32))))) | ((x) >> ((-(((u32)(((u64)(-(r)) % (32)))))) & 31))) : (((x) >> ((r) % (32))) | ((x) << ((-(r)) & 31))))
#define rotl32(x, r) (rotr32((x), (-(r))))
#define rotr64(x, r) ((r) < (0) ? (((x) << (((u64)(-(r)) % (64)))) | ((x) >> ((-((((u64)(-(r)) % (64))))) & 63))) : (((x) >> ((r) % (64))) | ((x) << ((-(r)) & 63))))
#define rotl64(x, r) (rotr64((x), (-(r))))
#endif // bitop

#if defined(ONLINE) // ONLINE-IO
#if 1               // ONLINE-INPUT
static char *input_data;
static struct stat input_stat;
static int input_size;
__attribute__((constructor)) void _constructor_read_(void) {
    fstat(0, &input_stat);
    input_size = input_stat.st_size + 64;
    input_data = (char *)mmap(0, input_size, PROT_READ, MAP_SHARED | MAP_POPULATE, 0, 0);
    if (input_data == MAP_FAILED)
        __builtin_trap();
    madvise(input_data, input_size, MADV_SEQUENTIAL);
}
#define READ_UNSIGN                                            \
    char c;                                                    \
    for (*x = *input_data++ & 15; (c = *input_data++) >= '0';) \
        *x = *x * 10 + (c & 15);
// clang-format off
void rd_uint(unsigned *x) { READ_UNSIGN }
void rd_ull(unsigned long long *x) { READ_UNSIGN }
void rd_u32(u32 *x) { READ_UNSIGN }
void rd_u64(u64 *x) { READ_UNSIGN }
void rd_u128(u128 *x) { READ_UNSIGN }
// clang-format on
#undef READ_UNSIGN
#define READ_SIGN                                              \
    char c;                                                    \
    bool d = false;                                            \
    if (*input_data == '-')                                    \
        d = true, *input_data++;                               \
    for (*x = *input_data++ & 15; (c = *input_data++) >= '0';) \
        *x = *x * 10 + (c & 15);                               \
    *x = d ? (*x) * (-1LL) : *x;
// clang-format off
void rd_int(int *x) { READ_SIGN }
void rd_ll(long long *x) { READ_SIGN }
void rd_i32(i32 *x) { READ_SIGN }
void rd_i64(i64 *x) { READ_SIGN }
void rd_i128(i128 *x) { READ_SIGN }
// clang-format on
#undef READ_SIGN
__attribute__((destructor)) void _destructor_read_(void) {
    munmap(input_data, input_size);
    input_size = 0;
}
#endif // ONLINE-INPUT
#if 1  // ONLINE-OUTPUT
#define output_buf_size 1048576
#define output_integer_size 39
#define output_block_size 10000
static char output_buf[output_buf_size + 1];
static char output_block_str[output_block_size * 4 + 1];
static u128 power10[39];
static size_t pos;
__attribute__((constructor)) void _write_constructor_(void) {
    pos = 0;
    for (size_t i = 0; i < output_block_size; i++) {
        size_t j = 4, k = i;
        while (j--) {
            output_block_str[i * 4 + j] = k % 10 + '0';
            k /= 10;
        }
    }
    power10[0] = 1ull;
    for (size_t i = 1; i < 39; i++)
        power10[i] = power10[i - 1] * 10;
}
void flush() {
    fwrite(output_buf, 1, pos, stdout);
    pos = 0;
}
// clang-format off
size_t get_integer_size(u128 n)
{
    if (n >= power10[30])
    {
        if (n >= power10[38]) return 39;
        if (n >= power10[37]) return 38;
        if (n >= power10[36]) return 37;
        if (n >= power10[35]) return 36;
        if (n >= power10[34]) return 35;
        if (n >= power10[33]) return 34;
        if (n >= power10[32]) return 33;
        if (n >= power10[31]) return 32;
        return 31;
    }
    else if (n >= power10[20])
    {
        if (n >= power10[29]) return 30;
        if (n >= power10[28]) return 29;
        if (n >= power10[27]) return 28;
        if (n >= power10[26]) return 27;
        if (n >= power10[25]) return 26;
        if (n >= power10[24]) return 25;
        if (n >= power10[23]) return 24;
        if (n >= power10[22]) return 23;
        if (n >= power10[21]) return 22;
        return 21;
    }
    else if (n >= power10[10])
    {
        if (n >= power10[19]) return 20;
        if (n >= power10[18]) return 19;
        if (n >= power10[17]) return 18;
        if (n >= power10[16]) return 17;
        if (n >= power10[15]) return 16;
        if (n >= power10[14]) return 15;
        if (n >= power10[13]) return 14;
        if (n >= power10[12]) return 13;
        if (n >= power10[11]) return 12;
        return 11;
    }
    else
    {
        if (n >= power10[9]) return 10;
        if (n >= power10[8]) return 9;
        if (n >= power10[7]) return 8;
        if (n >= power10[6]) return 7;
        if (n >= power10[5]) return 6;
        if (n >= power10[4]) return 5;
        if (n >= power10[3]) return 4;
        if (n >= power10[2]) return 3;
        if (n >= power10[1]) return 2;
        return 1;
    }
}
// clang-format on
#define OUTPUT_BUFFER_CHECK                          \
    if (__builtin_expect(pos == output_buf_size, 0)) \
        flush();
void wtn(void) {
    output_buf[pos++] = '\n';
    OUTPUT_BUFFER_CHECK
}
void wts(void) {
    output_buf[pos++] = ' ';
    OUTPUT_BUFFER_CHECK
}
void wt_char(char c) {
    output_buf[pos++] = c;
    OUTPUT_BUFFER_CHECK
}
void wt_str(const char *s) {
    while (*s != 0) {
        output_buf[pos++] = *s++;
        OUTPUT_BUFFER_CHECK
    }
}
#undef OUTPUT_BUFFER_CHECK
#define WRITE_UNSIGN_NUMBER                                                                \
    if (__builtin_expect(pos + output_integer_size >= output_buf_size, 0))                 \
        flush();                                                                           \
    size_t digit = get_integer_size((u128)x);                                              \
    size_t len = digit;                                                                    \
    while (len >= 4) {                                                                     \
        len -= 4;                                                                          \
        memcpy(output_buf + pos + len, output_block_str + (x % output_block_size) * 4, 4); \
        x /= output_block_size;                                                            \
    }                                                                                      \
    memcpy(output_buf + pos, output_block_str + x * 4 + (4 - len), len);                   \
    pos += digit;

#define WRITE_SIGN_NUMBER                                                                  \
    if (__builtin_expect(pos + output_integer_size >= output_buf_size, 0))                 \
        flush();                                                                           \
    if (x < (i128)0ull)                                                                    \
        wt_char('-'), x = -x;                                                              \
    size_t digit = get_integer_size(x);                                                    \
    size_t len = digit;                                                                    \
    while (len >= 4) {                                                                     \
        len -= 4;                                                                          \
        memcpy(output_buf + pos + len, output_block_str + (x % output_block_size) * 4, 4); \
        x /= output_block_size;                                                            \
    }                                                                                      \
    memcpy(output_buf + pos, output_block_str + x * 4 + (4 - len), len);                   \
    pos += digit;
// clang-format off
void wt_uint(unsigned x) { WRITE_UNSIGN_NUMBER }
void wt_ull(unsigned long long x) { WRITE_UNSIGN_NUMBER }
void wt_u32(u32 x) { WRITE_UNSIGN_NUMBER }
void wt_u64(u64 x) { WRITE_UNSIGN_NUMBER }
void wt_u128(u128 x) { WRITE_UNSIGN_NUMBER }
void wt_int(int x) { WRITE_SIGN_NUMBER }
void wt_ll(long long x) { WRITE_SIGN_NUMBER }
void wt_i32(i32 x) { WRITE_SIGN_NUMBER }
void wt_i64(i64 x) { WRITE_SIGN_NUMBER }
void wt_i128(i128 x) { WRITE_SIGN_NUMBER }
// clang-format on
__attribute__((destructor)) void _write_destructor_(void) {
    flush();
    pos = 0;
}
#undef WRITE_SIGN_NUMBER
#undef WRITE_UNSIGN_NUMBER
#undef output_buf_size
#undef output_integer_size
#undef output_block_size
#endif // ONLINE-OUTPUT
#endif // ONLINE-IO

#if defined(OFF_LINE) // OFFLINE-IO
#if defined(_WIN32)
#define GetChar _getchar_nolock
#define PutChar _putchar_nolock
#else
#define GetChar getchar_unlocked
#define PutChar putchar_unlocked
#endif
#define INPUT_UINT(bit)                         \
    u##bit c, x = 0;                            \
    while (c = GetChar(), c < '0' || c > '9') { \
    }                                           \
    while ('/' < c && c < ':') {                \
        x = x * 10 + c - '0';                   \
        c = GetChar();                          \
    }                                           \
    return x
// clang-format off
u32 in_u32(void) { INPUT_UINT(32); }
u64 in_u64(void) { INPUT_UINT(64); }
u128 in_u128(void) { INPUT_UINT(128); }
// clang-format on
#undef INPUT_UINT
#define INPUT_SINT(bit)                         \
    i##bit c, x = 0, f = 1;                     \
    while (c = GetChar(), c < '0' || c > '9') { \
        if (c == '-') {                         \
            f = -f;                             \
        }                                       \
    }                                           \
    while ('/' < c && c < ':') {                \
        x = x * 10 + c - '0';                   \
        c = GetChar();                          \
    }                                           \
    return f * x
// clang-format off
i32 in_i32(void) { INPUT_SINT(32); }
i64 in_i64(void) { INPUT_SINT(64); }
i128 in_i128(void) { INPUT_SINT(128); }
// clang-format on
#undef INPUT_SINT
#
#define OUTPUT_UINT(bit)    \
    if (x >= 10) {          \
        out_u##bit(x / 10); \
    }                       \
    PutChar(x - x / 10 * 10 + '0')
// clang-format off
void out_u32(u32 x) { OUTPUT_UINT(32); }
void out_u64(u64 x) { OUTPUT_UINT(64); }
void out_u128(u128 x) { OUTPUT_UINT(128); }
// clang-format on
#undef OUTPUT_UINT
#define OUTPUT_SINT(bit) \
    if (x < 0) {         \
        PutChar('-');    \
        x = -x;          \
    }                    \
    out_u##bit((u##bit)x)
// clang-format off
void out_i32(i32 x) { OUTPUT_SINT(32); }
void out_i64(i64 x) { OUTPUT_SINT(64); }
void out_i128(i128 x) { OUTPUT_SINT(128); }
// clang-format on
#undef OUTPUT_SINT
// clang-format off
void newline(void) { PutChar('\n'); }
void space(void) { PutChar(' '); }
// clang-format on
#define OUTPUT_BINARY(bit)                                 \
    u##bit mask = (u##bit)1 << (sizeof(v) * CHAR_BIT - 1); \
    do {                                                   \
        PutChar(mask &v ? '1' : '0');                      \
    } while (mask >>= 1)
// clang-format off
void printb_32bit(u32 v) { OUTPUT_BINARY(32); }
void printb_64bit(u64 v) { OUTPUT_BINARY(64); }
// clang-format on
#undef OUTPUT_BINARY
#endif // OFFLINE-IO

#if 1 // MODINT

// clang-format off
typedef struct { i32 a, b; u32 d; } Bezout32;
typedef struct { i64 a, b; u64 d; } Bezout64;
#define BEZOUT(bit)                                                     \
    u##bit t;                                                           \
    bool swap = x < y;                                                  \
    if (swap) t = x, x = y, y = t;                                      \
    if (y == 0)                                                         \
    {                                                                   \
        if (x == 0)                                                     \
            return (Bezout##bit) {0, 0, 0};                             \
        else if (swap)                                                  \
            return (Bezout##bit) {0, 1, x};                             \
        else                                                            \
            return (Bezout##bit) {1, 0, x};                             \
    }                                                                   \
    i##bit s0 = 1, s1 = 0, t0 = 0, t1 = 1;                              \
    while (true)                                                        \
    {                                                                   \
        u##bit q = x / y, r = x % y;                                    \
        if (r == 0)                                                     \
        {                                                               \
            if (swap)                                                   \
                return (Bezout##bit) {t1, s1, y};                       \
            else                                                        \
                return (Bezout##bit) {s1, t1, y};                       \
        }                                                               \
        i##bit s2 = s0 - (i##bit)(q) * s1, t2 = t0 - (i##bit)(q) * t1;  \
        x = y, y = r;                                                   \
        s0 = s1, s1 = s2, t0 = t1, t1 = t2;                             \
    }
Bezout32 bezout32(u32 x, u32 y) { BEZOUT(32); }
Bezout64 bezout64(u64 x, u64 y) { BEZOUT(64); }
u32 modinv32(u32 x, u32 mod) { Bezout32 abd = bezout32(x, mod); return abd.a < 0 ? mod + abd.a : (u32)abd.a; }
u64 modinv64(u64 x, u64 mod) { Bezout64 abd = bezout64(x, mod); return abd.a < 0 ? mod + abd.a : (u64)abd.a; }
#undef BEZOUT
// clang-format on

// clang-format off
#if defined(USE_DM32) // DM32
u32 r1_dm32(u32 mod) { return (u32)(i32)-1 % mod + 1; }
u32 r2_dm32(u32 mod) { return (u64)(i64)-1 % mod + 1; }
u32 r3_dm32(u32 mod, u32 r1, u32 r2) { return (u32)(((u64)r1 * (u64)r2) % mod); }
u32 n_dm32(u32 mod) { return mod; }
u32 ni_dm32(u32 mod) { u32 ni = mod; for (int _ = 0; _ < 4; ++_) ni *= 2 - ni * mod; return ni; }
u32 n2_dm32(u32 mod) { return mod << 1; }
u32 dmr32(u64 a, u32 ni, u32 n) { u32 y = (u32)(a >> 32) - (u32)(((u64)((u32)a * ni) * n) >> 32); return (i32)y < 0 ? y + n : y; }
u32 to_dm32(u32 a, u32 r2, u32 ni, u32 n) { return dmr32((u64)a * r2, ni, n); }
u32 from_dm32(u32 a, u32 ni, u32 n) { return dmr32((u64)a, ni, n); }
u32 add_dm32(u32 a, u32 b, u32 n) { a += b; a -= (a >= n ? n : 0); return a; }
u32 sub_dm32(u32 a, u32 b, u32 n) { a += (a < b ? n : 0); a -= b; return a; }
u32 min_dm32(u32 a, u32 n) { return sub_dm32(0, a, n); }
u32 relaxed_add_dm32(u32 a, u32 b, u32 n2) { a += b - n2; a += n2 & -(a >> 31u); return a; }
u32 relaxed_sub_dm32(u32 a, u32 b, u32 n2) { a -= b; a += n2 & -(a >> 31u); return a; }
u32 relaxed_min_dm32(u32 a, u32 n2) { return relaxed_sub_dm32(0, a, n2); }
u32 mul_dm32(u32 a, u32 b, u32 ni, u32 n) { return dmr32((u64)a * b, ni, n); }
u32 squ_dm32(u32 a, u32 ni, u32 n) { return dmr32((u64)a * a, ni, n); }
u32 shl_dm32(u32 a, u32 n) { return (a <<= 1) >= n ? a - n : a; }
u32 shr_dm32(u32 a, u32 n) { return (a & 1) ? ((a >> 1) + (n >> 1) + 1) : (a >> 1); }
u32 pow_dm32(u32 a, u64 k, u32 r1, u32 ni, u32 n) { u32 ret = r1; while (k > 0) { if (k & 1) ret = mul_dm32(ret, a, ni, n); a = squ_dm32(a, ni, n); k >>= 1; } return ret; }
u32 inv_dm32(u32 a, u32 r3, u32 ni, u32 n) { return dmr32((u64)r3 * modinv32(a, n), ni, n); }
u32 div_dm32(u32 a, u32 b, u32 r3, u32 ni, u32 n) { return mul_dm32(a, inv_dm32(b, r3, ni, n), ni, n); }
#endif                // DM32
// clang-format off

// clang-fromat off
#if defined(USE_DM64) // DM64
u64 r1_dm64(u64 mod) { return (u64)(i64)-1 % mod + 1; }
u64 r2_dm64(u64 mod) { return (u128)(i128)-1 % mod + 1; }
u64 r3_dm64(u64 mod, u64 r1, u64 r2) { return (u64)(((u128)r1 * (u128)r2) % mod); }
u64 n_dm64(u64 mod) { return mod; }
u64 ni_dm64(u64 mod) { u64 ni = mod; for (int _ = 0; _ < 5; ++_) ni *= 2 - ni * mod; return ni; }
u64 n2_dm64(u64 mod) { return mod << 1; }
u64 dmr64(u128 a, u64 ni, u64 n) { u64 y = (u64)(a >> 64) - (u64)(((u128)((u64)a * ni) * n) >> 64); return (i64)y < 0 ? y + n : y; }
u64 to_dm64(u64 a, u64 r2, u64 ni, u64 n) { return dmr64((u128)a * r2, ni, n); }
u64 from_dm64(u64 a, u64 ni, u64 n) { return dmr64((u128)a, ni, n); }
u64 add_dm64(u64 a, u64 b, u64 n) { a += b; a -= (a >= n ? n : 0); return a; }
u64 sub_dm64(u64 a, u64 b, u64 n) { a += (a < b ? n : 0); a -= b; return a; }
u64 min_dm64(u64 a, u64 n) { return sub_dm64(0, a, n); }
u64 relaxed_add_dm64(u64 a, u64 b, u64 n2) { a += b - n2; a += n2 & -(a >> 63u); return a; }
u64 relaxed_sub_dm64(u64 a, u64 b, u64 n2) { a -= b; a += n2 & -(a >> 63u); return a; }
u64 relaxed_min_dm64(u64 a, u64 n2) { return relaxed_sub_dm64(0, a, n2); }
u64 mul_dm64(u64 a, u64 b, u64 ni, u64 n) { return dmr64((u128)a * b, ni, n); }
u64 squ_dm64(u64 a, u64 ni, u64 n) { return dmr64((u128)a * a, ni, n); }
u64 shl_dm64(u64 a, u64 n) { return (a <<= 1) >= n ? a - n : a; }
u64 shr_dm64(u64 a, u64 n) { return (a & 1) ? ((a >> 1) + (n >> 1) + 1) : (a >> 1); }
u64 pow_dm64(u64 a, u64 k, u64 r1, u64 ni, u64 n) { u64 ret = r1; while (k > 0) { if (k & 1) ret = mul_dm64(ret, a, ni, n); a = squ_dm64(a, ni, n); k >>= 1; } return ret; }
u64 inv_dm64(u64 a, u64 r3, u64 ni, u64 n) { return dmr64((u128)r3 * modinv64(a, n), ni, n); }
u64 div_dm64(u64 a, u64 b, u64 r3, u64 ni, u64 n) { return mul_dm64(a, inv_dm64(b, r3, ni, n), ni, n); }
#endif                // DM64
// clang-fromat on

// clang-fromat off
#if defined(USE_M32)  // M32
static u32 n_m32, n2_m32, ni_m32, r1_m32, r2_m32, r3_m32;
static inline void set_m32(u32 mod) { if (mod == n_m32) return; n_m32 = mod; n2_m32 = mod << 1; ni_m32 = mod; for (int _ = 0; _ < 4; ++_) ni_m32 *= 2 - ni_m32 * mod; r1_m32 = (u32)(i32)-1 % mod + 1; r2_m32 = (u64)(i64)-1 % mod + 1; r3_m32 = (u32)(((u64)r1_m32 * (u64)r2_m32) % mod); }
static inline u32 mr32(u64 a) { u32 y = (u32)(a >> 32) - (u32)(((u64)((u32)a * ni_m32) * n_m32) >> 32); return (i32)y < 0 ? y + n_m32 : y; }
static inline u32 to_m32(u32 a) { return mr32((u64)a * r2_m32); }
static inline u32 from_m32(u32 a) { return mr32((u64)a); }
static inline u32 add_m32(u32 a, u32 b) { a += b; a -= (a >= n_m32 ? n_m32 : 0); return a; }
static inline u32 sub_m32(u32 a, u32 b) { a += (a < b ? n_m32 : 0); a -= b; return a; }
static inline u32 min_m32(u32 a) { return sub_m32(0, a); }
static inline u32 relaxed_add_m32(u32 a, u32 b) { a += b - n2_m32; a += n2_m32 & -(a >> 31u); return a; }
static inline u32 relaxed_sub_m32(u32 a, u32 b) { a -= b; a += n2_m32 & -(a >> 31u); return a; }
static inline u32 relaxed_min_m32(u32 a) { return relaxed_sub_m32(0, a); }
static inline u32 mul_m32(u32 a, u32 b) { return mr32((u64)a * b); }
static inline u32 squ_m32(u32 a) { return mr32((u64)a * a); }
static inline u32 shl_m32(u32 a) { return (a <<= 1) >= n_m32 ? a - n_m32 : a; }
static inline u32 shr_m32(u32 a) { return (a & 1) ? ((a >> 1) + (n_m32 >> 1) + 1) : (a >> 1); }
static inline u32 pow_m32(u32 a, u64 k) { u32 ret = r1_m32; while (k > 0) { if (k & 1) ret = mul_m32(ret, a); a = squ_m32(a); k >>= 1; } return ret; }
static inline u32 inv_m32(u32 a) { return mr32((u64)r3_m32 * modinv32(a, n_m32)); }
static inline u32 div_m32(u32 a, u32 b) { return mul_m32(a, inv_m32(b)); }
#endif                // M32
// clang-fromat on

// clang-fromat off
#if defined(USE_M64)  // M64
static u64 n_m64, n2_m64, ni_m64, r1_m64, r2_m64, r3_m64;
static inline void set_m64(u64 mod) { if (mod == n_m64) return; n_m64 = mod; n2_m64 = mod << 1; ni_m64 = mod; for (int _ = 0; _ < 5; ++_) ni_m64 *= 2 - ni_m64 * mod; r1_m64 = (u64)(i64)-1 % mod + 1; r2_m64 = (u128)(i128)-1 % mod + 1; r3_m64 = (u64)(((u128)r1_m64 * (u128)r2_m64) % mod); }
static inline u64 mr64(u128 a) { u64 y = (u64)(a >> 64) - (u64)(((u128)((u64)a * ni_m64) * n_m64) >> 64); return (i64)y < 0 ? y + n_m64 : y; }
static inline u64 to_m64(u64 a) { return mr64((u128)a * r2_m64); }
static inline u64 from_m64(u64 a) { return mr64((u128)a); }
static inline u64 add_m64(u64 a, u64 b) { a += b; a -= (a >= n_m64 ? n_m64 : 0); return a; }
static inline u64 sub_m64(u64 a, u64 b) { a += (a < b ? n_m64 : 0); a -= b; return a; }
static inline u64 min_m64(u64 a) { return sub_m64(0, a); }
static inline u64 relaxed_add_m64(u64 a, u64 b) { a += b - n2_m64; a += n2_m64 & -(a >> 63u); return a; }
static inline u64 relaxed_sub_m64(u64 a, u64 b) { a -= b; a += n2_m64 & -(a >> 63u); return a; }
static inline u64 relaxed_min_m64(u64 a) { return relaxed_sub_m64(0, a); }
static inline u64 mul_m64(u64 a, u64 b) { return mr64((u128)a * b); }
static inline u64 squ_m64(u64 a) { return mr64((u128)a * a); }
static inline u64 shl_m64(u64 a) { return (a <<= 1) >= n_m64 ? a - n_m64 : a; }
static inline u64 shr_m64(u64 a) { return (a & 1) ? ((a >> 1) + (n_m64 >> 1) + 1) : (a >> 1); }
static inline u64 pow_m64(u64 a, u64 k) { u64 ret = r1_m64; while (k > 0) { if (k & 1) ret = mul_m64(ret, a); a = squ_m64(a); k >>= 1; } return ret; }
static inline u64 inv_m64(u64 a) { return mr64((u128)r3_m64 * modinv64(a, n_m64)); }
static inline u64 div_m64(u64 a, u64 b) { return mul_m64(a, inv_m64(b)); }
#endif                // M64
// clang-fromat on

// clang-fromat off
#if defined(USE_B32)  // B32
static u64 m_b32, m2_b32, im_b32, div_b32, rem_b32;
void set_b32(u64 mod) { if (mod == m_b32) return; m_b32 = mod; m2_b32 = mod << 1; im_b32 = ((((u128)(1ull) << 64)) + mod - 1) / mod; div_b32 = 0; rem_b32 = 0; }
static inline void br32(u64 a) { u64 x = (u64)(((u128)(a)*im_b32) >> 64); u64 y = x * m_b32; unsigned long long z; u32 w = __builtin_usubll_overflow(a, y, &z) ? m_b32 : 0; div_b32 = x; rem_b32 = z + w; }
u32 add_b32(u32 a, u32 b) { a += b; a -= (a >= (u32)m_b32 ? (u32)m_b32 : 0); return a; }
u32 sub_b32(u32 a, u32 b) { a += (a < b ? (u32)m_b32 : 0); a -= b; return a; }
u32 min_b32(u32 a) { return sub_b32(0, a); }
u32 relaxed_add_b32(u32 a, u32 b) { a += b - n2_m32; a += n2_m32 & -(a >> 31u); return a; }
u32 relaxed_sub_b32(u32 a, u32 b) { a -= b; a += n2_m32 & -(a >> 31u); return a; }
u32 relaxed_min_b32(u32 a) { return relaxed_sub_m32(0, a); }
u32 mul_b32(u32 a, u32 b) { br32((u64)a * b); return (u32)rem_b32; }
u32 squ_b32(u32 a) { br32((u64)a * a); return (u32)rem_b32; }
u32 shl_b32(u32 a) { return (a <<= 1) >= m_b32 ? a - m_b32 : a; }
u32 shr_b32(u32 a) { return (a & 1) ? ((a >> 1) + (m_b32 >> 1) + 1) : (a >> 1); }
u32 pow_b32(u32 a, u64 k) { u32 ret = 1u; while (k > 0) { if (k & 1) ret = mul_b32(ret, a); a = squ_b32(a); k >>= 1; } return ret; }
#endif                // B32
// clang-fromat on

// clang-fromat off
#if defined(USE_B64)  // B64
static u128 m_b64, m2_b64, im_b64, div_b64, rem_b64;
void set_b64(u128 mod) { if (mod == m_b64) return; m_b64 = mod; m2_b64 = mod << 1; im_b64 = (~((u128)0ull)) / mod; div_b64 = 0; rem_b64 = 0; }
static inline void br64(u128 x) { if (m_b64 == 1) { div_b64 = x; rem_b64 = 0; return; } u8 f; u128 a = x >> 64; u128 b = x & 0xffffffffffffffffull; u128 c = im_b64 >> 64; u128 d = im_b64 & 0xffffffffffffffffull; u128 ac = a * c; u128 bd = (b * d) >> 64; u128 ad = a * d; u128 bc = b * c; f = (ad > ((u128)((i128)(-1L)) - bd)); bd += ad; ac += f; f = (bc > ((u128)((i128)(-1L)) - bd)); bd += bc; ac += f; u128 q = ac + (bd >> 64); u128 r = x - q * m_b64; if (m_b64 <= r) { r -= m_b64; q += 1; } div_b64 = q; rem_b64 = r; }
u64 add_b64(u64 a, u64 b) { a += b; a -= (a >= (u64)m_b64 ? (u64)m_b64 : 0); return a; }
u64 sub_b64(u64 a, u64 b) { a += (a < b ? (u64)m_b64 : 0); a -= b; return a; }
u64 min_b64(u64 a) { return sub_b64(0, a); }
u64 relaxed_add_b64(u64 a, u64 b) { a += b - m2_b64; a += m2_b64 & -(a >> 63u); return a; }
u64 relaxed_sub_b64(u64 a, u64 b) { a -= b; a += m2_b64 & -(a >> 63u); return a; }
u64 relaxed_min_b64(u64 a) { return relaxed_sub_b64(0, a); }
u64 mul_b64(u64 a, u64 b) { br64((u128)a * b); return (u64)rem_b64; }
u64 squ_b64(u64 a) { br64((u128)a * a); return (u64)rem_b64; }
u64 shl_b64(u64 a) { return (a <<= 1) >= m_b64 ? a - m_b64 : a; }
u64 shr_b64(u64 a) { return (a & 1) ? ((a >> 1) + (m_b64 >> 1) + 1) : (a >> 1); }
u64 pow_b64(u64 a, u64 k) { u64 ret = 1ull; while (k > 0) { if (k & 1) ret = mul_b64(ret, a); a = squ_b64(a); k >>= 1; } return ret; }
#endif                // B64
// clang-fromat on

#endif // MODINT

#if defined(USE_GCD) // GCD
#define BINARY_GCD(bit)             \
    if (!a || !b)                   \
        return a | b;               \
    u##bit t, s = ctz##bit(a | b);  \
    a >>= ctz##bit(a);              \
    do                              \
    {                               \
        b >>= ctz##bit(b);          \
        if (a > b)                  \
            t = a, a = b, b = t;    \
        b -= a;                     \
    } while (b);                    \
    return a << s
u32 gcd32(u32 a, u32 b) { BINARY_GCD(32); }
u64 gcd64(u64 a, u64 b) { BINARY_GCD(64); }
#undef BINARY_GCD
#endif // GCD

#if defined(USE_COMBSORT) // COMBSORT
#define COMBSORT11(bit)                                  \
    int g = a_len;                                       \
    u##bit t;                                            \
    while (true)                                         \
    {                                                    \
        bool flag = 1;                                   \
        g = (((g * 10) / 13) > 1) ? ((g * 10) / 13) : 1; \
        if (g == 9 || g == 10)                           \
            g = 11;                                      \
        for (int i = 0; i + g < a_len; i++)              \
        {                                                \
            if (a[i] > a[i + g])                         \
            {                                            \
                t = a[i], a[i] = a[i + g], a[i + g] = t; \
                flag = false;                            \
            }                                            \
        }                                                \
        if (g == 1 && flag)                              \
            break;                                       \
    }
void combsort11_32(int a_len, u32 *a) { COMBSORT11(32) }
void combsort11_64(int a_len, u64 *a) { COMBSORT11(64) }
#undef COMBSORT11
#endif // COMBSORT

#if defined(USE_QUADRATIC_RESIDUE) // QUADRATIC_RESIDUE
bool euler_criterion(u32 a, u32 mod)
{
#if defined(USE_M32)
    return pow_m32(to_m32(a), (mod - 1) >> 1) == r1_m32;
#elif defined(USE_B32)
    return pow_b32(a, (mod - 1) >> 1) == 1;
#else
    u32 ret = 1, b = a, k = (mod - 1) >> 1;
    while (k)
    {
        if (k & 1)
            ret = (u64)b * ret % mod;
        b = (u64)b * b % mod;
        k >>= 1;
    }
    return ret == 1;
#endif
}
int legendre_symbol(u32 a, u32 mod)
{
    /* assert(a >= 0 && mod & 1 && is_prime(mod)); */
    int ret;
#if defined(USE_M32)
    if (mr32((u64)a) == 0)
        ret = 0;
    else if (euler_criterion(a, mod))
        ret = 1;
    else
        ret = -1;
#else
    if (a == 0)
        ret = 0;
    else if (euler_criterion(a, mod))
        ret = 1;
    else
        ret = -1;
#endif
    return ret;
}
int jacobi_symbol(long long a, long long n)
{
    int j = 1;
    long long t;
    while (a)
    {
        if (a < 0)
        {
            a = -a;
            if ((n & 3) == 3)
                j = -j;
        }
        int s = ctz64(a);
        a >>= s;
        if ((((n & 7) == 3) || ((n & 7) == 5)) && (s & 1))
            j = -j;
        if ((a & n & 3) == 3)
            j = -j;
        t = a, a = n, n = t;
        a %= n;
        if ((a << 1) > n)
            a -= n;
    }
    return n == 1 ? j : 0;
}
#endif // QUADRATIC_RESIDUE

#if defined(USE_POW_ROOT) // POW_ROOT
#define IPOW(bit)   \
    u##bit p = 1;   \
    while (k)       \
    {               \
        if (k & 1)  \
            p *= n; \
        k >>= 1;    \
        if (k)      \
            n *= n; \
    }               \
    return p
u32 ipow32(u32 n, u64 k) { IPOW(32); }
u64 ipow64(u64 n, u64 k) { IPOW(64); }
#undef IPOW

#define SPOW(bit)                                                                          \
    if (y == 0)                                                                            \
        return 1;                                                                          \
    u##bit res = 1;                                                                        \
    while (y)                                                                              \
    {                                                                                      \
        if (y & 1)                                                                         \
            res = __builtin_mul_overflow_p(res, x, (u##bit)0) ? UINT##bit##_MAX : res * x; \
        x = __builtin_mul_overflow_p(x, x, (u##bit)0) ? UINT##bit##_MAX : x * x;           \
        y >>= 1;                                                                           \
    }                                                                                      \
    return res

u32 spow_u32(u32 x, u32 y) { SPOW(32); }
u64 spow_u64(u64 x, u64 y) { SPOW(64); }
#undef SPOW

u64 isqrt(u64 n)
{
    u64 root;
    if (n >= (u64)(18446744065119617025ull))
        return (u64)(4294967295ull);
    root = (u64)sqrt((double)n);
    if (root * root > n)
        root--;
    if ((root + 1) * (root + 1) <= n)
        root++;
    return root;
}
u64 icbrt(u64 n)
{
    u64 b, root = 0;
    int s = 63;
    if (n >= (u64)(18446724184312856125ull))
        return (u64)(2642245ull);
    for (; s >= 0; s -= 3)
    {
        root += root;
        b = 3 * root * (root + 1) + 1;
        if ((n >> s) >= b)
        {
            n -= b << s;
            root++;
        }
    }
    return root;
}
u64 floor_kth_root_integer(u64 a, u64 k)
{
    if (a <= 1 || k == 1)
        return a;
    if (k >= 64)
        return 1;
    if (k == 2)
        return isqrt(a);
    if (a == UINT64_MAX)
        a--;
    u64 res = (k == 3 ? icbrt(a) : pow(a, nextafter(1 / (double)k, 0)));
    while (spow_u64(res + 1, k) <= a)
        res++;
    return res;
}
bool is_perfect_square(u64 n)
{
    u32 m = n & 127;
    if ((m * 0x8bc40d7d) & (m * 0xa1e2f5d1) & 0x14020a) return false;
    m = n % 240;
    if ((m * 0xfa445556) & (m * 0x8021feb1) & 0x614aaa0f) return false;
    m = isqrt(n);
    return (u64)m * m == n;
}
bool is_perfect_cube(u64 n)
{
    u32 m;
    m = n % 117;
    if ((m * 833230740) & (m * 120676722) & 813764715) return false;
    m = n % 133;
    if ((m * 76846229) & (m * 305817297) & 306336544) return false;
    m = n % 43;
    if ((m * 193635074) & (m * 3653322805U) & 74401) return false;
    m = n % 37;
    if ((m * 919307198) & (m * 3908849845U) & 6665) return false;
    m = icbrt(n);
    return (u64)m * m * m == n;
}
bool is_perfect_fifth(u64 n)
{
    u64 m;
    if ((n & 3) == 2) return false;
    m = n % 88;
    if ((m * 85413603) & (m * 76260301) & 26476550) return false;
    m = n % 31;
    if ((m * 80682551) & (m * 73523539) & 45414528) return false;
    m = n % 41;
    if ((m * 92806493) & (m * 130690042) & 35668129) return false;
    m = floor_kth_root_integer(n, 5);
    return m * m * m * m * m == n;
}
bool is_perfect_seventh(u64 n)
{
    u64 m;
    m = n & 511;
    if ((m * 97259473) & (m * 51311663) & 894)
        return false;
    m = n % 49;
    if ((m * 109645301) & (m * 76482737) & 593520192)
        return false;
    m = n % 71;
    if ((m * 71818386) & (m * 38821587) & 35299393)
        return false;
    m = floor_kth_root_integer(n, 7);
    return m * m * m * m * m * m * m == n;
}
#endif // POW_ROOT


static const u32 nf[16] = {
    0x0001u, 0x071cu, 0x6bd1u, 0x1224u,
    0x6ba8u, 0x1333u, 0x1553u, 0x0007u,
    0x071eu, 0x0925u, 0xc586u, 0x5dbdu,
    0xc463u, 0x5efdu, 0x2aa1u, 0x155au};
static const u32 fn[16] = {
    0x0001u, 0x0102u, 0x0183u, 0x8041u,
    0x015cu, 0x5f24u, 0xde2cu, 0x957eu,
    0x01f4u, 0xf7d8u, 0x76b0u, 0x5d52u,
    0xa977u, 0x20d1u, 0xc1a4u, 0x271fu};
static inline u32 to_field(u32 x) {
    u32 y = 0;
    for (; x; x &= x - 1)
        y ^= nf[ctz32(x)];
    return y;
}
static inline u32 to_nimber(u32 x) {
    u32 y = 0;
    for (; x; x &= x - 1)
        y ^= fn[ctz32(x)];
    return y;
}
static inline u32 to_double(u32 x) { return x << 1 ^ (x < 32768 ? 0 : 93099U); }
u16 ln[65536];
u16 expo[196605];
u16 *Hexpo = expo + 62133;
u16 *H2expo = expo + 58731;
__attribute__((constructor)) void _construct_nimber_product_() {
    *expo = 1;
    for (int i = 1; i < 65535; ++i)
        expo[i] = to_double(expo[i - 1]);
    for (int i = 1; i < 65535; ++i) {
        expo[i] = to_nimber(expo[i]);
        ln[expo[i]] = i;
    }
    memcpy(expo + 65535, expo, 131070);
    memcpy(expo + 131070, expo, 131070);
}
u16 product_16(u16 A, u16 B) { return A && B ? expo[ln[A] + ln[B]] : 0; }
u16 H_16(u16 A) { return A ? Hexpo[ln[A]] : 0; }
u16 H2_16(u16 A) { return A ? H2expo[ln[A]] : 0; }
u16 Hproduct_16(u16 A, u16 B) { return A && B ? Hexpo[ln[A] + ln[B]] : 0; }
u32 product_32(u32 A, u32 B) {
    u16 a = A & 65535;
    u16 b = B & 65535;
    u16 c = A >> 16;
    u16 d = B >> 16;
    u16 e = product_16(a, b);
    return (u32)(product_16((u16)(a ^ c), (u16)(b ^ d)) ^ e) << 16 | (Hproduct_16(c, d) ^ e);
}
u32 H_32(u32 A) {
    u16 a = A & 65535;
    u16 b = A >> 16;
    return H_16((u16)(a ^ b)) << 16 | H2_16(b);
}
u64 product(u64 A, u64 B) {
    u32 a = A & UINT_MAX;
    u32 b = B & UINT_MAX;
    u32 c = A >> 32;
    u32 d = B >> 32;
    u32 e = product_32(a, b);
    return (u64)(product_32(a ^ c, b ^ d) ^ e) << 32 | (H_32(product_32(c, d)) ^ e);
}

uint64_t nimProd(uint64_t a, uint64_t b) {
	return product(a, b);
}

uint64_t nimExp(uint64_t a, uint64_t e) {
	uint64_t r = 1;
	while (e) {
		if (e & 1) r = nimProd(r, a);
		a = nimProd(a, a);
		e >>= 1;
	}
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q, N; cin >> Q >> N;
	uint64_t sum, sum3;
	if (Q == 1) {
		sum = sum3 = 0;
	} else if (Q == 2) {
		cin >> sum >> sum3;
	} else assert(false);

	for (int i = 0; i < N; i++) {
		uint64_t v; cin >> v;
		sum ^= v;
		sum3 ^= nimProd(nimProd(v, v), v);
	}

	if (Q == 1) {
		cout << sum << ' ' << sum3 << '\n';
	} else if (Q == 2) {
		assert(sum != 0);
		// sum3 / sum
		uint64_t invSum = nimExp(sum, uint64_t(-2));
		assert(nimProd(sum, invSum) == 1);
		// a^2 + b^2 + ab
		uint64_t prod = nimProd(sum3, invSum) ^ nimProd(sum, sum);

		// We want to roots to x^2 + sum * x + prod == 0
		// We'll solve one bit at a time
		std::vector<std::pair<u64, u64>> contrib(64);
		for (int i = 0; i < 64; i++) {
			uint64_t v = uint64_t(1) << i;
			contrib[i] = {nimProd(v, v ^ sum), v};
		}

		std::vector<std::pair<u64, u64>> basis; basis.reserve(64);
		for (auto v : contrib) {
			for (auto [x, y] : basis) {
				if ((v.first ^ x) < v.first) {
					v.first ^= x;
					v.second ^= y;
				}
			}
			if (v.first == 0) {
				assert(v.second == sum);
			} else {
				basis.push_back(v);
			}
		}

		uint64_t cur_prod = prod, cur_res = 0;
		for (auto [x, y] : basis) {
			if ((cur_prod ^ x) < cur_prod) {
				cur_prod ^= x;
				cur_res ^= y;
			}
		}
		uint64_t cur_res_2 = cur_res ^ sum;
		if (cur_res_2 < cur_res) std::swap(cur_res, cur_res_2);
		cout << cur_res << ' ' << cur_res_2 << '\n';
	} else assert(false);

	return 0;
}
