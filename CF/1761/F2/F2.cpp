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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;

		int N_even = (N+1)/2;

		std::vector<int> known(N, -1);
		bool has_known = false;
		for (int i = 0; i < N; i++) {
			int v; cin >> v;
			if (v != -1) {
				known[v-1] = (i & 1) ? N - (i+1)/2 : i / 2;
				has_known = true;
			}
		}

		auto flip = [&]() {
			std::reverse(known.begin(), known.end());
		};

		using num = modnum<int(1e9)+7>;

		int V = N;
		std::vector<num> fact(V+1);
		fact[0] = 1;
		for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(V+1);
		ifact[V] = inv(fact[V]);
		for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		auto choose_den = [&](int a, int b) -> num {
			if (0 <= a && 0 <= b) return ifact[a] * ifact[b];
			else return 0;
		};

		auto cross_ways = [&](std::array<int, 2> fr, std::array<int, 2> to) -> num {
			assert(fr[1] - fr[0] <= to[1] - to[0]);
			if (!(0 <= fr[0] + fr[1] && fr[0] + fr[1] < N)) return 0;
			if (!(0 <= to[0] + to[1] && to[0] + to[1] < N)) return 0;
			return choose_den(fr[0] - to[0], to[1] - fr[1])
				- choose_den(fr[0] - (N-to[1]), (N-to[0]) - fr[1])
				- choose_den(fr[0] - (-1-to[1]), (-1-to[0]) - fr[1]);
		};

		if (!has_known) {
			//https://oeis.org/A182555
			//D-finite with recurrence: n*(n-4)*a(n) +2*(5+2n-n^2)*a(n-1) -4*(n-2)*(n-5)*a(n-2) +8*(n-2)*(n-3)*a(n-3)=0. - R. J. Mathar, Jun 28 2012
			std::vector<num> A(std::max(N-1, 5));
			A[0] = 1;
			A[1] = 2;
			A[2] = 5;
			A[3] = 12;
			A[4] = 29;
			for (int i = 5; i <= N-2; i++) {
				num n = i;
				A[i] = (2*(5+2*n-n*n)*A[i-1] -4*(n-2)*(n-5)*A[i-2] +8*(n-2)*(n-3)*A[i-3]) * -ifact[i] * fact[i-1] * ifact[i-4] * fact[i-5];
			}
			cout << 2 * A[N-2] << '\n';
		} else {
			auto solve_even_1 = [&]() -> num {
				std::vector<std::pair<std::array<int, 2>, num>> cnds; cnds.reserve(std::max(2, N_even));
				for (int i = 0; i < N_even; i++) {
					if (known[0] == -1 || known[0] == i) {
						cnds.push_back({{i, i}, num(1)});
					}
				}

				auto to_ways = [&](std::array<int, 2> to) -> num {
					num nways = 0;
					for (auto [fr, ways] : cnds) {
						nways += cross_ways(fr, to) * ways;
					}
					return nways;
				};

				std::vector<std::pair<std::array<int, 2>, num>> ncnds; ncnds.reserve(2);

				num fac = 1;
				int last_v = 0;

				for (int v = 1; v < N-1; v++) {
					int idx = known[v];
					if (idx == -1) continue;
					fac *= fact[v-1-last_v];
					last_v = v;
					ncnds.clear();
					{
						int hi_idx = idx;
						num ways = to_ways({hi_idx-v, hi_idx-1});
						if (ways != 0) ncnds.push_back({{hi_idx-v, hi_idx}, ways});
					}
					{
						int lo_idx = idx < N_even ? idx : idx - N;
						num ways = to_ways({lo_idx+1, lo_idx+v});
						if (ways != 0) ncnds.push_back({{lo_idx, lo_idx+v}, ways});
					}
					std::swap(cnds, ncnds);
				}

				fac *= fact[N-2-last_v];

				num ans = 0;
				for (int i = N_even; i < N; i++) {
					if (known[N-1] == -1 || known[N-1] == i) {
						ans += to_ways({i+1-N, i-1});
					}
				}
				ans *= fac;
				return ans;
			};

			num final_ans = solve_even_1();
			flip();
			final_ans += solve_even_1();
			cout << final_ans << '\n';
		}
	}

	return 0;
}
