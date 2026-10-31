/**
 *    author:  tourist
 *    created: 05.11.2023 15:18:46       
**/
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

template <typename T>
T inverse(T a, T m) {
  T u = 0, v = 1;
  while (a != 0) {
    T t = m / a;
    m -= t * a; swap(a, m);
    u -= t * v; swap(u, v);
  }
  assert(m == 1);
  return u;
}

template <typename T>
class Modular {
 public:
  using Type = typename decay<decltype(T::value)>::type;

  constexpr Modular() : value() {}
  template <typename U>
  Modular(const U& x) {
    value = normalize(x);
  }

  template <typename U>
  static Type normalize(const U& x) {
    Type v;
    if (-mod() <= x && x < mod()) v = static_cast<Type>(x);
    else v = static_cast<Type>(x % mod());
    if (v < 0) v += mod();
    return v;
  }

  const Type& operator()() const { return value; }
  template <typename U>
  explicit operator U() const { return static_cast<U>(value); }
  constexpr static Type mod() { return T::value; }

  Modular& operator+=(const Modular& other) { if ((value += other.value) >= mod()) value -= mod(); return *this; }
  Modular& operator-=(const Modular& other) { if ((value -= other.value) < 0) value += mod(); return *this; }
  template <typename U> Modular& operator+=(const U& other) { return *this += Modular(other); }
  template <typename U> Modular& operator-=(const U& other) { return *this -= Modular(other); }
  Modular& operator++() { return *this += 1; }
  Modular& operator--() { return *this -= 1; }
  Modular operator++(int) { Modular result(*this); *this += 1; return result; }
  Modular operator--(int) { Modular result(*this); *this -= 1; return result; }
  Modular operator-() const { return Modular(-value); }

  template <typename U = T>
  typename enable_if<is_same<typename Modular<U>::Type, int>::value, Modular>::type& operator*=(const Modular& rhs) {
#ifdef _WIN32
    uint64_t x = static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value);
    uint32_t xh = static_cast<uint32_t>(x >> 32), xl = static_cast<uint32_t>(x), d, m;
    asm(
      "divl %4; \n\t"
      : "=a" (d), "=d" (m)
      : "d" (xh), "a" (xl), "r" (mod())
    );
    value = m;
#else
    value = normalize(static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value));
#endif
    return *this;
  }
  template <typename U = T>
  typename enable_if<is_same<typename Modular<U>::Type, long long>::value, Modular>::type& operator*=(const Modular& rhs) {
    long long q = static_cast<long long>(static_cast<long double>(value) * rhs.value / mod());
    value = normalize(value * rhs.value - q * mod());
    return *this;
  }
  template <typename U = T>
  typename enable_if<!is_integral<typename Modular<U>::Type>::value, Modular>::type& operator*=(const Modular& rhs) {
    value = normalize(value * rhs.value);
    return *this;
  }

  Modular& operator/=(const Modular& other) { return *this *= Modular(inverse(other.value, mod())); }

  friend const Type& abs(const Modular& x) { return x.value; }

  template <typename U>
  friend bool operator==(const Modular<U>& lhs, const Modular<U>& rhs);

  template <typename U>
  friend bool operator<(const Modular<U>& lhs, const Modular<U>& rhs);

  template <typename V, typename U>
  friend V& operator>>(V& stream, Modular<U>& number);

 private:
  Type value;
};

template <typename T> bool operator==(const Modular<T>& lhs, const Modular<T>& rhs) { return lhs.value == rhs.value; }
template <typename T, typename U> bool operator==(const Modular<T>& lhs, U rhs) { return lhs == Modular<T>(rhs); }
template <typename T, typename U> bool operator==(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) == rhs; }

template <typename T> bool operator!=(const Modular<T>& lhs, const Modular<T>& rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator!=(const Modular<T>& lhs, U rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator!=(U lhs, const Modular<T>& rhs) { return !(lhs == rhs); }

template <typename T> bool operator<(const Modular<T>& lhs, const Modular<T>& rhs) { return lhs.value < rhs.value; }

template <typename T> Modular<T> operator+(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U> Modular<T> operator+(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U> Modular<T> operator+(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) += rhs; }

template <typename T> Modular<T> operator-(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U> Modular<T> operator-(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U> Modular<T> operator-(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) -= rhs; }

template <typename T> Modular<T> operator*(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U> Modular<T> operator*(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U> Modular<T> operator*(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) *= rhs; }

template <typename T> Modular<T> operator/(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U> Modular<T> operator/(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U> Modular<T> operator/(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) /= rhs; }

template<typename T, typename U>
Modular<T> power(const Modular<T>& a, const U& b) {
  assert(b >= 0);
  Modular<T> x = a, res = 1;
  U p = b;
  while (p > 0) {
    if (p & 1) res *= x;
    x *= x;
    p >>= 1;
  }
  return res;
}

template <typename T>
bool IsZero(const Modular<T>& number) {
  return number() == 0;
}

template <typename T>
string to_string(const Modular<T>& number) {
  return to_string(number());
}

// U == std::ostream? but done this way because of fastoutput
template <typename U, typename T>
U& operator<<(U& stream, const Modular<T>& number) {
  return stream << number();
}

// U == std::istream? but done this way because of fastinput
template <typename U, typename T>
U& operator>>(U& stream, Modular<T>& number) {
  typename common_type<typename Modular<T>::Type, long long>::type x;
  stream >> x;
  number.value = Modular<T>::normalize(x);
  return stream;
}

/*
using ModType = int;

struct VarMod { static ModType value; };
ModType VarMod::value;
ModType& md = VarMod::value;
using Mint = Modular<VarMod>;
*/

constexpr int md = (int) 1e9 + 7;
using Mint = Modular<std::integral_constant<decay<decltype(md)>::type, md>>;

vector<Mint> fact(1, 1);
vector<Mint> inv_fact(1, 1);

Mint C(int n, int k) {
  if (k < 0 || k > n) {
    return 0;
  }
  while ((int) fact.size() < n + 1) {
    fact.push_back(fact.back() * (int) fact.size());
    inv_fact.push_back(1 / fact.back());
  }
  return fact[n] * inv_fact[k] * inv_fact[n - k];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n;
  cin >> n;
  C(10 * n, 0);
  vector<vector<pair<int, int>>> g(2 * n - 1);
  vector<pair<int, int>> edges;
  auto AddEdge = [&](int x, int y) {
    g[x].emplace_back(y, (int) edges.size());
    edges.emplace_back(x, y);
    g[y].emplace_back(x, (int) edges.size());
    edges.emplace_back(y, x);
  };
  for (int i = 0; i < n - 1; i++) {
    int x, y;
    cin >> x >> y;
    --x; --y;
    AddEdge(x, n + i);
    AddEdge(y, n + i);
  }
  vector<vector<int>> cc(edges.size());
  vector<int> max_dist(2 * n - 1, 0);
  for (int i = 0; i < (int) edges.size(); i++) {
    function<void(int, int, int)> Dfs = [&](int v, int pv, int d) {
      if (v < n) {
        if ((int) cc[i].size() < d + 1) {
          cc[i].resize(d + 1, 0);
        }
        cc[i][d] += 1;
      }
      for (auto& p : g[v]) {
        int u = p.first;
        if (u == pv) {
          continue;
        }
        Dfs(u, v, d + 1);
      }
    };
    Dfs(edges[i].second, edges[i].first, 0);
    max_dist[edges[i].first] = max(max_dist[edges[i].first], (int) cc[i].size());
  }
  int center = (int) (min_element(max_dist.begin(), max_dist.end()) - max_dist.begin());
  int radius = max_dist[center];
  assert(count(max_dist.begin(), max_dist.end(), max_dist[center]) == 1);
  vector<vector<Mint>> dp(edges.size());
  auto Add = [&](int i, int j, Mint val) {
    if ((int) dp[i].size() < j + 1) {
      dp[i].resize(j + 1, 0);
    }
    dp[i][j] += val;
  };
  for (int i = 0; i < (int) edges.size(); i++) {
    if (edges[i].second == center) {
      Add(i, 0, 1);
      break;
    }
  }
  debug(edges);
  while (radius > 0) {
    debug(dp);
    vector<vector<Mint>> old_dp(edges.size());
    swap(dp, old_dp);
    for (int i = 0; i < (int) edges.size(); i++) {
      if (old_dp[i].empty()) {
        continue;
      }
      debug(radius, i, edges[i], old_dp[i], cc[i].size(), cc[i ^ 1].size());
      int cnt_from = cc[i ^ 1][radius - 1];
      int cnt_total = cc[i][radius] + cnt_from;
      for (auto& p : g[edges[i].second]) {
        if ((int) cc[p.second].size() < radius) {
          continue;
        }
        int cnt_to = cc[p.second][radius - 1];
        if ((p.second ^ 1) == i) {
          debug(i, p.second, cnt_total, cnt_from, old_dp[i], dp);
          for (int j = 0; j < (int) old_dp[i].size(); j++) {
            if (old_dp[i][j] == 0) {
              continue;
            }
            int cnt_out = cnt_total - cnt_from - j;
            assert(cnt_out > 0);
            for (int k = 0; k < cnt_from; k++) {
              auto ways = fact[k + cnt_out - 1] * cnt_out * C(cnt_total - cnt_from, j);
              Add(p.second, k, old_dp[i][j] * ways);
            }
          }
          debug(dp);
        } else {
          for (int j = 0; j < (int) old_dp[i].size(); j++) {
            if (old_dp[i][j] == 0) {
              continue;
            }
            for (int in_me = 0; in_me < cnt_to && in_me <= j; in_me++) {
              int cnt_out = cnt_total - j - (cnt_to - in_me);
              if (cnt_out <= 0) {
                continue;
              }
              for (int k = 0; k < cnt_to - in_me; k++) {
                auto ways = fact[k + cnt_out - 1] * cnt_out * C(cnt_total - cnt_from - cnt_to, j - in_me);
                Add(p.second, in_me + k, old_dp[i][j] * ways);
              }
            }
          }
        }
      }
    }
    radius -= 1;
  }
  debug(dp);
  Mint ans = 0;
  for (int i = 0; i < (int) edges.size(); i++) {
    if (!dp[i].empty()) {
      ans += dp[i][0];
    }
  }
  cout << ans << '\n';
  return 0;
}
