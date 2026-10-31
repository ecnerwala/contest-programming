#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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
	for (int case_num = 1; case_num <= T; case_num ++) {
		//cerr << "Case #" << case_num << '\n';

		int N; cin >> N;
		string S; cin >> S;
		assert(int(S.size()) == N);

		std::vector<int> B(N);
		for (auto& b : B) { cin >> b; b--; }

		using num = modnum<int(1e9)+7>;

		num coeff = 1;
		for (char c : S) if (c == '?') coeff += coeff;
		//cerr << "coeff " << coeff << '\n';

		auto solve = [&]() -> num {
			std::vector<std::vector<int>> ch(N);
			for (int i = 0; i < N; i++) ch[B[i]].push_back(i);

			auto char_prob_no_cat = [&](char c) -> num {
				if (c == '?') {
					return inv(num(2));
				} else if (c == '.') {
					return num(1);
				} else if (c == 'C') {
					return num(0);
				} else assert(false);
			};

			std::vector<int> path; path.reserve(N);
			path.push_back(N-1);
			for (int cur = N-1; B[cur] < cur; cur = B[cur]) {
				path.push_back(B[cur]);
			}

			int L = int(path.size());
			assert(L >= 1);

			num p_end_empty = std::y_combinator([&](auto self, int cur) -> num {
				num res = char_prob_no_cat(S[cur]);

				for (int nxt : ch[cur]) {
					if (nxt < cur) {
						res *= self(nxt);
					}
				}
				return res;
			})(path.back());

			//cerr << "p_end_empty " << p_end_empty << '\n';
			num ans = 0;
			for (int end_empty = 0; end_empty < 2; end_empty++) {
				//cerr << "end_empty " << end_empty << '\n';
				// Returns probability that this tree does not have a kitty when it procs
				auto solve_tree = std::y_combinator([&](auto self, int cur) -> num {
					if (cur == path.back()) return num(end_empty);

					num res = char_prob_no_cat(S[cur]);

					for (int nxt : ch[cur]) {
						if (nxt < cur) {
							res *= self(nxt);
						}
					}
					return res;
				});

				num nxt_prob;
				{
					int v = B[path.back()];
					nxt_prob = char_prob_no_cat(S[v]);
					for (int u : ch[v]) {
						if (u < path.back()) {
							nxt_prob *= solve_tree(u);
						}
					}
					//cerr << v << ' ' << nxt_prob << '\n';
				}

				for (int z = L-1; z >= 0; z--) {
					int v = path[z];
					nxt_prob = 1 - (1 - nxt_prob) * (1 - solve_tree(v));
					if (z > 0) {
						assert(v < path[z-1]);
						for (int u : ch[v]) {
							if (u >= v && u < path[z-1]) {
								nxt_prob *= solve_tree(u);
							}
						}
					}
					//cerr << v << ' ' << nxt_prob << '\n';
				}
				ans += nxt_prob * (end_empty ? p_end_empty : 1 - p_end_empty);
			}
			return (1 - ans) * coeff;
		}();
		cout << "Case #" << case_num << ": " << solve << '\n';
	}

	return 0;
}
