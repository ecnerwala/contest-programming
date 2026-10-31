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

using num = modnum<998244353>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; }

	std::vector<num> fact(N+2);
	fact[0] = 1;
	for (int i = 1; i < int(fact.size()); i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(fact.size());
	ifact.back() = inv(fact.back());
	for (int i = int(ifact.size())-1; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	auto mix_ways = [&](int a, int b) {
		return fact[a+b] * ifact[a] * ifact[b];
	};

	// for each odd prefix, return the max # of ops and the number of ways
	auto solve_prefs = [&]() -> std::vector<std::pair<int, num>> {
		std::vector<int> nxt(N+2);
		std::vector<int> prv(N+2);
		for (int i = 0; i < N+2; i++) {
			nxt[i] = i+1;
			prv[i] = i-1;
		}

		// store the floor median
		int cur_med = (N+1)/2;
		std::vector<int> odd_median(N+1);
		std::vector<std::array<int, 2>> even_median(N+1);
		for (int i = N; i >= 0; i--) {
			if (i < N) {
				int v = A[i];
				if (i & 1) {
					// currently even, going to be odd
					if (v <= cur_med) {
						cur_med = nxt[cur_med];
					}
				} else {
					// currently odd, going to be even
					if (v >= cur_med) {
						cur_med = prv[cur_med];
					}
				}

				int p = prv[v];
				int n = nxt[v];
				nxt[p] = n;
				prv[n] = p;

				//prv[v] = nxt[v] = v;
			}

			if (i & 1) {
				odd_median[i] = cur_med;
				//cerr << "median " << odd_median[i] << '\n';
			} else {
				even_median[i] = std::array<int, 2>{cur_med, nxt[cur_med]};
				//cerr << "median " << even_median[i][0] << '-' << even_median[i][1] << '\n';
			}
		}

		std::vector<int> num_even_med(N+3, 0);
		for (int i = 0; i <= N; i += 2) {
			auto [a, b] = even_median[i];
			assert(a+1 <= b);
			num_even_med[a+1]++;
			num_even_med[b]--;
		}
		for (int i = 0; i <= N+1; i++) num_even_med[i+1] += num_even_med[i];
		assert(num_even_med[N+2] == 0);

		std::vector<bool> no_med_op(N+2, false);

		std::vector<int> num_odd_med(N+2, 0);

		std::vector<std::pair<int, num>> ans(N+1);
		for (int i = 1; i <= N; i += 2) {
			int m = odd_median[i];
			num_odd_med[m]++;
			if (m == A[i-1]) no_med_op[m] = true;

			int right_ops = num_odd_med[m]-1;
			int left_ops = num_even_med[m]-1;
			int mid_op = !no_med_op[m];

			ans[i] = {left_ops + mid_op + right_ops, mix_ways(left_ops, right_ops)};
			//cerr << i << ' ' << m << ' ' << left_ops << '/' << mid_op << '/' << right_ops << ' ' << ans[i].first << 'x' << ans[i].second << '\n';
		}
		return ans;
	};

	auto a = solve_prefs();
	if (N & 1) {
		cout << 1 << ' ' << a.back().first << ' ' << a.back().second << '\n';
	} else {
		std::reverse(A.begin(), A.end());
		auto b = solve_prefs();
		std::reverse(b.begin(), b.end());
		std::pair<int, num> ans(-1, 0);
		for (int m = 1; m < N; m += 2) {
			std::pair<int, num> cnd;
			cnd.first = a[m].first + b[m].first;
			cnd.second = a[m].second * b[m].second * mix_ways(a[m].first, b[m].first);
			if (cnd.first > ans.first) ans = cnd;
			else if (cnd.first == ans.first) ans.second += cnd.second;
		}
		cout << 2 << ' ' << ans.first << ' ' << ans.second << '\n';
	}

	return 0;
}
