/**
 *    author:  tourist
 *    created: 22.02.2025 13:02:53
**/
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.cpp"
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

  Modular& operator+=(const Modular& other) { value += other.value; value -= (value >= mod()) * mod(); return *this; }
  Modular& operator-=(const Modular& other) { value -= other.value; value += (value < 0) * mod(); return *this; }
  template <typename U> Modular& operator+=(const U& other) { return *this += Modular(other); }
  template <typename U> Modular& operator-=(const U& other) { return *this -= Modular(other); }
  Modular& operator++() { return *this += 1; }
  Modular& operator--() { return *this -= 1; }
  Modular operator++(int) { Modular result(*this); *this += 1; return result; }
  Modular operator--(int) { Modular result(*this); *this -= 1; return result; }
  Modular operator-() const { return Modular(-value); }

  template <typename U = T>
  typename enable_if<is_same<typename Modular<U>::Type, int>::value, Modular>::type& operator*=(const Modular& rhs) {
    value = normalize(static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value));
    return *this;
  }
  template <typename U = T>
  typename enable_if<is_same<typename Modular<U>::Type, int64_t>::value, Modular>::type& operator*=(const Modular& rhs) {
    int64_t q = int64_t(static_cast<long double>(value) * rhs.value / mod());
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
  typename common_type<typename Modular<T>::Type, int64_t>::type x;
  stream >> x;
  number.value = Modular<T>::normalize(x);
  return stream;
}

// using ModType = int;

// struct VarMod { static ModType value; };
// ModType VarMod::value;
// ModType& md = VarMod::value;
// using Mint = Modular<VarMod>;

constexpr int md = 998244353;
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
  cin.tie(nullptr);
  int n, m, k;
  cin >> n >> m >> k;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<int> b(m);
  for (int i = 0; i < m; i++) {
    cin >> b[i];
  }
  map<int, pair<int, int>> mp;
  for (int x : a) {
    mp[-x].first += 1;
  }
  for (int x : b) {
    mp[-x].second += 1;
  }
  auto Dist = [&](int items, int boxes) -> Mint {
    assert(items >= 0 && boxes >= 0);
    if (boxes == 0) {
      return items == 0 ? 1 : 0;
    }
    return C(items + boxes - 1, items);
  };
  auto Count = [&](int equal, int greater, int rem_a, int rem_b, int items_a, int items_b) -> Mint {
    equal += greater;
    items_a -= greater;
    if (items_a < 0) return 0;
    rem_a += greater;
    Mint ret = 0;
    for (int in_eq = 0; in_eq <= min(items_a, items_b); in_eq++) {
      Mint cur = Dist(in_eq, equal);
      cur *= Dist(items_a - in_eq, rem_a);
      cur *= Dist(items_b - in_eq, rem_b);
      ret += cur;
    }
    return ret;
  };
  vector dp(k + 1, vector(k + 1, vector<Mint>(3)));
  dp[k][0][2] = 1;
  int pa = k + 1;
  int pb = k + 1;
  C(n + m + k, 0);
  Mint quo = fact[n + k] * fact[m] * inv_fact[k];
  for (auto& [foo, bar] : mp) {
    auto [na, nb] = bar;
    vector new_dp(k + 1, vector(k + 1, vector<Mint>(3)));
    // c = how many are still in state 0 (winner in b is not filled)
    // d = how many are already in state 2 (winner in b is filled, and a is winning)
    for (int c = 0; c <= k; c++) {
      for (int d = 0; d <= k - c; d++) {
        for (int v = 0; v < 3; v++) {
          for (int kill = 0; kill <= min(nb, c); kill++) {
            for (int win = 0; win <= k - d - c; win++) {
              Mint ft = dp[c][d][v];
              if (ft == 0) {
                continue;
              }
              ft *= C(c, kill) * C(k - d - c, win);
              if (v == 2) {
                Mint total;
                {
                  int equal = k - d - win;
                  int greater = win;
                  int rem_a = pa - equal - greater;
                  int rem_b = pb - equal - greater - (k - c);
                  total = Count(equal, greater, rem_a, rem_b, na, nb - kill);
                }
                {
                  int equal = k - d - win + 1;
                  int greater = win;
                  int rem_a = pa - equal - greater;
                  int rem_b = pb - equal - greater - (k - c);
                  auto got = Count(equal, greater, rem_a, rem_b, na, nb - kill);
                  new_dp[c - kill][d + win][2] += ft * got;
                  total -= got;
                }
                {
                  int equal = k - d - win;
                  int greater = win + 1;
                  int rem_a = pa - equal - greater;
                  int rem_b = pb - equal - greater - (k - c);
                  auto got = Count(equal, greater, rem_a, rem_b, na, nb - kill);
                  new_dp[c - kill][d + win][0] += ft * got;
                  total -= got;
                }
                new_dp[c - kill][d + win][1] += ft * total;
              } else {
                int equal = k - d - win;
                int greater = win;
                int rem_a = pa - equal - greater;
                int rem_b = pb - equal - greater - (k - c);
                new_dp[c - kill][d + win][v] += ft * Count(equal, greater, rem_a, rem_b, na, nb - kill);
              }
              debug(c, d, v, ft, kill, win, pa, pb, new_dp);
            }
          }
        }
      }
    }
    pa += na;
    pb += nb;
    quo *= inv_fact[na] * inv_fact[nb];
    swap(dp, new_dp);
    debug(dp);
  }
  vector<Mint> res(3);
  Mint sum = 0;
  for (int i = 1; i < 3; i++) {
    Mint cur = 0;
    for (int d = 0; d <= k; d++) {
      cur += dp[0][d][i];
    }
    sum += cur;
    res[i] = cur / quo;
    // cout << cur / quo << '\n';
  }
  res[0] = 1 - res[1] - res[2];
  for (int i = 0; i < 3; i++) {
    cout << res[i] << '\n';
  }
  debug(sum, quo, sum == quo);
  // assert(sum == quo);
  return 0;
}
