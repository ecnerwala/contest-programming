#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <chrono>
#include <complex>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using Int = long long;

template <class T1, class T2> ostream &operator<<(ostream &os, const pair<T1, T2> &a) { return os << "(" << a.first << ", " << a.second << ")"; };
template <class T> ostream &operator<<(ostream &os, const vector<T> &as) { const int sz = as.size(); os << "["; for (int i = 0; i < sz; ++i) { if (i >= 256) { os << ", ..."; break; } if (i > 0) { os << ", "; } os << as[i]; } return os << "]"; }
template <class T> void pv(T a, T b) { for (T i = a; i != b; ++i) cerr << *i << " "; cerr << endl; }
template <class T> bool chmin(T &t, const T &f) { if (t > f) { t = f; return true; } return false; }
template <class T> bool chmax(T &t, const T &f) { if (t < f) { t = f; return true; } return false; }
#define COLOR(s) ("\x1b[" s "m")


int sig(Int r) { return (r < 0) ? -1 : (r > 0) ? +1 : 0; }
Int sq(Int r) { return r * r; }
struct Pt {
  Int x, y;
  Pt() : x(0), y(0) {}
  Pt(Int x_, Int y_) : x(x_), y(y_) {}
  Pt operator+(const Pt &a) const { return Pt(x + a.x, y + a.y); }
  Pt operator-(const Pt &a) const { return Pt(x - a.x, y - a.y); }
  Pt operator*(const Pt &a) const { return Pt(x * a.x - y * a.y, x * a.y + y * a.x); }
  Pt operator+() const { return Pt(+x, +y); }
  Pt operator-() const { return Pt(-x, -y); }
  Pt operator*(const Int &k) const { return Pt(x * k, y * k); }
  Pt operator/(const Int &k) const { return Pt(x / k, y / k); }
  friend Pt operator*(const Int &k, const Pt &a) { return Pt(k * a.x, k * a.y); }
  Pt &operator+=(const Pt &a) { x += a.x; y += a.y; return *this; }
  Pt &operator-=(const Pt &a) { x -= a.x; y -= a.y; return *this; }
  Pt &operator*=(const Pt &a) { return *this = *this * a; }
  Pt &operator*=(const Int &k) { x *= k; y *= k; return *this; }
  Pt &operator/=(const Int &k) { x /= k; y /= k; return *this; }
  Int abs2() const { return x * x + y * y; }
  Int dot(const Pt &a) const { return x * a.x + y * a.y; }
  Int det(const Pt &a) const { return x * a.y - y * a.x; }
  bool operator==(const Pt &a) const { return (x == a.x && y == a.y); }
  bool operator!=(const Pt &a) const { return !(x == a.x && y == a.y); }
  bool operator<(const Pt &a) const { return ((x != a.x) ? (x < a.x) : (y < a.y)); }
  friend ostream &operator<<(ostream &os, const Pt &a) { os << "(" << a.x << ", " << a.y << ")"; return os; }
};
Int tri(const Pt &a, const Pt &b, const Pt &c) { return (b - a).det(c - a); }

// [0, 2 PI)
int cmpArg(const Pt &a, const Pt &b) {
  const int sa = (a.y > 0) ? 1 : (a.y < 0) ? 3 : (a.x > 0) ? 0 : 2;
  const int sb = (b.y > 0) ? 1 : (b.y < 0) ? 3 : (b.x > 0) ? 0 : 2;
  return (sa < sb) ? -1 : (sa > sb) ? +1 : sig(b.det(a));
}


////////////////////////////////////////////////////////////////////////////////
template <unsigned M_> struct ModInt {
  static constexpr unsigned M = M_;
  unsigned x;
  constexpr ModInt() : x(0U) {}
  constexpr ModInt(unsigned x_) : x(x_ % M) {}
  constexpr ModInt(unsigned long long x_) : x(x_ % M) {}
  constexpr ModInt(int x_) : x(((x_ %= static_cast<int>(M)) < 0) ? (x_ + static_cast<int>(M)) : x_) {}
  constexpr ModInt(long long x_) : x(((x_ %= static_cast<long long>(M)) < 0) ? (x_ + static_cast<long long>(M)) : x_) {}
  ModInt &operator+=(const ModInt &a) { x = ((x += a.x) >= M) ? (x - M) : x; return *this; }
  ModInt &operator-=(const ModInt &a) { x = ((x -= a.x) >= M) ? (x + M) : x; return *this; }
  ModInt &operator*=(const ModInt &a) { x = (static_cast<unsigned long long>(x) * a.x) % M; return *this; }
  ModInt &operator/=(const ModInt &a) { return (*this *= a.inv()); }
  ModInt pow(long long e) const {
    if (e < 0) return inv().pow(-e);
    ModInt a = *this, b = 1U; for (; e; e >>= 1) { if (e & 1) b *= a; a *= a; } return b;
  }
  ModInt inv() const {
    unsigned a = M, b = x; int y = 0, z = 1;
    for (; b; ) { const unsigned q = a / b; const unsigned c = a - q * b; a = b; b = c; const int w = y - static_cast<int>(q) * z; y = z; z = w; }
    assert(a == 1U); return ModInt(y);
  }
  ModInt operator+() const { return *this; }
  ModInt operator-() const { ModInt a; a.x = x ? (M - x) : 0U; return a; }
  ModInt operator+(const ModInt &a) const { return (ModInt(*this) += a); }
  ModInt operator-(const ModInt &a) const { return (ModInt(*this) -= a); }
  ModInt operator*(const ModInt &a) const { return (ModInt(*this) *= a); }
  ModInt operator/(const ModInt &a) const { return (ModInt(*this) /= a); }
  template <class T> friend ModInt operator+(T a, const ModInt &b) { return (ModInt(a) += b); }
  template <class T> friend ModInt operator-(T a, const ModInt &b) { return (ModInt(a) -= b); }
  template <class T> friend ModInt operator*(T a, const ModInt &b) { return (ModInt(a) *= b); }
  template <class T> friend ModInt operator/(T a, const ModInt &b) { return (ModInt(a) /= b); }
  explicit operator bool() const { return x; }
  bool operator==(const ModInt &a) const { return (x == a.x); }
  bool operator!=(const ModInt &a) const { return (x != a.x); }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) { return os << a.x; }
};
////////////////////////////////////////////////////////////////////////////////

constexpr unsigned MO = 998244353;
using Mint = ModInt<MO>;


int N;
vector<Pt> P;

bool f[510][510];
Mint g[510][510];

bool check(const vector<int> &us) {
  const int usLen = us.size();
  for (int i = 0; i < usLen; ++i) {
    const Int d = P[us[i]].abs2();
    for (int j = 0; j < usLen; ++j) if (i != j) {
      if (d >= (P[us[j]] - P[us[i]]).abs2()) {
        return false;
      }
    }
  }
  return true;
}

int main() {
  /*
  for (; ~scanf("%d", &N); ) {
    P.resize(N);
    for (int u = 0; u < N; ++u) {
      scanf("%lld%lld", &P[u].x, &P[u].y);
    }
    */
  std::mt19937 mt(48);
  while (true) {
    N = std::uniform_int_distribution<int>(1, 10)(mt);
    P.resize(N);
    for (int u = 0; u < N; ++u) {
      P[u].x = std::uniform_int_distribution<int>(-5, 5)(mt);
      P[u].y = std::uniform_int_distribution<int>(-5, 5)(mt);
    }
    cerr << "case " << N << '\n';
    for (int u = 0; u < N; ++u) {
      cerr << P[u].x << ' ' << P[u].y << '\n';
    }

    auto slow = [&]() {
      Mint ans = 0;
      for (int u0 = 0; u0 < N; ++u0) {
	if (check({u0})) ans += 1;
	for (int u1 = u0 + 1; u1 < N; ++u1) {
	  if (check({u0, u1})) ans += 1;
	  for (int u2 = u1 + 1; u2 < N; ++u2) {
	    if (check({u0, u1, u2})) ans += 1;
	    for (int u3 = u2 + 1; u3 < N; ++u3) {
	      if (check({u0, u1, u2, u3})) ans += 1;
	      for (int u4 = u3 + 1; u4 < N; ++u4) {
		if (check({u0, u1, u2, u3, u4})) ans += 1;
	      }}}}}
      return ans;
    };

    sort(P.begin(), P.end(), [&](const Pt &a, const Pt &b) -> bool {
      return (cmpArg(a, b) < 0);
    });
    
    memset(f, 0, sizeof(f));
    for (int u = 0; u < N; ++u) for (int v = u + 1; v < N; ++v) {
      const Int d = (P[u] - P[v]).abs2();
      f[u][v] = (P[u].abs2() < d && P[v].abs2() < d);
    }
    memset(g, 0, sizeof(g));
    for (int u = 0; u < N; ++u) for (int v = u + 1; v < N; ++v) for (int w = v + 1; w < N; ++w) {
      if (f[u][v] && f[v][w] && f[u][w]) {
        g[u][w] += 1;
      }
    }
    
    Mint ans = 0;
    ans += N;
    for (int u = 0; u < N; ++u) for (int v = u + 1; v < N; ++v) if (f[u][v]) ans += 1;
    cerr << "ans after 2" << ans << '\n';
    for (int u = 0; u < N; ++u) for (int v = u + 1; v < N; ++v) if (f[u][v]) ans += g[u][v];
    cerr << "ans after 3" << ans << '\n';
    for (int u = 0; u < N; ++u) for (int v = u + 1; v < N; ++v) for (int w = v + 1; w < N; ++w) if (f[u][w]) ans += g[u][v];
    cerr << "ans after 4" << ans << '\n';
    for (int u = 0; u < N; ++u) for (int v = u + 1; v < N; ++v) for (int w = v + 1; w < N; ++w) if (f[u][w]) ans += g[u][v] * g[v][w];
    cerr << "ans after 5" << ans << '\n';
    printf("%u\n", ans.x);
    cerr << slow() << '\n';
    assert(ans == slow());
  }
  return 0;
}
