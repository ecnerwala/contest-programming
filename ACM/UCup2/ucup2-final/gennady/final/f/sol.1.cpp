/**
 *    author:  tourist
 *    created: 22.02.2025 11:45:07
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

// vector<Mint> fact(1, 1);
// vector<Mint> inv_fact(1, 1);

// Mint C(int n, int k) {
//   if (k < 0 || k > n) {
//     return 0;
//   }
//   while ((int) fact.size() < n + 1) {
//     fact.push_back(fact.back() * (int) fact.size());
//     inv_fact.push_back(1 / fact.back());
//   }
//   return fact[n] * inv_fact[k] * inv_fact[n - k];
// }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int tt;
  cin >> tt;
  while (tt--) {
    int n, m, s;
    cin >> n >> m >> s;
    vector<int> d(s);
    for (int i = 0; i < s; i++) {
      cin >> d[i];
    }
    vector<int64_t> y(s + 1);
    y[0] = 0;
    for (int i = 0; i < s; i++) {
      y[i + 1] = y[i] + d[i];
    }
    vector<pair<int64_t, int>> e;
    e.reserve(2 * (s + 1));
    for (int i = 0; i <= s; i++) {
      e.emplace_back(y[i] - m + 1, ~i);
      e.emplace_back(y[i] + 1, i);
    }
    sort(e.begin(), e.end());
    vector<Mint> fact(n + 2);
    fact[0] = 1;
    for (int i = 1; i <= n + 1; i++) {
      fact[i] = fact[i - 1] * i;
    }
    Mint ans = 1;
    Mint cur = 1;
    auto Func = [&](int len) {
      int from = -n + 1;
      int to = len - 1;
      int cnt = to - from + 1;
      Mint ret = 1;
      int lim = min(len, n);
      if (2 * lim <= cnt) {
        ret = fact[lim + 1] * fact[lim + 1] * power(Mint(lim + 1), cnt - 2 * lim);
      } else {
        ret = fact[cnt / 2 + 1] * fact[(cnt + 1) / 2 + 1];
      }
      // for (int i = 0; i <= cnt - 1; i++) {
      //   ret *= min({lim, i + 1, cnt - i}) + 1;
      // }
      return ret;
    };
    set<pair<int, int>> st;
    for (int it = 0; it < int(e.size()); it++) {
      int id = e[it].second;
      if (id < 0) {
        id = ~id;
        // add id
        auto it = st.lower_bound({id + 1, -1});
        int l = id;
        int r = id;
        if (it != st.end() && it->first == id + 1) {
          r = it->second;
          cur /= Func(it->second - it->first + 1);
          it = st.erase(it);
        }
        if (it != st.begin()) {
          it = prev(it);
          if (it->second == id - 1) {
            l = it->first;
            cur /= Func(it->second - it->first + 1);
            it = st.erase(it);
          }
        }
        cur *= Func(r - l + 1);
        st.emplace(l, r);
      } else {
        // remove id
        auto it = st.lower_bound({id + 1, -1});
        assert(it != st.begin());
        it = prev(it);
        auto [l, r] = *it;
        assert(l <= id && id <= r);
        cur /= Func(r - l + 1);
        st.erase(it);
        if (id > l) {
          cur *= Func(id - l);
          st.emplace(l, id - 1);
        }
        if (id < r) {
          cur *= Func(r - id);
          st.emplace(id + 1, r);
        }
      }
      // debug(e[it], st, cur);
      if (it < int(e.size()) - 1) {
        auto T = e[it + 1].first - e[it].first;
        if (T > 0) {
          ans *= power(cur, T);
        }
      }
    }
    cout << ans << '\n';
  }
  return 0;
}
