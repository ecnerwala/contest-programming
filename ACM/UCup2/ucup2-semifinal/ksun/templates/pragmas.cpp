#pragma GCC optimize ("-Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2")
#pragma GCC optimize ("unroll-loops")

#define CSR_FLUSH_TO_ZERO         (1 << 15)
unsigned csr = __builtin_ia32_stmxcsr();
csr |= CSR_FLUSH_TO_ZERO;
__builtin_ia32_ldmxcsr(csr);