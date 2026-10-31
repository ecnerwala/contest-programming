#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

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

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};


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
	template <typename U, typename V>
	size_t operator()(pairnum<U, V> x) const {
		return operator() (uint64_t(uint32_t(int(x.v))) << 32 | uint64_t(uint32_t(int(x.u))));
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, L; cin >> N >> M >> L;
	std::string S; cin >> S;
	assert(int(S.size()) == N);
	std::vector<std::string> Q(M);
	for (auto& q : Q) {
		cin >> q;
		assert(int(q.size()) == L);
	}

	using mnum = modnum<998244353>;
	using pnum = pairnum<mnum, mnum>;
	const pnum BASE(2579381, 192785823);

	std::vector<pnum> pref(N+1);
	pref[0] = 0;
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i] * BASE + pnum(S[i] - 'a');
	}
	std::vector<pnum> pow_BASE(L+1);
	pow_BASE[0] = 1;
	for (int i = 0; i < L; i++) {
		pow_BASE[i+1] = pow_BASE[i] * BASE;
	}

	if (L >= 15) {
		hash_map<pnum, std::vector<int>> short_locs;

		int L_short = L / 3;
		pnum BASE_L_short = pow_BASE[L_short];
		for (int i = 0; i+L_short <= N; i++) {
			pnum cnd = pref[i+L_short] - BASE_L_short * pref[i];
			short_locs[cnd].push_back(i);
		}

		auto check_str = [&](const std::string& q) -> bool {
			for (int z = 0; z < 3; z++) {
				pnum cur = 0;
				for (int i = 0; i < L_short; i++) {
					cur = cur * BASE + pnum(q[z * L_short + i] - 'a');
				}
				auto it = short_locs.find(cur);
				if (it != short_locs.end()) {
					for (int j : it->second) {
						j -= z * L_short;
						if (0 <= j && j + L <= N) {
							// check this guy
							int cnt = 0;
							for (int i = 0; i < L; i++) {
								if (q[i] != S[j+i]) {
									cnt++;
									if (cnt > 2) break;
								}
							}
							if (cnt <= 2) return true;
						}
					}
				}
			}
			return false;
		};

		int ans = 0;
		for (const auto& q : Q) {
			ans += check_str(q);
		}
		cout << ans << '\n';
	} else if (L <= 2) {
		cout << M << '\n';
	} else {
		std::vector<hash_set<pnum>> excl(L*L);
		for (int xa = 0; xa < L; xa++) {
			for (int xb = xa+1; xb < L; xb++) {
				for (int i = 0; i+L <= N; i++) {
					pnum cnd = pref[i+L] - pref[i] * pow_BASE[L];
					cnd -= pnum(S[i + xa] - 'a') * pow_BASE[L - 1 - xa];
					cnd -= pnum(S[i + xb] - 'a') * pow_BASE[L - 1 - xb];
					excl[xa*L+xb].insert(cnd);
				}
			}
		}
		auto check_str = [&](const std::string& q) -> bool {
			pnum tot = 0;
			for (int i = 0; i < L; i++) {
				tot = tot * BASE + pnum(q[i] - 'a');
			}
			for (int xa = 0; xa < L; xa++) {
				for (int xb = xa+1; xb < L; xb++) {
					pnum cnd = tot;
					cnd -= pnum(q[xa] - 'a') * pow_BASE[L - 1 - xa];
					cnd -= pnum(q[xb] - 'a') * pow_BASE[L - 1 - xb];
					if (excl[xa*L+xb].find(cnd) != excl[xa*L+xb].end()) return true;
				}
			}
			return false;
		};

		int ans = 0;
		for (const auto& q : Q) {
			ans += check_str(q);
		}
		cout << ans << '\n';
	}

	return 0;
}
