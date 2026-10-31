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
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(ll(v) * ll(o.v) % MOD);
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

		using num = modnum<int(1e9)+7>;

		int N; int64_t M; int L; cin >> N >> M >> L;
		vector<int> A(1 << L);
		for (auto& a : A) { cin >> a; a--; }

		auto p2 = [](int a) -> uint32_t { return uint32_t(1) << a; };

		uint32_t l_has = 0, r_has = 0;
		for (int i = 0; i < (1 << L); i++) {
			((i < (1 << (L-1))) ? l_has : r_has) |= p2(A[i]);
		}
		uint32_t both_has = l_has & r_has;
		uint32_t l_only = l_has & ~r_has;
		uint32_t r_only = r_has & ~l_has;

		int NL = __builtin_popcount(l_only);
		int NR = __builtin_popcount(r_only);

		int NB = __builtin_popcount(l_has | r_has);

		vector<num> dp(NB+1);

		vector<int> cur; cur.reserve(1 << (L-1));
		vector<int> nxt; nxt.reserve(1 << (L-1));
		for (uint32_t m_both = both_has; true; m_both = (m_both-1) & both_has) {
			vector<array<num, 2>> dp_l(NL+1);
			for (uint32_t m_left = l_only; true; m_left = (m_left-1) & l_only) {
				uint32_t m = m_left | m_both;

				cur.resize(1 << (L-1));
				for (int i = 0; i < (1 << (L-1)); i++) {
					cur[i] = bool(m & p2(A[i]));
				}
				for (int t = L-2; t >= 0; t--) {
					nxt.resize(1 << t);
					for (int i = 0; i < (1 << t); i++) {
						// -1 is max
						nxt[i] = (t & 1) ? max(cur[2*i], cur[2*i+1]) : min(cur[2*i], cur[2*i+1]);
					}
					std::swap(cur, nxt);
				}

				dp_l[__builtin_popcount(m_left)][cur[0]] ++;

				if (m_left == 0) break;
			}
			vector<array<num, 2>> dp_r(NR+1);
			for (uint32_t m_right = r_only; true; m_right = (m_right-1) & r_only) {
				uint32_t m = m_right | m_both;

				cur.resize(1 << (L-1));
				for (int i = 0; i < (1 << (L-1)); i++) {
					cur[i] = bool(m & p2(A[i + (1 << (L-1))]));
				}
				for (int t = L-2; t >= 0; t--) {
					nxt.resize(1 << t);
					for (int i = 0; i < (1 << t); i++) {
						// -1 is max
						nxt[i] = (t & 1) ? max(cur[2*i], cur[2*i+1]) : min(cur[2*i], cur[2*i+1]);
					}
					std::swap(cur, nxt);
				}

				dp_r[__builtin_popcount(m_right)][cur[0]] ++;

				if (m_right == 0) break;
			}

			int p = __builtin_popcount(m_both);
			for (int i = 0; i <= NL; i++) {
				for (int j = 0; j <= NR; j++) {
					for (int a = 0; a < 2; a++) {
						for (int b = 0; b < 2; b++) {
							if (max(a, b) == 1) {
								dp[p + i + j] += dp_l[i][a] * dp_r[j][b];
							}
						}
					}
				}
			}
			if (m_both == 0) break;
		}

		vector<num> ans_small(2*NB+10);
		for (int m = 0; m < int(ans_small.size()); m++) {
			for (int v = 0; v <= NB; v++) {
				for (int i = 0; i <= m; i++) {
					ans_small[m] += dp[v] * pow(num(m-i), v) * pow(num(i), NB-v);
				}
			}
		}

		num ans = 0;
		for (int m = 0; m < int(ans_small.size()); m++) {
			num v = ans_small[m];
			for (int j = 0; j < int(ans_small.size()); j++) {
				if (j == m) continue;
				v *= num(M - j);
				v /= num(m - j);
			}
			ans += v;
		}

		ans *= pow(num(M), N - NB);
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
