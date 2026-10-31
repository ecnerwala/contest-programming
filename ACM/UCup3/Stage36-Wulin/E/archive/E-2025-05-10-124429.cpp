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

struct frac_t {
	int64_t n, d;
	friend std::strong_ordering operator <=> (frac_t a, frac_t b) {
		return a.n * b.d <=> b.n * a.d;
	}
};

using num = modnum<int(1e9)+7>;
using poly_t = std::array<num, 3>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	struct pt_t { int64_t x, y; };
	std::vector<pt_t> P(N);
	for (auto& [x, y] : P) {
		cin >> x >> y; y = -y;
		assert(x >= 0);
		assert(y > 0);
	}

	std::vector<std::tuple<frac_t, int, int>> ops;
	ops.reserve(N * N);
	std::vector<int> rnk(N);
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (P[i].x == P[j].x) {
				if (P[i].y <= P[j].y) {
					// i is better
					rnk[j]++;
				} else {
					rnk[i]++;
				}
			} else {
				int l = i, r = j;
				if (P[l].x > P[r].x) std::swap(l, r);
				frac_t a{(P[r].x * P[r].x + P[r].y * P[r].y) - (P[l].x * P[l].x + P[l].y * P[l].y), 2 * (P[r].x - P[l].x)};
				rnk[r]++;
				ops.push_back({a, l, +1});
				ops.push_back({a, r, -1});
			}
		}
	}
	std::sort(ops.begin(), ops.end());

	std::vector<num> ans(Q);
	std::vector<std::tuple<int, int, int, num>> query_ops; query_ops.reserve(2*Q);
	for (int q = 0; q < Q; q++) {
		int a, b, k; cin >> a >> b >> k;
		if (a == b) {
			query_ops.push_back({a, k, ~q, num(1)});
		} else {
			assert(a < b);
			num w = inv(num(b - a));
			query_ops.push_back({a, k, q, -w});
			query_ops.push_back({b, k, q, +w});
		}
	}
	std::sort(query_ops.begin(), query_ops.end(), [&](auto a, auto b) -> bool {
		return std::get<0>(a) < std::get<0>(b);
	});

	std::vector<poly_t> cur_poly(N);
	std::vector<num> cur_integ(N);
	for (int i = 0; i < N; i++) {
		int r = rnk[i];
		cur_poly[r] = {P[i].x * P[i].x + P[i].y * P[i].y, -2 * P[i].x, 1};
	}
	for (int i = 1; i < N; i++) {
		cur_poly[i][0] += cur_poly[i-1][0];
		cur_poly[i][1] += cur_poly[i-1][1];
		cur_poly[i][2] += cur_poly[i-1][2];
	}

	auto integ_at = [&](poly_t a, num x) {
		num r = a[2] / num(3);
		r *= x;
		r += a[1] / num(2);
		r *= x;
		r += a[0];
		r *= x;
		return r;
	};

	auto eval_at = [&](poly_t a, num x) {
		num r = a[2];
		r *= x;
		r += a[1];
		r *= x;
		r += a[0];
		return r;
	};

	auto oit = ops.begin();
	for (auto query_op : query_ops) {
		for (; oit != ops.end() && std::get<0>(*oit) < frac_t{std::get<0>(query_op), 1}; ++oit) {
			// process oit
			auto [x_frac, i, delta] = *oit;
			int old_r = rnk[i];
			rnk[i] += delta;
			int new_r = rnk[i];
			assert(std::abs(delta) == 1);
			int to_change = std::min(old_r, new_r);
			int w = old_r < new_r ? -1 : +1; // if old_r < new_r, then we should subtract from old
			num x = num(x_frac.n) / num(x_frac.d);
			if (0 <= to_change && to_change < N) {
				cur_integ[to_change] += integ_at(cur_poly[to_change], x);
				cur_poly[to_change][0] += w * (P[i].x * P[i].x + P[i].y * P[i].y);
				cur_poly[to_change][1] += w * (-2 * P[i].x);
				cur_poly[to_change][2] += w * (1);
				cur_integ[to_change] -= integ_at(cur_poly[to_change], x);
			}
		}

		{
			auto [x, k, q, w] = query_op;
			if (q >= 0) {
				num v = cur_integ[k-1] + integ_at(cur_poly[k-1], x);
				ans[q] += w * v;
			} else {
				assert(w == 1);
				ans[~q] += eval_at(cur_poly[k-1], x);
			}
		}
	}
	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
