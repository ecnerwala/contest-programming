#include <bits/stdc++.h>
#include <cassert>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
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
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(unsigned v_) : v(int(v_ % MOD)) { }
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(uint64_t v_) : v(int(v_ % MOD)) { }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string S; std::cin >> S;
		S.insert(S.begin(), '0');
		assert(int(S.size()) == N);
		std::vector<std::vector<int>> ch(N);
		for (int i = 0; i < N; i++) {
			int l; std::cin >> l;
			ch[i].resize(l);
			for (auto& j : ch[i]) { std::cin >> j; j--; }
		}

		// Count everything by the canonical one
		using num = modnum<int(1e9)+7>;
		std::vector<std::vector<std::array<num, 6>>> dp(N);
		[&](this auto&& self, int cur) -> void {
			// we store for the subtree:
			// whether there's an active bad-cut
			// whether there's a next bad-cut, and its depth, else our own depth
			dp[cur] = std::vector<std::array<num, 6>>(1, {num(1), num(0), num(0), num(0), num(1), num(0)});
			// has_active == 2 is a special state:
			// it stores the number of ways of picking the maximal active set ending at this point that doesn't use S='1'
			// (obviously it can't use S='0' either, so it must be only S='?')
			for (int nxt : ch[cur]) {
				self(nxt);
				auto ndp = std::vector<std::array<num, 6>>(std::max(int(dp[cur].size()), int(dp[nxt].size()) + 1), {num(0), num(0), num(0), num(0), num(0), num(0)});
				for (int d0 = 0; d0 < int(dp[cur].size()); d0++) {
					for (int d1 = 0; d1 < int(dp[nxt].size()); d1++) {
						for (const int cur_has_active : {0, 1, 2}) {
							for (const bool cur_has_nxt : {0, 1}) {
								for (const int nxt_has_active : {0, 1, 2}) {
									for (const bool nxt_has_nxt : {0, 1}) {
										num v_cur = dp[cur][d0][cur_has_active * 2 + cur_has_nxt];
										num v_nxt = dp[nxt][d1][nxt_has_active * 2 + nxt_has_nxt];
										num v = v_cur * v_nxt;
										for (bool e : {0, 1}) {
											if (S[nxt] != '?' && S[nxt] != char('0' + e)) continue;

											int n_nxt_has_active;
											bool n_nxt_has_nxt;
											int nd1;
											if (e && nxt_has_active) {
												n_nxt_has_active = 0;
												n_nxt_has_nxt = true;
												nd1 = 1;
											} else {
												n_nxt_has_active = nxt_has_active;
												n_nxt_has_nxt = nxt_has_nxt;
												nd1 = d1 + e;
											}
											if (cur_has_active && n_nxt_has_active) continue;
											if (cur_has_nxt && n_nxt_has_nxt) continue;
											if (cur_has_active && !e) continue;

											if (n_nxt_has_nxt && d0 > nd1) continue;
											if (cur_has_nxt && d0 <= nd1) continue;
											int nd = std::max(d0, nd1);

											int n_has_nxt = cur_has_nxt || n_nxt_has_nxt;

											if (n_nxt_has_active && d0 > 0) continue;

											if (cur_has_active == 2 && nxt_has_active == 2) continue;

											if (cur_has_active == 2 || nxt_has_active == 2) {
												// don't handle these the normal way
												if (S[nxt] == '0') {
													// we want to copy up nxt_has_active only
													if (nxt_has_active != 2) continue;
													// just sanity check some state things now
													assert(e == 0);
													assert(n_nxt_has_active == 2);
													assert(!cur_has_active);
													assert(d0 == 0);
													ndp[nd][2 * 2 + n_has_nxt] += v;
												} else if (S[nxt] == '1') {
													// the only thing is we want to add the e 
													assert(e == 1);
													if (cur_has_active != 2) continue;
													if (nxt_has_active == 2) continue;
													assert(cur_has_active == 2 && nxt_has_active != 2);
													assert(!n_nxt_has_active);
													// we want to add a copy from cur_has_active -> has_active == 1
													// because this guy is actually legal
													ndp[nd][2 + n_has_nxt] += v;
												} else if (S[nxt] == '?') {
													// we go cur_has_active == 2
													if (cur_has_active != 2) continue;
													if (nxt_has_active == 2) continue;
													if (e == 0) continue;
													assert(cur_has_active == 2 && nxt_has_active != 2);
													ndp[nd][2 * 2 + n_has_nxt] += v;
												} else assert(false);
												continue;
											}

											ndp[nd][(cur_has_active || n_nxt_has_active) * 2 + n_has_nxt] += v;

											if (!cur_has_active && e && d0 == 0 && S[nxt] != '1') {
												// just set ourselves to active
												ndp[nd][2 + n_has_nxt] -= v;
											}
										}
									}
								}
							}
						}
					}
				}
				dp[cur] = ndp;
			}
		}(0);
		num ans = 0;
		for (auto v : dp[0]) ans += v[0] + v[1] + v[2] + v[3];
		std::cout << ans << '\n';
	}

	return 0;
}
