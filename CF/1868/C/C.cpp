#include <bits/stdc++.h>

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

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int64_t a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clzll(a) : -1;
}

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N; int M; cin >> N >> M;

		using num = modnum<998244353>;
		std::map<int64_t, std::pair<std::vector<num>, std::vector<num>>> mem;
		auto [_, path_freq] = std::y_combinator([&](auto self, int64_t sz) -> std::pair<std::vector<num>, std::vector<num>> {
			assert(sz >= 0);
			if (!sz) {
				return {{}, {}};
			}
			if (sz == 1) {
				return {{num(1)}, {num(1)}};
			}
			if (mem.count(sz)) {
				return mem[sz];
			}
			assert(sz >= 2);
			int h = log_2(sz) + 1;
			assert(h >= 2);
			assert(sz <= (1LL << h) - 1);
			assert(sz >= (1LL - (h-1)));
			int64_t l_sz = std::min(sz - (1LL << (h-2)), (1LL << (h-1)) - 1);
			int64_t r_sz = sz - 1 - l_sz;
			assert(l_sz >= 0 && r_sz >= 0);
			assert(l_sz == (1LL << (h-1)) - 1 || r_sz == (1LL << (h-2)) - 1);

			auto [l_vert, l_path] = self(l_sz);
			auto [r_vert, r_path] = self(r_sz);

			std::vector<num> vert_freqs(h);
			vert_freqs[0] ++;
			for (int i = 0; i < int(l_vert.size()); i++) {
				vert_freqs[i+1] += l_vert[i];
			}
			for (int j = 0; j < int(r_vert.size()); j++) {
				vert_freqs[j+1] += r_vert[j];
			}
			std::vector<num> path_freqs(2*h-1);
			for (int i = 0; i < h; i++) {
				path_freqs[i] += vert_freqs[i];
			}
			for (int i = 0; i < int(l_vert.size()); i++) {
				for (int j = 0; j < int(r_vert.size()); j++) {
					path_freqs[i+j+2] += l_vert[i] * r_vert[j];
				}
			}
			for (int i = 0; i < int(l_path.size()); i++) {
				path_freqs[i] += l_path[i];
			}
			for (int i = 0; i < int(r_path.size()); i++) {
				path_freqs[i] += r_path[i];
			}
			assert(std::accumulate(vert_freqs.begin(), vert_freqs.end(), num(0)) == num(sz));
			assert(std::accumulate(path_freqs.begin(), path_freqs.end(), num(0)) == num(sz) * num(sz+1) / num(2));
			return mem[sz] = {vert_freqs, path_freqs};
		})(N);

		//for (auto v : path_freq) { cerr << v << ' '; } cerr << '\n';
		for (int L = 1; L <= int(path_freq.size()); L++) {
			path_freq[L-1] *= pow(num(M), std::max<int64_t>(0, N - L));
		}
		num ans = 0;
		// Pick L numbers from 1..M; what's prob the max is >= M
		for (int V = 1; V <= M; V++) {
			num ways_smaller = 1;
			num ways_tot = 1;
			for (int L = 1; L <= int(path_freq.size()); L++) {
				ways_smaller *= V-1;
				ways_tot *= M;
				ans += path_freq[L-1] * (ways_tot - ways_smaller);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
