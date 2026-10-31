#include <bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

#define i main

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
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
		v = int(ll(v) * ll(o.v) % MOD);
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

using num = modnum<998244353>;

int i() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int Q, K; cin >> Q >> K;

		auto p2 = [](int a) -> int64_t {
			return int64_t(1) << a;
		};

		auto reverse_bits = [&](int64_t v) -> int64_t {
			int64_t o = 0;
			int64_t x = v;
			for (int i = 0; i < K; i++) {
				o <<= 1;
				o |= (x & 1);
				x >>= 1;
			}
			return o;
		};

		num ans = 0;

		hash_map<int64_t, num> cnt_includes;
		hash_map<int64_t, num> tot_subtree;

		// Level is # of ?'s
		auto add_interval = [&](int64_t a, int level) -> void {
			assert(0 <= level && level <= K);
			// First, we need to fold the interval
			num weight;
			if (level * 2 >= K) {
				weight = num(p2(level - (K/2)));
				level = K/2;
				a = 0;
			} else {
				weight = 1;
				a ^= reverse_bits(a);
				a &= p2(K/2)-p2(level);
			}

			ans += num(p2(level)) * ((weight) * (weight-1) / num(2));
			ans += tot_subtree[(a + p2(K/2)) >> level] * weight;
			for (int i = level+1; i <= K/2; i++) {
				ans += cnt_includes[(a + p2(K/2)) >> i] * num(p2(level)) * weight;
			}

			cnt_includes[(a + p2(K/2)) >> level] += weight;
			for (int i = level; i <= K/2; i++) {
				tot_subtree[(a + p2(K/2)) >> i] += num(p2(level)) * weight;
			}
		};

		for (int q = 0; q < Q; q++) {
			int64_t L, R; cin >> L >> R; R++;
			for (int l = 0; L < R; l++) {
				if (L & p2(l)) {
					add_interval(L, l);
					L += p2(l);
				}
				if (R & p2(l)) {
					R -= p2(l);
					add_interval(R, l);
				}
			}

			cout << ans << '\n';
		}
	}

	return 0;
}



// x = 1010101110????
// y = 1010??????????
// z = 1010101110110?
//
// x -> low_bits(x) ^ reverse(high_bits(x))
// Then, x^y is a palindrome iff folded(x) ^ folded(y) == 0

