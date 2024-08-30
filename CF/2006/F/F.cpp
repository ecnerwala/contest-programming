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

using std::cerr;

// return either the run-length encoded sequence, or return a checkerboard
std::variant<std::vector<int>, std::array<std::array<int, 2>, 2>> check_matrix(int N, std::vector<std::array<int, 2>> A) {
	std::vector<std::pair<int, int>> l_deg(N);
	for (int i = 0; i < N; i++) l_deg[i].second = i;
	std::vector<std::pair<int, int>> r_deg(N);
	for (int i = 0; i < N; i++) r_deg[i].second = i;
	for (auto [x, y] : A) {
		l_deg[x].first++;
		r_deg[y].first++;
	}
	std::sort(l_deg.begin(), l_deg.end());
	std::sort(r_deg.begin(), r_deg.end());
	std::vector<int> l_mapping(N);
	for (int i = 0; i < N; i++) l_mapping[l_deg[i].second] = i;
	std::vector<int> r_mapping(N);
	for (int i = 0; i < N; i++) r_mapping[r_deg[i].second] = i;

	for (auto& [x, y] : A) {
		x = l_mapping[x];
		y = r_mapping[y];
	}

	for (auto [x, y] : A) {
		// y should be a suffix
		if (y < N - l_deg[x].first) {
			// we've found a bad pair
			// now, let's try to find the corresponding hole
			std::vector<bool> x_has(N);
			for (auto [nx, ny] : A) { if (nx == x) x_has[ny] = true; }
			int y2 = N - l_deg[x].first;
			while (x_has[y2]) {
				y2++;
				assert(y2 < N);
			}
			std::vector<bool> y_has(N);
			std::vector<bool> y2_has(N);
			for (auto [nx, ny] : A) { if (ny == y) y_has[nx] = true; else if (ny == y2) y2_has[nx] = true; }
			assert(y_has[x] && !y2_has[x]);
			int x2 = 0;
			while (!(!y_has[x2] && y2_has[x2])) {
				x2++;
				assert(x2 < N);
			}
			return std::array<std::array<int, 2>, 2>{
				std::array<int, 2>{l_deg[x].second, r_deg[y].second},
				std::array<int, 2>{l_deg[x2].second, r_deg[y2].second}
			};
		}
	}
	// otherwise, we're good
	std::vector<int> moves;
	for (int i = 0; i < N; i++) {
		int d = l_deg[i].first;
		if (i == 0 && d == 0) {
			moves.push_back(1);
		} else if (i == 0) {
			assert(d > 0);
			moves.push_back(d);
			moves.push_back(1);
		} else if (d == l_deg[i-1].first) {
			moves.back()++;
		} else {
			assert(d > l_deg[i-1].first);
			moves.push_back(d - l_deg[i-1].first);
			moves.push_back(1);
		}
	}
	return moves;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> A(M);
		for (auto& [x, y] : A) { cin >> x >> y; x--, y--; }
		cerr << "CASE: " << N << ' ' << M << '\n';

		using num = modnum<998244353>;
		std::vector<num> fact(N+1);
		fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(N+1);
		ifact[N] = inv(fact[N]);
		for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		auto cnt_good = [&](std::vector<int> a) {
			num res = 1;
			for (auto v : a) res *= fact[v];
			return res;
		};

		auto r = check_matrix(N, A);
		num ans = 0;
		if (std::holds_alternative<std::vector<int>>(r)) {
			auto moves = std::get<std::vector<int>>(r);
			cerr << "good" << '\n';
			for (auto v : moves) { cerr << v << ' '; } cerr << '\n';
			num baseline = cnt_good(moves);
			for (int i = 0; i+1 < int(moves.size()); i++) {
				// try to swap this pair
				num w = baseline;
				if (moves[i] == 1 && i-1 >= 0) w *= moves[i-1] + 1;
				if (moves[i+1] == 1 && i+2 < int(moves.size())) w *= moves[i+2] + 1;
				ans += w;
			}
		} else {
			auto checkerboard = std::get<std::array<std::array<int, 2>, 2>>(r);
			// try swapping all of them
			for (int z = 0; z < 2; z++) {
				auto NA = A;
				auto it = std::find(NA.begin(), NA.end(), checkerboard[z]);
				assert(it != NA.end());
				NA.erase(it);
				auto nr = check_matrix(N, NA);
				if (std::holds_alternative<std::vector<int>>(nr)) {
					ans += cnt_good(std::get<std::vector<int>>(nr));
				}
			}
			for (int z = 0; z < 2; z++) {
				std::array<int, 2> cnd{checkerboard[z][0], checkerboard[!z][1]};
				auto NA = A;
				assert(std::find(NA.begin(), NA.end(), cnd) == NA.end());
				NA.push_back(cnd);
				auto nr = check_matrix(N, NA);
				if (std::holds_alternative<std::vector<int>>(nr)) {
					ans += cnt_good(std::get<std::vector<int>>(nr));
				}
			}
		}
		ans /= num(N) * num(N);
		cout << ans << '\n';
	}

	return 0;
}
