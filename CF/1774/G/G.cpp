#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

	int N, Q; cin >> N >> Q;

	std::vector<std::array<int, 2>> segs(N);
	for (auto& [x, y] : segs) {
		cin >> x >> y;
	}
	std::sort(segs.begin(), segs.end(), [](auto a, auto b) { return a[1] < b[1] || (a[1] == b[1] && a[0] > b[0]); });

	{
		int L = 1;
		for (int i = 1; i < N; i++) {
			assert(segs[L-1][1] <= segs[i][1]);
			if (segs[i][0] <= segs[L-1][0]) {
				// skip it
			} else {
				segs[L] = segs[i];
				L++;
			}
		}
		N = L;
		segs.resize(N);
	}

	for (int i = 0; i < N; i++) {
		//cerr << i << ' ' << segs[i][0] << '-' << segs[i][1] << '\n';
	}
	for (int i = 1; i < N; i++) {
		assert(segs[i-1][0] < segs[i][0]);
		assert(segs[i-1][1] < segs[i][1]);
	}

	// it's just the parity of the length of the shortest path
	const int L = 20;
	std::vector<std::vector<int>> jumps(L+1, std::vector<int>(N+1));
	for (int i = 0; i < N; i++) {
		int mi = i, ma = N;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (segs[md][0] > segs[i][1]) ma = md;
			else mi = md;
		}
		jumps[0][i] = ma;
		//cerr << i << ' ' << jumps[0][i] << '\n';
	}
	jumps[0][N] = N;
	for (int l = 0; l+1 <= L; l++) {
		for (int i = 0; i <= N; i++) {
			jumps[l+1][i] = jumps[l][jumps[l][i]];
		}
	}

	auto get_anc = [&](int l, int r) -> std::pair<int, int> {
		int ans = 0;
		for (int b = L; b >= 0; b--) {
			if (jumps[b][l] <= r) {
				l = jumps[b][l];
				ans += (1 << b);
			}
		}
		return {l, ans};
	};

	using num = modnum<998244353>;
	auto solve = [&](int l, int r) -> num {
		{
			int mi = -1, ma = N;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (segs[md][0] < l) mi = md;
				else ma = md;
			}
			if (ma == N || segs[ma][0] != l) {
				return 0;
			}
			l = ma;
		}
		{
			int mi = -1, ma = N;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (segs[md][1] < r) mi = md;
				else ma = md;
			}
			if (ma == N || segs[ma][1] != r) {
				return 0;
			}
			r = ma;
		}

		//cerr << l << ' ' << r << '\n';

		if (l == r) return -1;

		auto [af, adist] = get_anc(l, r);
		auto [bf, bdist] = get_anc(l+1, r);
		if (af == bf) return 0;
		if (af != r && bf != r) return 0;
		return (adist == bdist) ? 1 : -1;
	};

	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r;
		num ans = solve(l, r);
		cout << ans << '\n';
	}

	return 0;
}
