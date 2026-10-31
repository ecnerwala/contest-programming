#include <bits/stdc++.h>

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

	const int BASE = 1 << 30;

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());

		if (A[0] == A[N-1]) {
			cout << 0 << '\n';
			continue;
		}

		auto eval_pref = [&](int a) -> std::vector<int> {
			assert(a > 0);
			std::vector<int> res(N/30+2);
			for (int i = a-1; i >= 0; i--) {
				int offset = (int(res.size()) - 1) * 30 - (i == a-1 ? i : i+1);
				int off_int = offset / 30;
				int off_frac = offset % 30;
				int64_t carry = int64_t(A[i]) << off_frac;
				while (carry > 0) {
					assert(off_int < int(res.size()));
					carry += res[off_int];
					res[off_int] = int(carry & ((int64_t(1) << 30) - 1));
					carry >>= 30;
					off_int++;
				}
			}
			std::reverse(res.begin(), res.end());
			return res;
		};

		auto eval_suff = [&](int a) -> std::vector<int> {
			std::reverse(A.begin(), A.end());
			auto res =  eval_pref(a);
			std::reverse(A.begin(), A.end());
			return res;
		};

		auto eval_cnd = [&](int a) -> std::vector<int> {
			auto s = eval_suff(N-a);
			auto p = eval_pref(a);
			assert(int(s.size()) == int(p.size()));
			for (int i = 0; i < int(s.size()); i++) {
				s[i] -= p[i];
			}
			for (int i = int(s.size())-1; i > 0; i--) {
				if (s[i] < 0) {
					s[i] += BASE;
					s[i-1] --;
				}
			}
			assert(s[0] >= 0);
			return s;
		};

		int best_mid;

		// take the best of the middle with brute force
		{
			int st = std::max(N / 2 - 20, 0);
			int en = N - 1 - st;
			assert(st < en);
			int L = en - st;
			std::vector<__int128_t> vals(L);

			{
				__int128_t cur = __int128_t(A[st]) << L;
				for (int i = 0; i < L; i++) {
					vals[i] -= cur;
					cur += __int128_t(A[st+i+1] - A[st+i]) << (L-(i+1));
				}
			}

			{
				__int128_t cur = __int128_t(A[en]) << L;
				for (int i = 0; i < L; i++) {
					vals[L-1-i] += cur;
					cur -= __int128_t(A[en-i] - A[en-i-1]) << (L-(i+1));
				}
			}

			best_mid = int(std::max_element(vals.begin(), vals.end()) - vals.begin()) + st + 1;
		}

		auto v_mid = eval_cnd(best_mid);

		{
			int best_st = 1;
			while (A[best_st] == A[0]) best_st++;

			auto v_st = eval_cnd(best_st);
			if (v_st > v_mid) {
				v_mid = std::move(v_st);
				best_mid = best_st;
			}
		}

		{
			int best_en = N-1;
			while (A[best_en-1] == A[N-1]) best_en--;

			auto v_en = eval_cnd(best_en);
			if (v_en > v_mid) {
				v_mid = std::move(v_en);
				best_mid = best_en;
			}
		}

		using num = modnum<int(1e9)+7>;
		num inv_base = inv(num(BASE));
		std::reverse(v_mid.begin(), v_mid.end());
		num ans = 0;
		for (int v : v_mid) {
			ans *= inv_base;
			ans += v;
		}
		cout << ans << '\n';
	}

	return 0;
}
