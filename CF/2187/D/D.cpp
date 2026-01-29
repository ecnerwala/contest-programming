#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
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
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(unsigned v_) : v(int(v_ % MOD)) { }
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(uint64_t v_) : v(int(v_ % MOD)) { }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		cerr << "start case" << '\n';
		int N; int64_t X, Y; cin >> N >> X >> Y;
		std::string S; cin >> S;

		using num = modnum<998244353>;
		int64_t C0 = 0;
		int64_t pref = 0;
		int st = 0;
		for (; st < int(S.size()) && S[st] != '?'; st++) {
			if (S[st] == '0') C0 = X + C0;
			else if (S[st] == '1') C0 = Y - C0;
			else assert(false);
			pref += C0;
		}
		if (st == int(S.size())) {
			cout << num(pref) << '\n';
			continue;
		}
		assert(S[st] == '?');
		S.erase(S.begin(), S.begin() + st + 1);
		N = int(S.size());
		cerr << N << ' ' << S << '\n';
		std::vector<int> gaps; gaps.reserve(N);
		std::array<int, 2> off{1, 0};
		for (int i = 0; i < int(S.size()); i++) {
			if (S[i] == '0') {
				off[0]++;
				off[1]--;
			} else if (S[i] == '1') {
				std::swap(off[0], off[1]);
			} else if (S[i] == '?') {
				// we want off[0] = merge(off[0]+1, off[1]), off[1] = merge(off[0], off[1]-1)
				gaps.push_back(std::abs(off[0] - (off[1] - 1)));
				off[1] = std::min(off[0], off[1]-1);
				off[0] = off[1]+1;
			} else assert(false);
		}
		cerr << "off " << off[0] << ' ' << off[1] << '\n';
		std::vector<int> gap_freq(N+1);
		for (auto g : gaps) {
			cerr << "gap " << g << '\n';
			assert(!(g & 1));
			g /= 2;
			assert(g <= N);
			gap_freq[g]++;
		}
		std::vector<int> okay(N+1);
		int mx = 0;
		okay[mx] = true;
		for (int g = 1; g <= N; g++) {
			if (gap_freq[g] >= 3) {
				int c = (gap_freq[g] - 1) / 2;
				assert(2 * g <= N);
				gap_freq[g] -= 2 * c;
				gap_freq[2 * g] += c;
			}
			for (int z = 0; z < gap_freq[g]; z++) {
				assert(mx + g <= N);
				for (int i = mx; i >= 0; i--) {
					if (okay[i]) okay[i+g] = true;
				}
				mx += g;
			}
		}
		std::vector<int64_t> all_opts; all_opts.reserve(2*(N+1));
		for (int i = 0; i <= mx; i++) {
			if (okay[i]) {
				for (int d = 0; d < 2; d++) {
					// go right 2*i + off[d] steps
					int64_t init_C = (d ? Y - C0 : X + C0);
					int init_pos = !d;
					int final_pos = 2*i + off[d];
					int steps = (final_pos - init_pos) * (d ? -1 : 1);
					cerr << init_pos << ' ' << final_pos << ' ' << d << ' ' << steps << '\n';
					assert((steps & 1) == (N & 1));
					assert(steps <= N);
					assert(steps >= -N);
					int64_t val = pref + init_C;
					val += int64_t(init_C) * N + int64_t(X) * (int64_t(N) * int64_t(N+1) / 2);
					int extra = (N - steps) / 2;
					val += extra * 2 * (Y - int64_t(init_C + int64_t(N) * X)) + int64_t(extra) * int64_t(extra+1) * X;
					all_opts.push_back(val);
				}
			}
		}
		std::sort(all_opts.begin(), all_opts.end());
		all_opts.resize(std::unique(all_opts.begin(), all_opts.end()) - all_opts.begin());
		num ans = 0;
		for (auto v : all_opts) ans += num(v);
		cout << ans << '\n';
	}

	return 0;
}
