#include <bits/stdc++.h>

using namespace std;

#line 2 "modint/montgomery-modint.hpp"

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
  static_assert(mod < (1 << 30), "invalid, mod >= 2 ^ 30");
  static_assert((mod & 1) == 1, "invalid, mod % 2 == 0");
  static_assert(r * mod == 1, "this code has bugs.");

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
  constexpr mint operator+() const { return mint(*this); }

  constexpr mint pow(u64 n) const {
    mint ret(1), mul(*this);
    while (n > 0) {
      if (n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }

  constexpr mint inverse() const {
    int x = get(), y = mod, u = 1, v = 0, t = 0, tmp = 0;
    while (y > 0) {
      t = x / y;
      x -= t * y, u -= t * v;
      tmp = x, x = y, y = tmp;
      tmp = u, u = v, v = tmp;
    }
    return mint{u};
  }

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

#line 2 "fps/kitamasa.hpp"

#line 2 "fps/formal-power-series.hpp"

template <typename mint>
struct FormalPowerSeries : vector<mint> {
  using vector<mint>::vector;
  using FPS = FormalPowerSeries;

  FPS &operator+=(const FPS &r) {
    if (r.size() > this->size()) this->resize(r.size());
    for (int i = 0; i < (int)r.size(); i++) (*this)[i] += r[i];
    return *this;
  }

  FPS &operator+=(const mint &r) {
    if (this->empty()) this->resize(1);
    (*this)[0] += r;
    return *this;
  }

  FPS &operator-=(const FPS &r) {
    if (r.size() > this->size()) this->resize(r.size());
    for (int i = 0; i < (int)r.size(); i++) (*this)[i] -= r[i];
    return *this;
  }

  FPS &operator-=(const mint &r) {
    if (this->empty()) this->resize(1);
    (*this)[0] -= r;
    return *this;
  }

  FPS &operator*=(const mint &v) {
    for (int k = 0; k < (int)this->size(); k++) (*this)[k] *= v;
    return *this;
  }

  FPS &operator/=(const FPS &r) {
    if (this->size() < r.size()) {
      this->clear();
      return *this;
    }
    int n = this->size() - r.size() + 1;
    if ((int)r.size() <= 64) {
      FPS f(*this), g(r);
      g.shrink();
      mint coeff = g.back().inverse();
      for (auto &x : g) x *= coeff;
      int deg = (int)f.size() - (int)g.size() + 1;
      int gs = g.size();
      FPS quo(deg);
      for (int i = deg - 1; i >= 0; i--) {
        quo[i] = f[i + gs - 1];
        for (int j = 0; j < gs; j++) f[i + j] -= quo[i] * g[j];
      }
      *this = quo * coeff;
      this->resize(n, mint(0));
      return *this;
    }
    return *this = ((*this).rev().pre(n) * r.rev().inv(n)).pre(n).rev();
  }

  FPS &operator%=(const FPS &r) {
    *this -= *this / r * r;
    shrink();
    return *this;
  }

  FPS operator+(const FPS &r) const { return FPS(*this) += r; }
  FPS operator+(const mint &v) const { return FPS(*this) += v; }
  FPS operator-(const FPS &r) const { return FPS(*this) -= r; }
  FPS operator-(const mint &v) const { return FPS(*this) -= v; }
  FPS operator*(const FPS &r) const { return FPS(*this) *= r; }
  FPS operator*(const mint &v) const { return FPS(*this) *= v; }
  FPS operator/(const FPS &r) const { return FPS(*this) /= r; }
  FPS operator%(const FPS &r) const { return FPS(*this) %= r; }
  FPS operator-() const {
    FPS ret(this->size());
    for (int i = 0; i < (int)this->size(); i++) ret[i] = -(*this)[i];
    return ret;
  }

  void shrink() {
    while (this->size() && this->back() == mint(0)) this->pop_back();
  }

  FPS rev() const {
    FPS ret(*this);
    reverse(begin(ret), end(ret));
    return ret;
  }

  FPS dot(FPS r) const {
    FPS ret(min(this->size(), r.size()));
    for (int i = 0; i < (int)ret.size(); i++) ret[i] = (*this)[i] * r[i];
    return ret;
  }

  // 前 sz 項を取ってくる。sz に足りない項は 0 埋めする
  FPS pre(int sz) const {
    FPS ret(begin(*this), begin(*this) + min((int)this->size(), sz));
    if ((int)ret.size() < sz) ret.resize(sz);
    return ret;
  }

  FPS operator>>(int sz) const {
    if ((int)this->size() <= sz) return {};
    FPS ret(*this);
    ret.erase(ret.begin(), ret.begin() + sz);
    return ret;
  }

  FPS operator<<(int sz) const {
    FPS ret(*this);
    ret.insert(ret.begin(), sz, mint(0));
    return ret;
  }

  FPS diff() const {
    const int n = (int)this->size();
    FPS ret(max(0, n - 1));
    mint one(1), coeff(1);
    for (int i = 1; i < n; i++) {
      ret[i - 1] = (*this)[i] * coeff;
      coeff += one;
    }
    return ret;
  }

  FPS integral() const {
    const int n = (int)this->size();
    FPS ret(n + 1);
    ret[0] = mint(0);
    if (n > 0) ret[1] = mint(1);
    auto mod = mint::get_mod();
    for (int i = 2; i <= n; i++) ret[i] = (-ret[mod % i]) * (mod / i);
    for (int i = 0; i < n; i++) ret[i + 1] *= (*this)[i];
    return ret;
  }

  mint eval(mint x) const {
    mint r = 0, w = 1;
    for (auto &v : *this) r += w * v, w *= x;
    return r;
  }

  FPS log(int deg = -1) const {
    assert(!(*this).empty() && (*this)[0] == mint(1));
    if (deg == -1) deg = (int)this->size();
    return (this->diff() * this->inv(deg)).pre(deg - 1).integral();
  }

  FPS pow(int64_t k, int deg = -1) const {
    const int n = (int)this->size();
    if (deg == -1) deg = n;
    if (k == 0) {
      FPS ret(deg);
      if (deg) ret[0] = 1;
      return ret;
    }
    for (int i = 0; i < n; i++) {
      if ((*this)[i] != mint(0)) {
        mint rev = mint(1) / (*this)[i];
        FPS ret = (((*this * rev) >> i).log(deg) * k).exp(deg);
        ret *= (*this)[i].pow(k);
        ret = (ret << (i * k)).pre(deg);
        if ((int)ret.size() < deg) ret.resize(deg, mint(0));
        return ret;
      }
      if (__int128_t(i + 1) * k >= deg) return FPS(deg, mint(0));
    }
    return FPS(deg, mint(0));
  }

  static void *ntt_ptr;
  static void set_fft();
  FPS &operator*=(const FPS &r);
  void ntt();
  void intt();
  void ntt_doubling();
  static int ntt_pr();
  FPS inv(int deg = -1) const;
  FPS exp(int deg = -1) const;
};
template <typename mint>
void *FormalPowerSeries<mint>::ntt_ptr = nullptr;

/**
 * @brief 多項式/形式的冪級数ライブラリ
 * @docs docs/fps/formal-power-series.md
 */
#line 4 "fps/kitamasa.hpp"

template <typename mint>
mint LinearRecurrence(long long k, FormalPowerSeries<mint> Q,
                      FormalPowerSeries<mint> P) {
  Q.shrink();
  mint ret = 0;
  if (P.size() >= Q.size()) {
    auto R = P / Q;
    P -= R * Q;
    P.shrink();
    if (k < (int)R.size()) ret += R[k];
  }
  if ((int)P.size() == 0) return ret;

  FormalPowerSeries<mint>::set_fft();
  if (FormalPowerSeries<mint>::ntt_ptr == nullptr) {
    P.resize((int)Q.size() - 1);
    while (k) {
      auto Q2 = Q;
      for (int i = 1; i < (int)Q2.size(); i += 2) Q2[i] = -Q2[i];
      auto S = P * Q2;
      auto T = Q * Q2;
      if (k & 1) {
        for (int i = 1; i < (int)S.size(); i += 2) P[i >> 1] = S[i];
        for (int i = 0; i < (int)T.size(); i += 2) Q[i >> 1] = T[i];
      } else {
        for (int i = 0; i < (int)S.size(); i += 2) P[i >> 1] = S[i];
        for (int i = 0; i < (int)T.size(); i += 2) Q[i >> 1] = T[i];
      }
      k >>= 1;
    }
    return ret + P[0];
  } else {
    int N = 1;
    while (N < (int)Q.size()) N <<= 1;

    P.resize(2 * N);
    Q.resize(2 * N);
    P.ntt();
    Q.ntt();
    vector<mint> S(2 * N), T(2 * N);

    vector<int> btr(N);
    for (int i = 0, logn = __builtin_ctz(N); i < (1 << logn); i++) {
      btr[i] = (btr[i >> 1] >> 1) + ((i & 1) << (logn - 1));
    }
    mint dw = mint(FormalPowerSeries<mint>::ntt_pr())
                  .inverse()
                  .pow((mint::get_mod() - 1) / (2 * N));

    while (k) {
      mint inv2 = mint(2).inverse();

      // even degree of Q(x)Q(-x)
      T.resize(N);
      for (int i = 0; i < N; i++) T[i] = Q[(i << 1) | 0] * Q[(i << 1) | 1];

      S.resize(N);
      if (k & 1) {
        // odd degree of P(x)Q(-x)
        for (auto &i : btr) {
          S[i] = (P[(i << 1) | 0] * Q[(i << 1) | 1] -
                  P[(i << 1) | 1] * Q[(i << 1) | 0]) *
                 inv2;
          inv2 *= dw;
        }
      } else {
        // even degree of P(x)Q(-x)
        for (int i = 0; i < N; i++) {
          S[i] = (P[(i << 1) | 0] * Q[(i << 1) | 1] +
                  P[(i << 1) | 1] * Q[(i << 1) | 0]) *
                 inv2;
        }
      }
      swap(P, S);
      swap(Q, T);
      k >>= 1;
      if (k < N) break;
      P.ntt_doubling();
      Q.ntt_doubling();
    }
    P.intt();
    Q.intt();
    return ret + (P * (Q.inv()))[k];
  }
}

template <typename mint>
mint kitamasa(long long N, FormalPowerSeries<mint> Q,
              FormalPowerSeries<mint> a) {
  assert(!Q.empty() && Q[0] != 0);
  if (N < (int)a.size()) return a[N];
  assert((int)a.size() >= int(Q.size()) - 1);
  auto P = a.pre((int)Q.size() - 1) * Q;
  P.resize(Q.size() - 1);
  return LinearRecurrence<mint>(N, Q, P);
}

/**
 * @brief 線形漸化式の高速計算
 * @docs docs/fps/kitamasa.md
 */

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = LazyMontgomeryModInt<998244353>;

	int N, M; cin >> N >> M;
	assert(M >= N);
	M -= N;
	std::string S; S.reserve(N); cin >> S;
	assert(int(S.size()) == N);
	std::vector<int> fail(N+1);
	fail[0] = -1;
	for (int i = 0; i < N; i++) {
		int cur = fail[i];
		while (cur != -1 && S[i] != S[cur]) cur = fail[cur];
		fail[i+1] = cur+1;
	}

	using power_series = FormalPowerSeries<num>;
	power_series Q(N+1);
	Q[0] += 1;
	Q[1] -= 26;
	for (int a = fail[N]; a > 0; a = fail[a]) {
		Q[N - a] += 1;
		Q[N - a + 1] -= 26;
	}
	Q[N] += 1;

	power_series P(1);
	P[0] = 1;
	cout << LinearRecurrence(M, Q, P) << '\n';

	return 0;
}
