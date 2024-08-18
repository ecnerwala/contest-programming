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

	using num = modnum<998244353>;
	auto solve_minimal = [](std::vector<std::array<int, 2>> ints, num C) -> num {
		assert(!ints.empty());
		cerr << "solving minimal " << ints.size() << '\n';
		for (int i = 0; i < int(ints.size()); i++) {
			cerr << ints[i][0] << '-' << ints[i][1] << '\n';
		}
		for (int i = 0; i+1 < int(ints.size()); i++) {
			assert(ints[i][0] < ints[i+1][0]);
			assert(ints[i][1] < ints[i+1][1]);
			// must touch
			assert(ints[i][1] >= ints[i+1][0]);
		}
		{
			int lo = ints.front()[0];
			for (auto& [l, r] : ints) { l -= lo, r -= lo; }
		}
		cerr << "shifted" << '\n';
		for (int i = 0; i < int(ints.size()); i++) {
			cerr << ints[i][0] << '-' << ints[i][1] << '\n';
		}
		int L = ints.back()[1];
		std::vector<int> can_l(L+1, -1);
		std::vector<int> can_r(L+1, -1);
		for (auto [l, r] : ints) {
			can_l[l] = r;
			can_r[r] = l;
		}
		// we want to count ways to pick some subset of deltas so that
		// there is a way to assign them to L and R so that every L -> R transition is wide enough
		assert(can_l[0] != -1 && can_r[L] != -1);
		std::vector<std::array<num, 4>> dp(L+1, std::array<num, 4>{num(0), num(0), num(0), num(0)});
		for (int i = 0; i <= L; i++) {
			for (int j = i+1; j <= L; j++) {
				for (int msk_i = 1; msk_i <= 3; msk_i++) {
					int msk_j = (can_l[j] != -1) + 2 * (can_r[j] != -1 && ((msk_i & 2) || (can_l[i] != -1 && can_l[i] <= j)));
					dp[j][msk_j] += (i == 0 && msk_i == 1 ? num(1) : dp[i][msk_i] * num(C-1));
				}
			}
		}
		return dp[L][2] + dp[L][3];
	};

	int N, M; num C; cin >> N >> M >> C;

	std::vector<std::array<int, 2>> intervals(M);
	for (auto& [u, v] : intervals) {
		cin >> u >> v; u--;
	}

	num ans = 1;

	while (N > 0) {
		std::sort(intervals.begin(), intervals.end(), [&](auto a, auto b) { return (a[0] == b[0]) ? (a[1] > b[1]) : (a[0] < b[0]); });
		std::vector<std::array<int, 2>> mins; mins.reserve(N);
		for (auto [l, r] : intervals) {
			while (true) {
				if (mins.empty()) break;
				assert(mins.back()[0] <= l);
				if (mins.back()[1] >= r) {
					mins.pop_back();
				} else {
					break;
				}
			}
			mins.push_back({l, r});
		}
		std::vector<bool> covered(N, false);
		for (int i = 0, j = 0; i < int(mins.size()); i = j) {
			j++;
			while (j < int(mins.size()) && mins[j-1][1] >= mins[j][0]) {
				j++;
			}
			solve_minimal(std::vector(mins.begin()+i, mins.begin()+j), C);
			int lo = mins[i][0], hi = mins[j-1][1];
			for (int z = lo; z < hi; z++) covered[z] = true;
		}
		std::vector<int> idx(N+1);
		for (int i = 0; i < N; i++) {
			idx[i+1] = idx[i] + !covered[i];
		}
		std::vector<std::array<int, 2>> n_intervals;
		for (auto [l, r] : intervals) {
			l = idx[l];
			r = idx[r];
			if (l < r) n_intervals.push_back({l, r});
		}
		intervals = std::move(n_intervals);
		N = idx[N];
	}

	cout << ans << '\n';

	return 0;
}

// You need a subset of delta positions such that
