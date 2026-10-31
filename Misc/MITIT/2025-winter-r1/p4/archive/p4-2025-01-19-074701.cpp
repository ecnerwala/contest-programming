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

using num = modnum<int(1e9)+7>;
std::pair<bool, num> solve(std::vector<std::pair<std::array<int, 2>, std::string>> pts) {
	int N = int(pts.size());
	std::vector<int> x_to_y(N, -1);
	std::vector<int> y_to_x(N, -1);
	std::vector<bool> x_is_vert(N);
	std::vector<bool> y_is_horz(N);
	for (const auto& [p, s] : pts) {
		auto [x, y] = p;
		assert(0 <= x && x < N);
		assert(x_to_y[x] == -1);
		assert(0 <= y && y < N);
		assert(y_to_x[y] == -1);
		x_to_y[x] = y;
		y_to_x[y] = x;
		if (s == "UD") {
			x_is_vert[x] = true;
			y_is_horz[y] = false;
		} else if (s == "LR") {
			x_is_vert[x] = false;
			y_is_horz[y] = true;
		} else assert(false);
	}

	std::vector<int> horz_xs; horz_xs.reserve(N);
	for (int x = 0; x < N; x++) {
		if (!x_is_vert[x]) horz_xs.push_back(x);
	}

	bool ans_poss = false;
	num ans_ways = 0;

	for (int z = 0; z+1 < int(horz_xs.size()); z++) {
		bool poss = true;
		std::vector<bool> can_left(N);
		{
			int min_up = N;
			int max_down = -1;
			for (int x = 0; x < N; x++) {
				int y = x_to_y[x];
				if (x_is_vert[x]) {
					assert(x != horz_xs[z]);
					assert(x != horz_xs[z+1]);
					bool can_up = true, can_down = true;
					if (x > horz_xs[z]) {
						if (y > x_to_y[horz_xs[z]]) {
							can_down = false;
						} else {
							can_up = false;
						}
					}
					if (x < horz_xs[z+1]) {
						if (y > x_to_y[horz_xs[z+1]]) {
							can_down = false;
						} else {
							can_up = false;
						}
					}
					assert(!can_up || !can_down);
					if (can_up) {
						min_up = std::min(min_up, y);
					} else if (can_down) {
						max_down = std::max(max_down, y);
					} else {
						poss = false;
						break;
					}
				} else {
					can_left[x] = (y > max_down) && (y < min_up);
					if (z == 2) std::cerr << x << ' ' << can_left[x] << '\n';
				}
			}
		}
		if (!poss) continue;
		std::vector<bool> can_right(N);
		{
			int min_up = N;
			int max_down = -1;
			for (int x = N-1; x >= 0; x--) {
				int y = x_to_y[x];
				if (x_is_vert[x]) {
					assert(x != horz_xs[z]);
					assert(x != horz_xs[z+1]);
					bool can_up = true, can_down = true;
					if (x > horz_xs[z]) {
						if (y > x_to_y[horz_xs[z]]) {
							can_down = false;
						} else {
							can_up = false;
						}
					}
					if (x < horz_xs[z+1]) {
						if (y > x_to_y[horz_xs[z+1]]) {
							can_down = false;
						} else {
							can_up = false;
						}
					}
					assert(!can_up || !can_down);
					if (can_up) {
						min_up = std::min(min_up, y);
					} else if (can_down) {
						max_down = std::max(max_down, y);
					} else {
						poss = false;
						break;
					}
					if (z == 2) {
						std::cerr << "hi" << can_up << ' ' << can_down << '\n';
					}
				} else {
					can_right[x] = (y > max_down) && (y < min_up);
					if (z == 2) {
						std::cerr << x << ' ' << can_right[x] << '\n';
					}
				}
			}
		}
		assert(poss);
		if (!can_left[horz_xs[z+1]]) poss = false;
		if (!can_right[horz_xs[z]]) poss = false;
		num ways = 1;
		for (int a = z+2; a < int(horz_xs.size()); a++) {
			int x = horz_xs[a];
			if (can_left[x] && can_right[x]) {
				ways += ways;
			} else if (can_left[x] || can_right[x]) {
				// 1 way
			} else {
				poss = false;
				break;
			}
		}
		if (!poss) continue;
		int max_must_left = -1;
		int min_must_right = z;
		for (int a = 0; a < z; a++) {
			int x = horz_xs[a];
			if (!can_right[x]) {
				max_must_left = a;
			}
			if (!can_left[x] && min_must_right == z) {
				min_must_right = a;
			}
		}
		if (min_must_right <= max_must_left) {
			poss = false;
		}
		if (!poss) continue;

		ways *= num(min_must_right - max_must_left);
		std::cerr << "split " << horz_xs[z] << ' ' << horz_xs[z+1] << ' ' << ways << ' ' << min_must_right << '-' << max_must_left << '\n';
		ans_poss = true;
		ans_ways += ways;
	}

	for (int z = 0; z <= int(horz_xs.size()); z++) {
		std::vector<bool> can_down(N);
		{
			int max_left = -1; 
			int min_right = N;
			for (int y = 0; y < N; y++) {
				int x = y_to_x[y];
				if (y_is_horz[y]) {
					if (z == int(horz_xs.size()) || x < horz_xs[z]) {
						max_left = std::max(max_left, x);
					} else {
						// go right
						min_right = std::min(min_right, x);
					}
				} else {
					can_down[y] = (x > max_left) && (x < min_right);
				}
			}
		}
		std::vector<bool> can_up(N);
		{
			int max_left = -1; 
			int min_right = N;
			for (int y = N-1; y >= 0; y--) {
				int x = y_to_x[y];
				if (y_is_horz[y]) {
					if (z == int(horz_xs.size()) || x < horz_xs[z]) {
						max_left = std::max(max_left, x);
					} else {
						// go right
						min_right = std::min(min_right, x);
					}
				} else {
					can_up[y] = (x > max_left) && (x < min_right);
				}
			}
		}
		bool poss = true;
		num ways = 1;
		for (int y = 0; y < N; y++) {
			if (!y_is_horz[y]) {
				if (can_up[y] && can_down[y]) {
					ways += ways;
				} else if (can_up[y] || can_down[y]) {
				} else {
					poss = false;
				}
			}
		}
		if (!poss) continue;
		std::cerr << "normal " << z << ' ' << ways << '\n';
		ans_poss = true;
		ans_ways += ways;
	}

	return {ans_poss, ans_ways};
}

std::pair<bool, num> solve_slow(std::vector<std::pair<std::array<int, 2>, std::string>> pts) {
	int N = int(pts.size());

	bool poss = false;
	num ways = 0;
	for (int msk = 0; msk < (1 << N); msk++) {
		bool good = true;
		for (int i = 0; i < N; i++) {
			if (pts[i].second != "UD") continue;
			for (int j = 0; j < N; j++) {
				if (pts[j].second != "LR") continue;
				if (
					((pts[i].first[0] > pts[j].first[0]) == bool((msk >> j) & 1)) &&
					((pts[j].first[1] < pts[i].first[1]) == bool((msk >> i) & 1))
				) {
					good = false;
				}
			}
		}
		if (good) {
			for (int i = 0; i < N; i++) {
				std::cerr << pts[i].second[(msk >> i) & 1];
			}
			std::cerr << '\n';
			poss = true;
			ways++;
		}
	}
	return {poss, ways};
}


void test() {
	std::mt19937 mt(48);
	for (int z = 0; z < 1000; z++) {
		int N = std::uniform_int_distribution(1, 5)(mt);
		std::vector<std::pair<std::array<int, 2>, std::string>> pts(N);
		std::vector<int> xs(N);
		std::iota(xs.begin(), xs.end(), 0);
		std::shuffle(xs.begin(), xs.end(), mt);
		std::vector<int> ys(N);
		std::iota(ys.begin(), ys.end(), 0);
		std::shuffle(ys.begin(), ys.end(), mt);
		for (int i = 0; i < N; i++) {
			pts[i].first = {xs[i], ys[i]};
			pts[i].second = (mt() & 1) ? "UD" : "LR";
		}
		auto theirs = solve_slow(pts);
		auto mine = solve(pts);
		if (mine != theirs) {
			std::cerr << N << '\n';
			for (auto p : pts) {
				std::cerr << p.first[0]+1 << ' ' << p.first[1]+1 << ' ' << p.second << '\n';
			}
			std::cerr << (theirs.first ? "YES" : "NO") << ' ' << theirs.second << '\n';
			std::cerr << (mine.first ? "YES" : "NO") << ' ' << mine.second << '\n';
		}
	}
}


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<std::array<int, 2>, std::string>> pts(N);
		for (auto& [p, s] : pts) {
			auto& [x, y] = p;
			cin >> x >> y >> s; x--, y--;
		}
		solve_slow(pts);
		auto [ans_poss, ans_ways] = solve(pts);
		cout << (ans_poss ? "YES" : "NO") << '\n';
		cout << ans_ways << '\n';
	}

	return 0;
}
