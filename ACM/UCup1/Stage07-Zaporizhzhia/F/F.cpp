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

using num = modnum<int(1e9)+9>;

std::vector<num> interpolate(std::vector<std::pair<num, num>> pts) {
	// pts must be distinct
	int N = int(pts.size());
	std::vector<num> prod_all(N+1);
	prod_all[0] = 1;
	for (int i = 0; i < N; i++) {
		num a = pts[i].first;
		for (int j = i; j >= 0; j--) {
			prod_all[j+1] += prod_all[j];
			prod_all[j] *= -a;
		}
	}

	std::vector<num> res(N);
	for (int i = 0; i < N; i++) {
		num a = pts[i].first;

		num fac = 1;
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			fac *= a - pts[j].first;
		}
		fac = pts[i].second / fac;

		num v = 0;
		for (int j = N-1; j >= 0; j--) {
			v += prod_all[j+1];
			res[j] += v * fac;
			v *= a;
		}
		assert(v + prod_all[0] == 0);
	}

	return res;
}

std::optional<std::vector<num>> solve(std::vector<std::pair<num, num>> X) {
	int N = int(X.size());
	std::optional<num> y0 = std::nullopt;
	struct merged_pt_t {
		num z;
		int cnt;
		std::array<std::pair<num, num>, 2> pts;
	};
	std::vector<merged_pt_t> merged_pts; merged_pts.reserve(N);
	for (auto [x, y] : X) {
		if (x == 0) {
			assert(!y0);
			y0 = y;
			continue;
		} else {
			num z = x + inv(x);
			merged_pts.push_back({z, 1, {std::pair<num, num>{x, y}, {x, y}}});
		}
	}
	std::sort(merged_pts.begin(), merged_pts.end(), [&](const auto& a, const auto& b) {
		return int(a.z) < int(b.z);
	});
	{
		int L = 0;
		for (int i = 0; i < int(merged_pts.size()); i++) {
			if (i == 0 || merged_pts[i].z != merged_pts[i-1].z) {
				merged_pts[L++] = merged_pts[i];
			} else {
				assert(merged_pts[L-1].cnt == 1);
				assert(merged_pts[i].cnt == 1);
				assert(i > L-1);
				merged_pts[L-1].pts[merged_pts[L-1].cnt++] = merged_pts[i].pts[0];
			}
		}
		merged_pts.resize(L);
	}
	N = int(merged_pts.size());

	bool has_neg_1 = false;

	const int MAX_D = int(1e4);
	std::vector<bool> can_degree(MAX_D+1, true);
	for (const auto& p : merged_pts) {
		if (p.z == num(-2)) {
			assert(p.cnt == 1 && p.pts[0].first == num(-1));
			has_neg_1 = true;
			if (p.pts[0].second == num(0)) {
				// then this guy imposes no constraint on odds, and some constraint on evens
			} else {
				// this guy rules out odd palindromes
				for (int d = 1; d <= MAX_D; d += 2) {
					can_degree[d] = false;
				}
			}
			continue;
		}
		if (p.cnt == 1) continue;
		assert(p.cnt == 2);
		num x1 = p.pts[0].first, y1 = p.pts[0].second;
		num inv_x1 = inv(x1);
		num x2 = p.pts[1].first, y2 = p.pts[1].second;
		num inv_x2 = inv(x2);

		num y1_odd = y1 / (x1+1);
		num y2_odd = y2 / (x2+1);
		for (int d = 0; d <= MAX_D; d++) {
			if (d % 2 == 0) {
				if (y1 != y2) can_degree[d] = false;
				y1 *= inv_x1;
				y2 *= inv_x2;
			} else {
				if (y1_odd != y2_odd) can_degree[d] = false;
				y1_odd *= inv_x1;
				y2_odd *= inv_x2;
			}
		}
	}

	if (y0 && *y0 == 0) return std::nullopt;

	bool must_work = false;
	std::vector<int> deg_cnds; deg_cnds.reserve(MAX_D+1);
	for (int d = 0; d <= MAX_D; d++) {
		if (can_degree[d]) {
			deg_cnds.push_back(d);

			int num_constraints = N - (d % 2 == 1 && has_neg_1);
			int num_freedom = d / 2 + 1;
			if (num_freedom > num_constraints) {
				must_work = true;
				break;
			}
		}
	}
	if (deg_cnds.empty()) return std::nullopt;
	std::reverse(deg_cnds.begin(), deg_cnds.end());
	for (int d : deg_cnds) {
		// either we will have find one early, or there will be O(1) candidates
		int num_constraints = N;
		if (d % 2 == 1 && has_neg_1) {
			num_constraints--;
		}
		int num_freedom = d / 2 + 1;

		num cur_y0;
		if (num_freedom > num_constraints) {
			cur_y0 = y0 ? *y0 : num(1);
			assert(cur_y0 != 0);
		} else {
			cur_y0 = num(0);
		}

		std::vector<std::pair<num, num>> constraints;
		constraints.reserve(num_constraints);
		for (const auto& p : merged_pts) {
			if (p.z == num(-2)) {
				if (d % 2 == 1) {
					assert(p.pts[0].second == num(0));
					continue;
				}
			}

			num v1 = p.pts[0].second / pow(p.pts[0].first, d/2) / (d % 2 == 1 ? p.pts[0].first + 1 : 1);
			if (p.cnt == 2) {
				num v2 = p.pts[1].second / pow(p.pts[1].first, d/2) / (d % 2 == 1 ? p.pts[1].first + 1 : 1);
				assert(v1 == v2);
			}
			constraints.push_back({p.z, v1 - cur_y0 * pow(p.z, num_freedom - 1)});
		}
		assert(int(constraints.size()) == int(num_constraints));

		auto res = interpolate(constraints);
		assert(int(res.size()) == num_constraints);
		if (num_freedom <= num_constraints) {
			assert(cur_y0 == 0);
			bool good = true;
			while (int(res.size()) > num_freedom) {
				if (res.back() != 0) {
					good = false;
					break;
				}
				res.pop_back();
			}
			if (!good) continue;
			assert(int(res.size()) == num_freedom);
			if (res.back() == 0) continue;
			if (y0 && res.back() != y0) continue;
		} else {
			res.resize(num_freedom);
			res.back() += cur_y0;
			assert(res.back() != 0);
			if (y0) assert(res.back() == *y0);
			// this one is guaranteed to work
		}

		// Now, we just finish
		std::vector<num> poly; poly.reserve(d+1);
		poly.push_back(res.back());
		for (int i = d/2 - 1; i >= 0; i--) {
			// multiply by 1 + x^2
			poly.push_back(num(0));
			poly.push_back(num(0));

			for (int j = int(poly.size()) - 3; j >= 0; j--) {
				poly[j+2] += poly[j];
			}

			poly[d/2-i] += res[i];
		}
		if (d & 1) {
			poly.push_back(num(0));
			for (int j = int(poly.size()) - 2; j >= 0; j--) {
				poly[j+1] += poly[j];
			}
		}
		return poly;
	}
	assert(!must_work);
	return std::nullopt;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<num, num>> X(N);
		for (auto& [x, _] : X) cin >> x;
		for (auto& [_, y] : X) cin >> y;

		auto res = solve(X);
		if (!res) {
			cout << -1 << '\n';
		} else {
			const auto& v = *res;
			cout << int(v.size()) - 1 << '\n';
			for (int i = 0; i < int(v.size()); i++) {
				cout << v[int(v.size())-1-i] << " \n"[i+1==int(v.size())];
			}
			assert(!v.empty());
			assert(v.back() != num(0));
			for (int i = 0; i < int(v.size()); i++) {
				assert(v[i] == v[int(v.size())-1-i]);
			}
			for (auto [x, y] : X) {
				num cur_y = 0;
				for (int i = int(v.size())-1; i >= 0; i--) {
					cur_y *= x;
					cur_y += v[i];
				}
				assert(cur_y == y);
			}
		}
	}

	return 0;
}
