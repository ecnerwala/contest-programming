#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
#include <immintrin.h>

#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

namespace nyaan {

using namespace std;


namespace fastio {
static constexpr int SZ = 1 << 17;
char inbuf[SZ], outbuf[SZ];
int in_left = 0, in_right = 0, out_right = 0;

struct Pre {
  char num[40000];
  constexpr Pre() : num() {
    for (int i = 0; i < 10000; i++) {
      int n = i;
      for (int j = 3; j >= 0; j--) {
        num[i * 4 + j] = n % 10 + '0';
        n /= 10;
      }
    }
  }
} constexpr pre;

inline void load() {
  int len = in_right - in_left;
  memcpy(inbuf, inbuf + in_left, len);
  in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
  in_left = 0;
}

inline void flush() {
  fwrite(outbuf, 1, out_right, stdout);
  out_right = 0;
}

inline void skip_space() {
  if (in_left + 32 > in_right) load();
  while (inbuf[in_left] <= ' ') in_left++;
}

inline void rd(char& c) {
  if (in_left + 32 > in_right) load();
  c = inbuf[in_left++];
}
template <typename T>
inline void rd(T& x) {
  if (in_left + 32 > in_right) load();
  char c;
  do c = inbuf[in_left++];
  while (c < '-');
  [[maybe_unused]] bool minus = false;
  if constexpr (is_signed<T>::value == true) {
    if (c == '-') minus = true, c = inbuf[in_left++];
  }
  x = 0;
  while (c >= '0') {
    x = x * 10 + (c & 15);
    c = inbuf[in_left++];
  }
  if constexpr (is_signed<T>::value == true) {
    if (minus) x = -x;
  }
}
inline void rd() {}
template <typename Head, typename... Tail>
inline void rd(Head& head, Tail&... tail) {
  rd(head);
  rd(tail...);
}

inline void wt(char c) {
  if (out_right > SZ - 32) flush();
  outbuf[out_right++] = c;
}
inline void wt(bool b) {
  if (out_right > SZ - 32) flush();
  outbuf[out_right++] = b ? '1' : '0';
}
template <typename T>
inline void wt(T x) {
  if (out_right > SZ - 32) flush();
  if (!x) {
    outbuf[out_right++] = '0';
    return;
  }
  if constexpr (is_signed<T>::value == true) {
    if (x < 0) outbuf[out_right++] = '-', x = -x;
  }
  int i = 12;
  char buf[16];
  while (x >= 10000) {
    memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
    x /= 10000;
    i -= 4;
  }
  if (x < 100) {
    if (x < 10) {
      outbuf[out_right] = '0' + x;
      ++out_right;
    } else {
      uint32_t q = (uint32_t(x) * 205) >> 11;
      uint32_t r = uint32_t(x) - q * 10;
      outbuf[out_right] = '0' + q;
      outbuf[out_right + 1] = '0' + r;
      out_right += 2;
    }
  } else {
    if (x < 1000) {
      memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
      out_right += 3;
    } else {
      memcpy(outbuf + out_right, pre.num + (x << 2), 4);
      out_right += 4;
    }
  }
  memcpy(outbuf + out_right, buf + i + 4, 12 - i);
  out_right += 12 - i;
}
inline void wt() {}
template <typename Head, typename... Tail>
inline void wt(Head&& head, Tail&&... tail) {
  wt(head);
  wt(forward<Tail>(tail)...);
}
template <typename... Args>
inline void wtn(Args&&... x) {
  wt(forward<Args>(x)...);
  wt('\n');
}

struct Dummy {
  Dummy() { atexit(flush); }
} dummy;

}  // namespace fastio
using fastio::rd;
using fastio::skip_space;
using fastio::wt;
using fastio::wtn;

//

constexpr unsigned int constexpr_primitive_root(unsigned int mod) {
  using u32 = unsigned int;
  using u64 = unsigned long long;
  if(mod == 2) return 1;
  u64 m = mod - 1, ds[32] = {}, idx = 0;
  for (u64 i = 2; i * i <= m; ++i) {
    if (m % i == 0) {
      ds[idx++] = i;
      while (m % i == 0) m /= i;
    }
  }
  if (m != 1) ds[idx++] = m;
  for (u32 _pr = 2, flg = true;; _pr++, flg = true) {
    for (u32 i = 0; i < idx && flg; ++i) {
      u64 a = _pr, b = (mod - 1) / ds[i], r = 1;
      for (; b; a = a * a % mod, b >>= 1)
        if (b & 1) r = r * a % mod;
      if (r == 1) flg = false;
    }
    if (flg == true) return _pr;
  }
}





template <uint32_t mod>
struct LazyMontgomeryModInt {
  using mint = LazyMontgomeryModInt;
  using i32 = int32_t;
  using u32 = uint32_t;
  using u64 = uint64_t;

  static constexpr u32 get_r() {
    u32 ret = mod;
    for (i32 i = 0; i < 4; ++i) ret *= 2 - mod * ret;
    return ret;
  }

  static constexpr u32 r = get_r();
  static constexpr u32 n2 = -u64(mod) % mod;
  static_assert(r * mod == 1, "invalid, r * mod != 1");
  static_assert(mod < (1 << 30), "invalid, mod >= 2 ^ 30");
  static_assert((mod & 1) == 1, "invalid, mod % 2 == 0");

  u32 a;

  constexpr LazyMontgomeryModInt() : a(0) {}
  constexpr LazyMontgomeryModInt(const int64_t &b)
      : a(reduce(u64(b % mod + mod) * n2)){};

  static constexpr u32 reduce(const u64 &b) {
    return (b + u64(u32(b) * u32(-r)) * mod) >> 32;
  }

  constexpr mint &operator+=(const mint &b) {
    if (i32(a += b.a - 2 * mod) < 0) a += 2 * mod;
    return *this;
  }

  constexpr mint &operator-=(const mint &b) {
    if (i32(a -= b.a) < 0) a += 2 * mod;
    return *this;
  }

  constexpr mint &operator*=(const mint &b) {
    a = reduce(u64(a) * b.a);
    return *this;
  }

  constexpr mint &operator/=(const mint &b) {
    *this *= b.inverse();
    return *this;
  }

  constexpr mint operator+(const mint &b) const { return mint(*this) += b; }
  constexpr mint operator-(const mint &b) const { return mint(*this) -= b; }
  constexpr mint operator*(const mint &b) const { return mint(*this) *= b; }
  constexpr mint operator/(const mint &b) const { return mint(*this) /= b; }
  constexpr bool operator==(const mint &b) const {
    return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
  }
  constexpr bool operator!=(const mint &b) const {
    return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
  }
  constexpr mint operator-() const { return mint() - mint(*this); }

  constexpr mint pow(u64 n) const {
    mint ret(1), mul(*this);
    while (n > 0) {
      if (n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }
  
  constexpr mint inverse() const { return pow(mod - 2); }

  friend ostream &operator<<(ostream &os, const mint &b) {
    return os << b.get();
  }

  friend istream &operator>>(istream &is, mint &b) {
    int64_t t;
    is >> t;
    b = LazyMontgomeryModInt<mod>(t);
    return (is);
  }
  
  constexpr u32 get() const {
    u32 ret = reduce(a);
    return ret >= mod ? ret - mod : ret;
  }

  static constexpr u32 get_mod() { return mod; }
};
//

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")

using m256 = __m256i;
struct alignas(32) mmint {
  m256 x;
  static mmint R, M0, M1, M2, N2;

  mmint() : x() {}
  inline mmint(const m256& _x) : x(_x) {}
  inline mmint(unsigned int a) : x(_mm256_set1_epi32(a)) {}
  inline mmint(unsigned int a0, unsigned int a1, unsigned int a2,
               unsigned int a3, unsigned int a4, unsigned int a5,
               unsigned int a6, unsigned int a7)
      : x(_mm256_set_epi32(a7, a6, a5, a4, a3, a2, a1, a0)) {}
  inline operator m256&() { return x; }
  inline operator const m256&() const { return x; }
  inline int& operator[](int i) { return *(reinterpret_cast<int*>(&x) + i); }
  inline const int& operator[](int i) const {
    return *(reinterpret_cast<const int*>(&x) + i);
  }

  friend ostream& operator<<(ostream& os, const mmint& m) {
    unsigned r = R[0], mod = M1[0];
    auto reduce1 = [&](const uint64_t& b) {
      unsigned res = (b + uint64_t(unsigned(b) * unsigned(-r)) * mod) >> 32;
      return res >= mod ? res - mod : res;
    };
    for (int i = 0; i < 8; i++) {
      os << reduce1(m[i]) << (i == 7 ? "" : " ");
    }
    return os;
  }

  template <typename mint>
  static void set_mod() {
    R = _mm256_set1_epi32(mint::r);
    M0 = _mm256_setzero_si256();
    M1 = _mm256_set1_epi32(mint::get_mod());
    M2 = _mm256_set1_epi32(mint::get_mod() * 2);
    N2 = _mm256_set1_epi32(mint::n2);
  }

  static inline mmint reduce(const mmint& prod02, const mmint& prod13) {
    m256 unpalo = _mm256_unpacklo_epi32(prod02, prod13);
    m256 unpahi = _mm256_unpackhi_epi32(prod02, prod13);
    m256 prodlo = _mm256_unpacklo_epi64(unpalo, unpahi);
    m256 prodhi = _mm256_unpackhi_epi64(unpalo, unpahi);
    m256 hiplm1 = _mm256_add_epi32(prodhi, M1);
    m256 prodlohi = _mm256_shuffle_epi32(prodlo, 0xF5);
    m256 lmlr02 = _mm256_mul_epu32(prodlo, R);
    m256 lmlr13 = _mm256_mul_epu32(prodlohi, R);
    m256 prod02_ = _mm256_mul_epu32(lmlr02, M1);
    m256 prod13_ = _mm256_mul_epu32(lmlr13, M1);
    m256 unpalo_ = _mm256_unpacklo_epi32(prod02_, prod13_);
    m256 unpahi_ = _mm256_unpackhi_epi32(prod02_, prod13_);
    m256 prod = _mm256_unpackhi_epi64(unpalo_, unpahi_);
    return _mm256_sub_epi32(hiplm1, prod);
  }

  static inline mmint itom(const mmint& A) { return A * N2; }

  static inline mmint mtoi(const mmint& A) {
    m256 A13 = _mm256_shuffle_epi32(A, 0xF5);
    m256 lmlr02 = _mm256_mul_epu32(A, R);
    m256 lmlr13 = _mm256_mul_epu32(A13, R);
    m256 prod02_ = _mm256_mul_epu32(lmlr02, M1);
    m256 prod13_ = _mm256_mul_epu32(lmlr13, M1);
    m256 unpalo_ = _mm256_unpacklo_epi32(prod02_, prod13_);
    m256 unpahi_ = _mm256_unpackhi_epi32(prod02_, prod13_);
    m256 prod = _mm256_unpackhi_epi64(unpalo_, unpahi_);
    m256 cmp = _mm256_cmpgt_epi32(prod, M0);
    m256 dif = _mm256_and_si256(cmp, M1);
    return _mm256_sub_epi32(dif, prod);
  }

  friend inline mmint operator+(const mmint& A, const mmint& B) {
    m256 apb = _mm256_add_epi32(A, B);
    m256 ret = _mm256_sub_epi32(apb, M2);
    m256 cmp = _mm256_cmpgt_epi32(M0, ret);
    m256 add = _mm256_and_si256(cmp, M2);
    return _mm256_add_epi32(add, ret);
  }

  friend inline mmint operator-(const mmint& A, const mmint& B) {
    m256 ret = _mm256_sub_epi32(A, B);
    m256 cmp = _mm256_cmpgt_epi32(M0, ret);
    m256 add = _mm256_and_si256(cmp, M2);
    return _mm256_add_epi32(add, ret);
  }

  friend inline mmint operator*(const mmint& A, const mmint& B) {
    m256 a13 = _mm256_shuffle_epi32(A, 0xF5);
    m256 b13 = _mm256_shuffle_epi32(B, 0xF5);
    m256 prod02 = _mm256_mul_epu32(A, B);
    m256 prod13 = _mm256_mul_epu32(a13, b13);
    return reduce(prod02, prod13);
  }

  inline mmint& operator+=(const mmint& A) { return (*this) = (*this) + A; }
  inline mmint& operator-=(const mmint& A) { return (*this) = (*this) - A; }
  inline mmint& operator*=(const mmint& A) { return (*this) = (*this) * A; }

  bool operator==(const mmint& A) {
    m256 sub = _mm256_sub_epi32(x, A.x);
    return _mm256_testz_si256(sub, sub) == 1;
  }
  bool operator!=(const mmint& A) { return !((*this) == A); }
};
__attribute__((aligned(32))) mmint mmint::R;
__attribute__((aligned(32))) mmint mmint::M0, mmint::M1, mmint::M2, mmint::N2;

/**
 * @brief vectorize modint
 */

//

using mint = LazyMontgomeryModInt<998244353>;

constexpr uint32_t mod = mint::get_mod();
constexpr uint32_t pr = constexpr_primitive_root(mod);
constexpr int level = __builtin_ctzll(mod - 1);
mint dw[level], dy[level];

void setwy(int k) {
  mint w[level], y[level];
  w[k - 1] = mint(pr).pow((mod - 1) / (1 << k));
  y[k - 1] = w[k - 1].inverse();
  for (int i = k - 2; i > 0; --i)
    w[i] = w[i + 1] * w[i + 1], y[i] = y[i + 1] * y[i + 1];
  dw[1] = w[1], dy[1] = y[1], dw[2] = w[2], dy[2] = y[2];
  for (int i = 3; i < k; ++i) {
    dw[i] = dw[i - 1] * y[i - 2] * w[i];
    dy[i] = dy[i - 1] * w[i - 2] * y[i];
  }
}

void ntt(mmint* a, int k) {
  assert(k > 4);
  if (k & 1) {
    int v = 1 << (k - 1 - 3);
    for (int j = 0; j < v; ++j) {
      mmint ajv = a[j + v];
      a[j + v] = a[j] - ajv;
      a[j] += ajv;
    }
  }
  int u = 1 << (2 + (k & 1));
  int v = 1 << (k - 2 - (k & 1));
  mint one = mint(1), imag = dw[1];
  mmint ONE{one.a}, IMAG{imag.a};

  while (v) {
    if (v == 2) exit(1);
    if (v == 1) {
      /**/
      mint W = one, X = one, Y = imag;
      mmint A = a[0];
      mmint B = {one.a, one.a, W.a,           W.a,
                 one.a, one.a, (W * dw[1]).a, (W * dw[1]).a};
      int jh = 0;
      for (; jh < u - 8; jh += 8) {
        mmint T0 = A * B;
        mmint T0m = _mm256_sub_epi32(mmint::M2, T0);
        mmint T1 = m256(_mm256_shuffle_ps((__m256)T0m.x, (__m256)T0.x, 0x4E));
        mmint C = {one.a, Y.a,           one.a, X.a,
                   one.a, (Y * dw[2]).a, one.a, (X * dw[2]).a};
        X = X * dw[2];
        mmint T2 = (T0 + T1) * C;
        X *= dw[__builtin_ctzll(jh + 8)];
        W = X * X, Y = X * imag;
        mmint T2m = _mm256_sub_epi32(mmint::M2, T2);
        mmint T3 = _mm256_shuffle_epi32(T2, 0x88);
        A = a[jh / 8 + 1];
        B = {one.a, one.a, W.a,           W.a,
             one.a, one.a, (W * dw[1]).a, (W * dw[1]).a};
        mmint T4 = m256(_mm256_shuffle_ps((__m256)T2.x, (__m256)T2m.x, 0xDD));
        mmint T5 = T3 + T4;
        a[jh / 8] = _mm256_shuffle_epi32(T5, 0x8D);
      }
      mmint T0 = A * B;
      mmint T0m = _mm256_sub_epi32(mmint::M2, T0);
      mmint T1 = m256(_mm256_shuffle_ps((__m256)T0m.x, (__m256)T0.x, 0x4E));
      mmint C = {one.a, Y.a,           one.a, X.a,
                 one.a, (Y * dw[2]).a, one.a, (X * dw[2]).a};
      mmint T2 = (T0 + T1) * C;
      mmint T2m = _mm256_sub_epi32(mmint::M2, T2);
      mmint T3 = _mm256_shuffle_epi32(T2, 0x88);
      mmint T4 = m256(_mm256_shuffle_ps((__m256)T2.x, (__m256)T2m.x, 0xDD));
      mmint T5 = T3 + T4;
      a[jh / 8] = _mm256_shuffle_epi32(T5, 0x8D);
      //*/
    } else if (v == 4) {
      /**/
      mint W = one, X = one, WX = one;
      mmint IMAG1{one.a, one.a, one.a, one.a, imag.a, imag.a, imag.a, imag.a};
      mmint t01 = a[0], t23 = a[1], c01{one.a}, c23{one.a};
      int jh = 0;
      for (; jh < u - 4; jh += 4) {
        X *= dw[__builtin_ctzll(jh + 4)];
        mmint tp = t01 + t23, tm = (t01 - t23) * IMAG1;
        mmint tpm = _mm256_sub_epi32(mmint::M2, tp);
        mmint tmm = _mm256_sub_epi32(mmint::M2, tm);
        t01 = a[(jh >> 1) + 2];
        t23 = a[(jh >> 1) + 3];
        W = X * X, WX = W * X;
        c01 = {one.a, one.a, one.a, one.a, X.a, X.a, X.a, X.a};
        c23 = {W.a, W.a, W.a, W.a, WX.a, WX.a, WX.a, WX.a};
        mmint u0 = _mm256_permute2x128_si256(tp, tpm, 0x00);
        mmint u1 = _mm256_permute2x128_si256(tp, tpm, 0x31);
        mmint u2 = _mm256_permute2x128_si256(tm, tmm, 0x00);
        mmint u3 = _mm256_permute2x128_si256(tm, tmm, 0x31);
        t01 *= c01, t23 *= c23;
        a[(jh >> 1) | 0] = u0 + u1;
        a[(jh >> 1) | 1] = u2 + u3;
      }
      mmint tp = t01 + t23, tm = (t01 - t23) * IMAG1;
      mmint tpm = _mm256_sub_epi32(mmint::M2, tp);
      mmint tmm = _mm256_sub_epi32(mmint::M2, tm);
      mmint u0 = _mm256_permute2x128_si256(tp, tpm, 0x00);
      mmint u1 = _mm256_permute2x128_si256(tp, tpm, 0x31);
      mmint u2 = _mm256_permute2x128_si256(tm, tmm, 0x00);
      mmint u3 = _mm256_permute2x128_si256(tm, tmm, 0x31);
      a[(jh >> 1) | 0] = u0 + u1;
      a[(jh >> 1) | 1] = u2 + u3;
      //*/
    } else {
      int v8 = v / 8;
      {
        int j0 = 0, j1 = v8, j2 = j1 + v8, j3 = j2 + v8;
        int je = v8 - 1;
        mmint t0 = a[j0], t1 = a[j1];
        mmint t2 = a[j2], t3 = a[j3];
        for (; j0 < je; ++j0, ++j1, ++j2, ++j3) {
          mmint t0p2 = t0 + t2, t1p3 = t1 + t3;
          mmint t0m2 = t0 - t2, t1m3 = (t1 - t3) * IMAG;
          t0 = a[j0 + 1], t1 = a[j1 + 1];
          t2 = a[j2 + 1], t3 = a[j3 + 1];
          a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
          a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
        }
        mmint t0p2 = t0 + t2, t1p3 = t1 + t3;
        mmint t0m2 = t0 - t2, t1m3 = (t1 - t3) * IMAG;
        a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
        a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
      }
      mmint W{one.a}, X{dw[2].a}, Y{one.a};
      for (int jh = 4; jh < u;) {
        W = X * X, Y = X * IMAG;
        int j0 = jh * v8, j1 = j0 + v8, j2 = j1 + v8, j3 = j2 + v8;
        int je = j0 + v8 - 1;
        mmint t0, t1, t2, t3;
        t0 = a[j0], t1 = a[j1];
        t2 = a[j2] * W, t3 = a[j3] * W;
        for (; j0 < je; ++j0, ++j1, ++j2, ++j3) {
          mmint t0p2 = t0 + t2, t1p3 = (t1 + t3) * X;
          mmint t0m2 = t0 - t2, t1m3 = (t1 - t3) * Y;
          t0 = a[j0 + 1], t1 = a[j1 + 1];
          t2 = a[j2 + 1] * W, t3 = a[j3 + 1] * W;
          a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
          a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
        }
        mmint t0p2 = t0 + t2, t1p3 = (t1 + t3) * X;
        mmint t0m2 = t0 - t2, t1m3 = (t1 - t3) * Y;
        a[j0] = t0p2 + t1p3, a[j1] = t0p2 - t1p3;
        a[j2] = t0m2 + t1m3, a[j3] = t0m2 - t1m3;
        X *= mmint{dw[__builtin_ctzll((jh += 4))].a};
      }
    }
    u <<= 2;
    v >>= 2;
  }
}

void intt(mmint* a, int k) {
  assert(k > 4);
  int u = 1 << (k - 2);
  int v = 1;
  mint one = mint(1), imag = dy[1];
  mmint ONE{one.a}, IMAG{imag.a};
  while (u) {
    assert(v != 2);
    if (v == 1) {
      u <<= 2;
      /**/
      mint W = one, X = one, Y = one;
      for (int jh = 0; jh < u;) {
        W = X * X, Y = X * imag;
        mmint& A = a[jh / 8];
        mint t0, t1, t2, t3, t4, t5, t6, t7;
        t0.a = A[0], t1.a = A[1], t2.a = A[2], t3.a = A[3];
        t4.a = A[4], t5.a = A[5], t6.a = A[6], t7.a = A[7];
        mint t0p1 = t0 + t1, t2p3 = t2 + t3;
        mint t0m1 = (t0 - t1) * X, t2m3 = (t2 - t3) * Y;
        A[0] = (t0p1 + t2p3).a;
        A[1] = (t0m1 + t2m3).a;
        A[2] = ((t0p1 - t2p3) * W).a;
        A[3] = ((t0m1 - t2m3) * W).a;
        X *= dy[2], W *= dy[1], Y *= dy[2];
        mint t4p5 = t4 + t5, t6p7 = t6 + t7;
        mint t4m5 = (t4 - t5) * X, t6m7 = (t6 - t7) * Y;
        A[4] = (t4p5 + t6p7).a;
        A[5] = (t4m5 + t6m7).a;
        A[6] = ((t4p5 - t6p7) * W).a;
        A[7] = ((t4m5 - t6m7) * W).a;
        X *= dy[__builtin_ctzll(jh += 8)];
      }
      //*/
    } else if (v == 4) {
      u <<= 2;
      /**/
      mint W = one, X = one, Y = one;

      for (int jh = 0; jh < u;) {
        W = X * X, Y = X * imag;
        mmint c23{X.a, X.a, X.a, X.a, Y.a, Y.a, Y.a, Y.a};
        mmint Ws{W.a};
        mmint t01 = a[(jh >> 1) | 0];
        mmint t23 = a[(jh >> 1) | 1];
        mmint t02 = _mm256_permute2x128_si256(t01, t23, 0x20);
        mmint t13 = _mm256_permute2x128_si256(t01, t23, 0x31);
        mmint tp = t02 + t13, tm = (t02 - t13) * c23;
        mmint u0 = _mm256_permute2x128_si256(tp, tm, 0x20);
        mmint u1 = _mm256_permute2x128_si256(tp, tm, 0x31);
        a[(jh >> 1) | 0] = u0 + u1;
        a[(jh >> 1) | 1] = (u0 - u1) * Ws;
        X *= dy[__builtin_ctzll((jh += 4))];
      }
      //*/
    } else {
      int v8 = v / 8;
      u <<= 2;
      {
        int j0 = 0, j1 = v8, j2 = j1 + v8, j3 = j2 + v8;
        int je = v8 - 1;
        mmint t0 = a[j0], t1 = a[j1];
        mmint t2 = a[j2], t3 = a[j3];
        for (; j0 < je; ++j0, ++j1, ++j2, ++j3) {
          mmint t0p1 = t0 + t1, t2p3 = t2 + t3;
          mmint t0m1 = t0 - t1, t2m3 = (t2 - t3) * IMAG;
          t0 = a[j0 + 1], t1 = a[j1 + 1];
          t2 = a[j2 + 1], t3 = a[j3 + 1];
          a[j0] = t0p1 + t2p3, a[j1] = t0m1 + t2m3;
          a[j2] = t0p1 - t2p3, a[j3] = t0m1 - t2m3;
        }
        mmint t0p1 = t0 + t1, t2p3 = t2 + t3;
        mmint t0m1 = t0 - t1, t2m3 = (t2 - t3) * IMAG;
        a[j0] = t0p1 + t2p3, a[j1] = t0m1 + t2m3;
        a[j2] = t0p1 - t2p3, a[j3] = t0m1 - t2m3;
      }
      mmint W{one.a}, X{dy[2].a}, Y{one.a};
      for (int jh = 4; jh < u;) {
        W = X * X, Y = X * IMAG;
        int j0 = jh * v8, j1 = j0 + v8, j2 = j1 + v8, j3 = j2 + v8;
        int je = j0 + v8 - 1;
        mmint t0 = a[j0], t1 = a[j1];
        mmint t2 = a[j2], t3 = a[j3];
        for (; j0 < je; ++j0, ++j1, ++j2, ++j3) {
          mmint t0p1 = t0 + t1, t2p3 = t2 + t3;
          mmint t0m1 = (t0 - t1) * X, t2m3 = (t2 - t3) * Y;
          t0 = a[j0 + 1], t1 = a[j1 + 1];
          t2 = a[j2 + 1], t3 = a[j3 + 1];
          a[j0] = t0p1 + t2p3, a[j1] = t0m1 + t2m3;
          a[j2] = (t0p1 - t2p3) * W, a[j3] = (t0m1 - t2m3) * W;
        }
        mmint t0p1 = t0 + t1, t2p3 = t2 + t3;
        mmint t0m1 = (t0 - t1) * X, t2m3 = (t2 - t3) * Y;
        a[j0] = t0p1 + t2p3, a[j1] = t0m1 + t2m3;
        a[j2] = (t0p1 - t2p3) * W, a[j3] = (t0m1 - t2m3) * W;
        X *= mmint{dy[__builtin_ctzll(jh += 4)].a};
      }
    }
    u >>= 4;
    v <<= 2;
  }
  if (k & 1) {
    u = 1 << (k - 1 - 3);
    for (int j = 0; j < u; ++j) {
      mmint ajv = a[j] - a[j + u];
      a[j] += a[j + u];
      a[j + u] = ajv;
    }
  }
}

vector<mint> multiply(const vector<mint>& a, const vector<mint>& b) {
  int l = a.size() + b.size() - 1;
  if (min<int>(a.size(), b.size()) < 20) {
    vector<mint> s(l);
    for (int i = 0; i < (int)a.size(); ++i)
      for (int j = 0; j < (int)b.size(); ++j) s[i + j] += a[i] * b[j];
    return s;
  }
  int k = 2, M = 4;
  while (M < l) M <<= 1, ++k;
  setwy(k);
  mmint *s = new mmint[M / 8], *t = new mmint[M / 8];
  memset((__attribute__((aligned(32))) int*)s, 0, sizeof(int) * M);
  memset((__attribute__((aligned(32))) int*)t, 0, sizeof(int) * M);
  for (int i = 0; i < (int)a.size(); ++i) s[i / 8][i % 8] = a[i].a;
  for (int i = 0; i < (int)b.size(); ++i) t[i / 8][i % 8] = b[i].a;
  ntt(s, k), ntt(t, k);
  for (int i = 0; i < M / 8; ++i) s[i] *= t[i];
  intt(s, k);
  mmint invm{(mint(M).inverse()).a};
  for (int i = 0; i < M / 8; i++) s[i] *= invm;
  vector<mint> res(M);
  std::memcpy(res.data(), (mint*)s, sizeof(int) * M);
  return res;
}

mmint a[1 << 17], b[1 << 17];

void inline_multiply(int M, int k) {
  setwy(k);
  ntt(a, k), ntt(b, k);
  for (int i = 0; i < M / 8; i += 4) {
    a[i + 0] *= b[i + 0];
    a[i + 1] *= b[i + 1];
    a[i + 2] *= b[i + 2];
    a[i + 3] *= b[i + 3];
  }
  intt(a, k);
}

void LC_convolution() {
  int n, m;
  unsigned int x, *p;

  rd(n, m);
  // exampleでサーバーを温める
  if (n == 4 or n == 1) {
    for (int N : vector<int>{270000, 70000, 17000}) {
      vector<mint> s(N), t(N);
      auto u = multiply(s, t);
    }
  }

  p = (unsigned int*)a;
  for (int i = 0; i < n; ++i, ++p) rd(x), *p = x;
  p = (unsigned int*)b;
  for (int i = 0; i < m; ++i, ++p) rd(x), *p = x;

  if (n + m < 32) {
    long long c[32] = {};
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        c[i + j] += 1LL * a[i / 8][i % 8] * b[j / 8][j % 8];
        c[i + j] %= 998244353;
      }
    }
    for (int i = 0; i < n + m - 1; i++) wt(c[i], " \n"[i == n + m - 2]);
    return;
  }

  int k = 2, M = 4, l = n + m - 1;
  while (M < l) M <<= 1, ++k;
  for (int i = 0; i < (n + 7) / 8; ++i) {
    a[i + 0] = mmint::itom(a[i + 0]);
  }
  for (int i = 0; i < (m + 7) / 8; ++i) {
    b[i + 0] = mmint::itom(b[i + 0]);
  }
  inline_multiply(M, k);
  mmint im{(mint(M).inverse()).a};
  for (int i = 0; i < M / 8; ++i) {
    a[i + 0] *= im;
    a[i + 0] = mmint::mtoi(a[i + 0]);
  }

  p = (unsigned int*)a;
  wt(*p);
  p++;
  for (int i = 1; i < l; i++, p++) {
    wt(' ');
    wt(*p);
  }
  wt('\n');
  return;
}
}

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

/**
 * Author: Andrew He
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Papers about accuracy: http://www.daemonology.net/papers/fft.pdf, http://www.cs.berkeley.edu/~fateman/papers/fftvsothers.pdf
 * For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$.
 */

namespace ecnerwala {
namespace fft {

using std::swap;
using std::vector;
using std::min;
using std::max;

template<class T> int sz(T&& arg) { using std::size; return int(size(std::forward<T>(arg))); }
inline int nextPow2(int s) { return 1 << (s > 1 ? 32 - __builtin_clz(s-1) : 0); }

// Complex
template <typename dbl> struct cplx { /// start-hash
	dbl x, y;
	cplx(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) { }
	friend cplx operator+(cplx a, cplx b) { return cplx(a.x + b.x, a.y + b.y); }
	friend cplx operator-(cplx a, cplx b) { return cplx(a.x - b.x, a.y - b.y); }
	friend cplx operator*(cplx a, cplx b) { return cplx(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
	friend cplx conj(cplx a) { return cplx(a.x, -a.y); }
	friend cplx inv(cplx a) { dbl n = (a.x*a.x+a.y*a.y); return cplx(a.x/n,-a.y/n); }
};

// getRoot implementations
template <typename num> struct getRoot {
	static num f(int k) = delete;
};
template <typename dbl> struct getRoot<cplx<dbl>> {
	static cplx<dbl> f(int k) {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
		dbl a=2*M_PI/k;
		return cplx<dbl>(cos(a),sin(a));
	}
};
template <int MOD> struct primitive_root {
	static const int value;
};
template <> struct primitive_root<998244353> {
	static const int value = 3;
};
template <int MOD> struct getRoot<modnum<MOD>> {
	static modnum<MOD> f(int k) {
		assert((MOD-1)%k == 0);
		return pow(modnum<MOD>(primitive_root<MOD>::value), (MOD-1)/k);
	}
};

template <typename num> class fft {
	static vector<int> rev;
	static vector<num> rt;

public:
	static void init(int n);
	template <typename Iterator> static void go(Iterator begin, int n);

	static vector<num> scratch_a;
	static vector<num> scratch_b;
};

template <typename num> vector<int> fft<num>::rev;
template <typename num> vector<num> fft<num>::rt;
template <typename num> vector<num> fft<num>::scratch_a;
template <typename num> vector<num> fft<num>::scratch_b;

template <typename num> void fft<num>::init(int n) {
	if (n <= sz(rt)) return;
	rev.resize(n);
	for (int i = 0; i < n; i++) {
		rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
	}
	rt.reserve(n);
	while (sz(rt) < 2 && sz(rt) < n) rt.push_back(num(1));
	for (int k = sz(rt); k < n; k *= 2) {
		rt.resize(2*k);
		num z = getRoot<num>::f(2*k);
		for (int i = k/2; i < k; i++) {
			rt[2*i] = rt[i], rt[2*i+1] = rt[i]*z;
		}
	}
}

template <typename num> template <typename Iterator> void fft<num>::go(Iterator begin, int n) {
	init(n);
	int s = __builtin_ctz(sz(rev)/n);
	for (int i = 0; i < n; i++) {
		if (i < (rev[i]>>s)) {
			swap(*(begin+i), *(begin+(rev[i]>>s)));
		}
	}
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i < n; i += 2 * k) {
			Iterator it1 = begin + i, it2 = it1 + k;
			for (int j = 0; j < k; j++, ++it1, ++it2) {
				num t = rt[j+k] * *it2;
				*it2 = *it1 - t;
				*it1 = *it1 + t;
			}
		}
	}
}

template <typename num> struct fft_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void convolve_circular(int n, IterA ia, int sza, IterB ib, int szb, IterOut io, int szo) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		assert((n & (n - 1)) == 0);
		if (n == 0) return;
		assert(sza <= n);
		assert(szb <= n);
		assert(szo <= n);

		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);
		fft<num>::init(n);

		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		copy(ib, ib+szb, fb.begin());
		fill(fb.begin()+szb, fb.begin()+n, num(0));
		fft<num>::go(fa.begin(), n);
		fft<num>::go(fb.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);

		copy(fa.begin(), fa.begin()+szo, io);
	}

	template <typename IterA, typename IterOut>
	static void square_circular(int n, IterA ia, int sza, IterOut io, int szo) {
		vector<num>& fa = fft<num>::scratch_a;

		assert((n & (n - 1)) == 0);
		if (n == 0) return;
		assert(sza <= n);
		assert(szo <= n);

		if (sz(fa) < n) fa.resize(n);
		fft<num>::init(n);

		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		fft<num>::go(fa.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fa[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);

		copy(fa.begin(), fa.begin()+szo, io);
	}

	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);
		fft<num>::init(n);

		bool did_cut = false;
		if (sza > 1 && szb > 1 && n == 2 * (s - 1)) {
			// we have exactly 1 wraparound, so let's just handle it explicitly to save a factor of 2
			// only do it if sza < s and szb < s so we don't have to wrap the inputs
			did_cut = true;
			n /= 2;
		}
		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		copy(ib, ib+szb, fb.begin());
		fill(fb.begin()+szb, fb.begin()+n, num(0));
		// used if did_cut
		num v_init; if (did_cut) { v_init = fa[0] * fb[0]; }
		fft<num>::go(fa.begin(), n);
		fft<num>::go(fb.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);
		if (did_cut) {
			fa[s-1] = std::exchange(fa[0], v_init) - v_init;
		}
		copy(fa.begin(), fa.begin()+s, io);
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <typename num>
struct fft_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0) return;
		int s = nextPow2(sza) * 2;
		fft<num>::init(s);
		if (sz(fa) < s) fa.resize(s);
		if (sz(fb) < s) fb.resize(s);
		fb[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			fill(fb.begin() + n, fb.begin() + 4 * n, num(0));
			n *= 2;
			copy(ia, ia+min(n,sza), fa.begin());
			fill(fa.begin()+min(n,sza), fa.begin()+2*n, 0);
			fft<num>::go(fb.begin(), 2*n);
			fft<num>::go(fa.begin(), 2*n);
			num d = inv(num(2*n));
			for (int i = 0; i < 2*n; i++) fb[i] = fb[i] * (2 - fa[i] * fb[i]) * d;
			reverse(fb.begin()+1, fb.begin()+2*n);
			fft<num>::go(fb.begin(), 2*n);
		}
		copy(fb.begin(), fb.begin()+sza, io);
	}
};

template <typename dbl>
struct fft_double_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<cplx<dbl>>& fa = fft<cplx<dbl>>::scratch_a;
		vector<cplx<dbl>>& fb = fft<cplx<dbl>>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cplx<dbl>>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		fill(fa.begin(), fa.begin() + n, 0);
		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i].x = *it; }
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fa[i].y = *it; }
		fft<cplx<dbl>>::go(fa.begin(), n);
		for (auto& x : fa) x = x * x;
		for (int i = 0; i < n; ++i) fb[i] = fa[(n-i)&(n-1)] - conj(fa[i]);
		fft<cplx<dbl>>::go(fb.begin(), n);
		{ auto it = io; for (int i = 0; i < s; ++i, ++it) *it = fb[i].y / (4*n); }
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <typename mnum>
struct fft_mod_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		using cnum = cplx<double>;
		vector<cnum>& fa = fft<cnum>::scratch_a;
		vector<cnum>& fb = fft<cnum>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cnum>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fa.begin()+sza, fa.begin() + n, 0);
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fb[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fb.begin()+szb, fb.begin() + n, 0);

		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		double r0 = 0.5 / n; // 1/2n
		for (int i = 0; i <= n/2; i++) {
			int j = (n-i)&(n-1);
			cnum g0 = (fb[i] + conj(fb[j])) * r0;
			cnum g1 = (fb[i] - conj(fb[j])) * r0;
			swap(g1.x, g1.y); g1.y *= -1;
			if (j != i) {
				swap(fa[j], fa[i]);
				fb[j] = fa[j] * g1;
				fa[j] = fa[j] * g0;
			}
			fb[i] = fa[i] * conj(g1);
			fa[i] = fa[i] * conj(g0);
		}
		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		using ll = long long;
		const ll m = mnum::MOD;
		auto it = io;
		for (int i = 0; i < s; ++i, ++it) {
			*it = mnum((ll(fa[i].x+0.5)
						+ (ll(fa[i].y+0.5) % m << 15)
						+ (ll(fb[i].x+0.5) % m << 15)
						+ (ll(fb[i].y+0.5) % m << 30)) % m);
		}
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <class multiplier, typename num>
struct multiply_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		if (sza == 0) return;
		int s = nextPow2(sza);
		vector<num> b(s,num(0));
		vector<num> tmp(2*s);
		b[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			multiplier::square(b.begin(),n,tmp.begin());
			int nn = min(sza,2*n);
			multiplier::multiply(tmp.begin(),nn,ia,nn,tmp.begin());
			for (int i = n; i < nn; i++) b[i] = -tmp[i];
			n = nn;
		}
		copy(b.begin(), b.begin()+sza, io);
	}
};

template <class multiplier, typename T> vector<T> multiply(const vector<T>& a, const vector<T>& b) {
	if (sz(a) == 0 || sz(b) == 0) return {};
	vector<T> r(sz(a) + sz(b) - 1);
	multiplier::multiply(begin(a), sz(a), begin(b), sz(b), begin(r));
	return r;
}

template <typename T> vector<T> fft_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_double_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_double_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_mod_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_mod_multiplier<T>, T>(a, b);
}

template <class multiplier, typename T> vector<T> square(const vector<T>& a) {
	if (sz(a) == 0) return {};
	vector<T> r(2 * sz(a) - 1);
	multiplier::square(begin(a), sz(a), begin(r));
	return r;
}
template <typename T> vector<T> fft_square(const vector<T>& a) {
	return square<fft_multiplier<T>, T>(a);
}
template <typename T> vector<T> fft_double_square(const vector<T>& a) {
	return square<fft_double_multiplier<T>, T>(a);
}
template <typename T> vector<T> fft_mod_square(const vector<T>& a) {
	return square<fft_mod_multiplier<T>, T>(a);
}

template <class inverser, typename T> vector<T> inverse(const vector<T>& a) {
	vector<T> r(sz(a));
	inverser::inverse(begin(a), sz(a), begin(r));
	return r;
}
template <typename T> vector<T> fft_inverse(const vector<T>& a) {
	return inverse<fft_inverser<T>, T>(a);
}
template <typename T> vector<T> fft_double_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_double_multiplier<T>, T>, T>(a);
}
template <typename T> vector<T> fft_mod_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_mod_multiplier<T>, T>, T>(a);
}
/* namespace fft */ }

// Power series; these are assumed to be the min of the length
template <typename T, typename multiplier, typename inverser>
struct power_series : public std::vector<T> {
	using std::vector<T>::vector;

	int ssize() const {
		return int(this->size());
	}
	int len() const {
		return ssize();
	}
	int degree() const {
		return len() - 1;
	}
	void extend(int sz) {
		assert(sz >= ssize());
		this->resize(sz);
	}
	void shrink(int sz) {
		assert(sz <= ssize());
		this->resize(sz);
	}
	// multiply by x^n
	void shift(int n = 1) {
		assert(n >= 0 && n <= ssize());
		std::rotate(this->begin(), this->end()-n, this->end());
		std::fill(this->begin(), this->begin()+n, T(0));
	}
	// divide by x^n and 0-pad
	void unshift(int n = 1) {
		assert(n >= 0 && n <= ssize());
		std::fill(this->begin(), this->begin()+n, T(0));
		std::rotate(this->begin(), this->begin()+n, this->end());
	}
	power_series& operator += (const power_series& o) {
		assert(len() == o.len());
		for (int i = 0; i < int(o.size()); i++) {
			(*this)[i] += o[i];
		}
		return *this;
	}
	friend power_series operator + (const power_series& a, const power_series& b) {
		power_series r(std::min(a.size(), b.size()));
		for (int i = 0; i < r.len(); i++) {
			r[i] = a[i] + b[i];
		}
		return r;
	}
	power_series& operator -= (const power_series& o) {
		assert(len() == o.len());
		for (int i = 0; i < int(o.size()); i++) {
			(*this)[i] -= o[i];
		}
		return *this;
	}
	friend power_series operator - (const power_series& a, const power_series& b) {
		power_series r(std::min(a.size(), b.size()));
		for (int i = 0; i < r.len(); i++) {
			r[i] = a[i] - b[i];
		}
		return r;
	}

	power_series& operator *= (const T& n) {
		for (auto& v : *this) v *= n;
		return *this;
	}
	friend power_series operator * (const power_series& a, const T& n) {
		power_series r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = a[i] * n;
		}
		return r;
	}
	friend power_series operator * (const T& n, const power_series& a) {
		power_series r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = n * a[i];
		}
		return r;
	}

	friend power_series operator * (const power_series& a, const power_series& b) {
		if (sz(a) == 0 || sz(b) == 0) return {};
		power_series r(std::max(0, sz(a) + sz(b) - 1));
		multiplier::multiply(begin(a), sz(a), begin(b), sz(b), begin(r));
		r.resize(std::min(a.size(), b.size()));
		return r;
	}
	power_series& operator *= (const power_series& o) {
		return *this = (*this) * o;
	}
	friend power_series square(const power_series& a) {
		if (sz(a) == 0) return {};
		power_series r(sz(a) * 2 - 1);
		multiplier::square(begin(a), sz(a), begin(r));
		r.resize(a.size());
		return r;
	}

	friend power_series stretch(const power_series& a, int n) {
		power_series r(a.size());
		for (int i = 0; i*n < int(a.size()); i++) {
			r[i*n] = a[i];
		}
		return r;
	}
	friend power_series inverse(power_series a) {
		power_series r(sz(a));
		inverser::inverse(begin(a), sz(a), begin(r));
		return r;
	}
	friend power_series deriv_shift(power_series a) {
		for (int i = 0; i < a.len(); i++) {
			a[i] *= i;
		}
		return a;
	}
	friend power_series integ_shift(power_series a) {
		assert(a[0] == 0);
		T f = 1;
		for (int i = 1; i < int(a.size()); i++) {
			a[i] *= f;
			f *= i;
		}
		f = inv(f);
		for (int i = int(a.size()) - 1; i > 0; i--) {
			a[i] *= f;
			f *= i;
		}
		return a;
	}
	friend power_series deriv_shift_log(power_series a) {
		auto r = deriv_shift(a);
		return r * inverse(a);
	}
	friend power_series poly_log(power_series a) {
		assert(a[0] == 1);
		return integ_shift(deriv_shift_log(std::move(a)));
	}
	friend power_series poly_exp(power_series a) {
		assert(a.size() >= 1);
		assert(a[0] == 0);
		power_series r(1, T(1));
		while (r.size() < a.size()) {
			int n_sz = std::min(int(r.size()) * 2, int(a.size()));
			r.resize(n_sz);
			power_series v(a.begin(), a.begin() + n_sz);
			v -= poly_log(r);
			v[0] += 1;
			r *= v;
		}
		return r;
	}
	friend power_series poly_pow_monic(power_series a, int64_t k) {
		if (a.empty()) return a;
		assert(a.size() >= 1);
		assert(a[0] == 1);
		a = poly_log(a);
		a *= k;
		return poly_exp(a);
	}
	friend power_series poly_pow(power_series a, int64_t k) {
		int st = 0;
		while (st < a.len() && a[st] == 0) st++;
		if (st == a.len()) return a;

		power_series r(a.begin() + st, a.end());
		T leading_coeff = r[0];
		r *= inv(leading_coeff);
		r = poly_pow_monic(r, k);
		r *= pow(leading_coeff, k);
		r.insert(r.begin(), size_t(st), T(0));
		return r;
	}

	friend power_series to_newton_sums(const power_series& a, int deg) {
		auto r = log_deriv_shift(a);
		r[0] = deg;
		for (int i = 1; i < int(r.size()); i++) r[i] = -r[i];
		return r;
	}
	friend power_series from_newton_sums(power_series S, int deg) {
		assert(S[0] == int(deg));
		S[0] = 0;
		for (int i = 1; i < int(S.size()); i++) S[i] = -S[i];
		return poly_exp(integ_shift(std::move(S)));
	}

	// Calculates prod 1/(1-x^i)^{a[i]}
	friend power_series euler_transform(const power_series& a) {
		power_series r = deriv_shift(a);
		std::vector<bool> is_prime(a.size(), true);
		for (int p = 2; p < int(a.size()); p++) {
			if (!is_prime[p]) continue;
			for (int i = 1; i*p < int(a.size()); i++) {
				r[i*p] += r[i];
				is_prime[i*p] = false;
			}
		}
		return poly_exp(integ_shift(r));
	}
	friend power_series inverse_euler_transform(const power_series& a) {
		power_series r = deriv_shift(poly_log(a));
		std::vector<bool> is_prime(a.size(), true);
		for (int p = 2; p < int(a.size()); p++) {
			if (!is_prime[p]) continue;
			for (int i = (int(a.size())-1)/p; i >= 1; i--) {
				r[i*p] -= r[i];
				is_prime[i*p] = false;
			}
		}
		return integ_shift(r);
	}
};

template <typename num> using power_series_fft = power_series<num, fft::fft_multiplier<num>, fft::fft_inverser<num>>;
template <typename num, typename multiplier> using power_series_with_multiplier = power_series<num, multiplier, fft::multiply_inverser<multiplier, num>>;
template <typename num> using power_series_fft_mod = power_series_with_multiplier<num, fft::fft_mod_multiplier<num>>;
template <typename num> using power_series_fft_double = power_series_with_multiplier<num, fft::fft_double_multiplier<num>>;

// TODO: Use iterator traits to deduce value type?
template <typename base_iterator, typename value_type> struct add_into_iterator {
	base_iterator base;
	add_into_iterator() : base() {}
	add_into_iterator(base_iterator b) : base(b) {}
	add_into_iterator& operator * () { return *this; }
	add_into_iterator& operator ++ () { base.operator ++ (); return *this; }
	add_into_iterator& operator ++ (int) { auto temp = *this; operator ++ (); return temp; }
	auto operator = (value_type v) { base.operator * () += v; }
};

// TODO: Use iterator traits to deduce value type?
template <typename base_iterator, typename value_type> struct add_double_into_iterator {
	base_iterator base;
	add_double_into_iterator() : base() {}
	add_double_into_iterator(base_iterator b) : base(b) {}
	add_double_into_iterator& operator * () { return *this; }
	add_double_into_iterator& operator ++ () { base.operator ++ (); return *this; }
	add_double_into_iterator& operator ++ (int) { auto temp = *this; operator ++ (); return temp; }
	auto operator = (value_type v) { base.operator * () += 2 * v; }
};

template <typename num, typename multiplier> struct online_multiplier {
	int N; int i;
	std::vector<num> f, g;
	std::vector<num> res;

	// Computes the first 2N terms of the product
	online_multiplier(int N_) : N(N_), i(0), f(N), g(N), res(2*N+1, num(0)) {}

	num peek() {
		return res[i];
	}

	void push(num v_f, num v_g) {
		assert(i < N);
		f[i] = v_f;
		g[i] = v_g;
		if (i == 0) {
			res[i] += v_f * v_g;
		} else {
			res[i] += v_f * g[0];
			res[i] += f[0] * v_g;
			// TODO: We could do this second half more lazily, since it only affects res[i+1]...
			for (int p = 1; (i & (p-1)) == (p-1); p <<= 1) {
				int lo1 = p;
				int lo2 = i + 1 - p;
				multiplier::multiply(
					// TODO: We can cache FFT([f,g].begin() + p, p)
					f.begin() + lo1, p,
					g.begin() + lo2, p,
					add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
				);
				if (i == 2*p-1) break;
				// TODO: Don't recompute if squaring
				multiplier::multiply(
					f.begin() + lo2, p,
					g.begin() + lo1, p,
					add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
				);
			}
		}
		i++;
	}

	num back() {
		return res[i-1];
	}
};

template <typename num, typename multiplier> struct online_squarer {
	int N; int i;
	std::vector<num> f;
	std::vector<num> res;

	// Computes the first 2N terms of the product
	online_squarer(int N_) : N(N_), i(0), f(N), res(2*N+1, num(0)) {}

	num peek() {
		return res[i];
	}

	void push(num v_f) {
		assert(i < N);
		f[i] = v_f;
		if (i == 0) {
			res[i] += v_f * v_f;
		} else {
			res[i] += 2 * v_f * f[0];
			// TODO: We could do this second half more lazily, since it only affects res[i+1]...
			for (int p = 1; (i & (p-1)) == (p-1); p <<= 1) {
				int lo1 = p;
				int lo2 = i + 1 - p;
				if (i == 2*p-1) {
					multiplier::square(
						// TODO: We can cache FFT([f,g].begin() + p, p)
						f.begin() + lo1, p,
						add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
					);
					break;
				} else {
					multiplier::multiply(
						// TODO: Use cached FFT
						f.begin() + lo1, p,
						f.begin() + lo2, p,
						add_double_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
					);
				}
			}
		}
		i++;
	}

	num back() {
		return res[i-1];
	}
};

/* namespace ecnerwala */ }

struct pt_t {
	int x, y;
	friend auto operator <=> (pt_t a, pt_t b) =default;
	friend std::ostream& operator << (std::ostream& o, pt_t p) { return o << '(' << p.x << ',' << p.y << ')'; }
};
struct seg_t {
	pt_t a, b;
	friend std::ostream& operator << (std::ostream& o, seg_t s) { return o << s.a << '-' << s.b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<998244353>;
	int N, Q; cin >> N >> Q;
	std::vector<num> A(N);
	for (auto& a : A) cin >> a;
	std::vector<num> B(N);
	for (auto& b : B) cin >> b;
	std::reverse(B.begin(), B.end());

	std::vector<seg_t> root_h_segs;
	std::vector<seg_t> root_v_segs;
	std::vector<seg_t> root_d_segs;
	for (int q = 0; q < Q; q++) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		x1--, y1--, x2--, y2--;
		y1 = N - 1 - y1;
		y2 = N - 1 - y2;
		pt_t a{x1, y1};
		pt_t b{x2, y2};
		if (a > b) std::swap(a, b);
		assert(a < b);
		if (x1 == x2) {
			root_h_segs.push_back({a, b});
		} else if (y1 == y2) {
			root_v_segs.push_back({a, b});
		} else if (x1 + y1 == x2 + y2) {
			root_d_segs.push_back({a, b});
		} else assert(false);
	}

	//int N_MAX = 1;
	//while (N_MAX <= N) N_MAX *= 2;
	int N_MAX = 1 << 17;

	int tot_sz = 0;
	std::map<int, int> by_sz;
	assert(N_MAX > N);
	cout << std::y_combinator([&](auto self,
		int x0, int y0,
		const std::vector<num>& hs,
		const std::vector<num>& vs,
		const std::vector<int8_t>& ds,
		const std::vector<seg_t>& h_segs,
		const std::vector<seg_t>& v_segs,
		const std::vector<seg_t>& d_segs
	) -> num {
		int L = int(hs.size());
		assert((L & (L-1)) == 0);
		assert(L == int(vs.size()));
		assert(2 * L - 1 == int(ds.size()));
		if (h_segs.empty() && v_segs.empty() && d_segs.empty()) {
			by_sz[L]++;
			tot_sz += L * __builtin_ctz(L);
			num ans = 0;
			if (L >= 128) {
				auto r = ecnerwala::fft::fft_multiply(hs, vs);
				assert(int(r.size()) == 2*L-1);
				for (int z = 0; z < 2*L-1; z++) ans += ds[z] ? r[z] : 0;
			} else {
				for (int a = 0; a < L; a++) {
					int64_t v = 0;
					for (int b = 0; b < L; b++) {
						v += ds[a+b] ? int(vs[b]) : 0;
					}
					ans += hs[a] * num(v);
				}
			}
			//for (int z = 0; z < L; z++) ans += ds[z] * hs[z] * vs[z];
			return ans;
		}
		assert(L >= 2);
		assert(L % 2 == 0);
		num ans = 0;
		for (int nx : {x0, x0 + L/2}) {
			for (int ny : {y0, y0 + L/2}) {
				std::vector<num> nhs(hs.begin() + (nx - x0), hs.begin() + (nx - x0 + L/2));
				std::vector<num> nvs(vs.begin() + (ny - y0), vs.begin() + (ny - y0 + L/2));
				std::vector<int8_t> nds(ds.begin() + (nx - x0 + ny - y0), ds.begin() + (nx - x0 + ny - y0 + (L/2 * 2 - 1)));
				std::vector<seg_t> n_h_segs; n_h_segs.reserve(h_segs.size());
				for (auto [a, b] : h_segs) {
					assert(a.x == b.x);
					int x = a.x;
					assert(a.y <= b.y);
					if (nx <= x && x < nx + L/2) {
						int y_lo = std::max(a.y, ny);
						int y_hi = std::min(b.y, ny + L/2 - 1);
						if (y_lo == ny && y_hi == ny + L/2 - 1) {
							// set equal to avoid double counting
							nhs[x - nx] = x < N ? A[x] : 0;
						} else if (y_lo <= y_hi) {
							n_h_segs.push_back({{x, y_lo}, {x, y_hi}});
						}
					}
				}
				std::vector<seg_t> n_v_segs; n_v_segs.reserve(v_segs.size());
				for (auto [a, b] : v_segs) {
					assert(a.y == b.y);
					int y = a.y;
					assert(a.x <= b.x);
					if (ny <= y && y < ny + L/2) {
						int x_lo = std::max(a.x, nx);
						int x_hi = std::min(b.x, nx + L/2 - 1);
						if (x_lo == nx && x_hi == nx + L/2 - 1) {
							nvs[y - ny] = y < N ? B[y] : 0;
						} else if (x_lo <= x_hi) {
							n_v_segs.push_back({{x_lo, y}, {x_hi, y}});
						}
					}
				}
				std::vector<seg_t> n_d_segs; n_d_segs.reserve(d_segs.size());
				for (auto [a, b] : d_segs) {
					assert(a.x + a.y == b.x + b.y);
					int s = a.x + a.y;
					assert(a.x <= b.x);
					if (nx + ny <= s && s <= nx + ny + 2 * (L/2 - 1)) {
						int n_lo = std::max(nx, s - ny - (L/2 - 1));
						int n_hi = std::min(nx + (L/2 - 1), s - ny);
						int x_lo = std::max(n_lo, a.x);
						int x_hi = std::min(n_hi, b.x);
						if (x_lo == n_lo && x_hi == n_hi) {
							nds[s - nx - ny] = 1;
						} else if (x_lo <= x_hi) {
							n_d_segs.push_back({{x_lo, s - x_lo}, {x_hi, s - x_hi}});
						}
					}
				}
				ans += self(nx, ny, nhs, nvs, nds, n_h_segs, n_v_segs, n_d_segs);
			}
		}
		return ans;
	})(
		0, 0,
		std::vector<num>(N_MAX, 0),
		std::vector<num>(N_MAX, 0),
		std::vector<int8_t>(N_MAX * 2 - 1, 0),
		root_h_segs,
		root_v_segs,
		root_d_segs
	) << '\n';
	cerr << "tot_sz " << tot_sz << '\n';
	for (auto [k, v] : by_sz) {
		cerr << " sz " << k << ' ' << v << '\n';
	}

	return 0;
}
