#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

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
		res.v = minv(v, MOD);
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

using num = modnum<int(1e9)+7>;

class BinarySearch {

	string S;
	vector<int> pref_good;

	vector<vector<num>> choose;
	vector<num> fact;

	map<array<int, 5>, num> mem;

	num dfs(int len, int vlo, int vhi, int num_lo, int num_hi) {
		if (len == 0) {
			if (pref_good[vhi+1] - pref_good[vlo] > 0) {
				return 0;
			} else {
				return fact[vhi - vlo + 1];
			}
		}
		array<int, 5> key{len, vlo, vhi, num_lo, num_hi};
		if (mem.count(key)) return mem[key];
		//cerr << len << ' ' << vlo << ' ' << vhi << ' ' << num_lo << ' ' << num_hi << '\n';

		num ans = 0;
		int num_good = pref_good[vhi+1] - pref_good[vlo];

		int lo = 0, hi = len-1;

		int num_bad = len - num_lo - num_hi - num_good;
		assert(num_bad >= 0);
		assert(num_bad + num_good <= (vhi - vlo + 1));

		int mid = (lo + hi) / 2;
		int lhs = mid - lo;
		int rhs = hi - mid;
		assert(len == lhs + rhs + 1);
		if (num_hi > 0) { // we could take a high one
			// fill hi-mid guys
			for (int c_hi = 0; c_hi <= num_hi-1 && c_hi <= rhs; c_hi++) {
				for (int c_lo = 0; c_lo <= num_lo && c_lo + c_hi <= rhs; c_lo++) {
					int c_bad = rhs - c_lo - c_hi;
					if (0 <= c_bad && c_bad <= num_bad) {
						//cerr << "push rhs" << '\n';
						ans += dfs(lhs, vlo, vhi, num_lo - c_lo, num_hi - 1 - c_hi) * choose[rhs][c_lo + c_hi] * choose[c_lo + c_hi][c_hi];
						//cerr << "pop rhs" << '\n';
					}
				}
			}
		}
		if (num_lo > 0) {
			for (int c_hi = 0; c_hi <= num_hi && c_hi <= lhs; c_hi++) {
				for (int c_lo = 0; c_lo <= num_lo-1 && c_lo + c_hi <= lhs; c_lo++) {
					int c_bad = lhs - c_lo - c_hi;
					if (0 <= c_bad && c_bad <= num_bad) {
						//cerr << "push lhs" << '\n';
						ans += dfs(rhs, vlo, vhi, num_lo - 1 - c_lo, num_hi - c_hi) * choose[lhs][c_lo + c_hi] * choose[c_lo + c_hi][c_hi];
						//cerr << "pop lhs" << '\n';
					}
				}
			}
		}

		for (int v = vlo; v <= vhi; v++) {
			if (S[v] == '1') {
				int lbad = (v - vlo) - (pref_good[v] - pref_good[vlo]);
				int rbad = (vhi - v) - (pref_good[vhi+1] - pref_good[v+1]);

				// we try to take this guy as the midpoint
				for (int c_hi = 0; c_hi <= num_hi && c_hi <= lhs; c_hi++) {
					for (int c_lo = 0; c_lo <= num_lo && c_lo + c_hi <= lhs; c_lo++) {
						// how many hi's and lo's do we put on the left
						for (int c_l = 0; c_l <= lbad && c_lo + c_hi + c_l <= lhs; c_l++) {
							int c_r = lhs - c_lo - c_hi - c_l - (pref_good[v] - pref_good[vlo]);
							if (0 <= c_r && c_r <= rbad) {
								for (int d_l = 0; d_l <= lbad - c_l; d_l ++) {
									int d_r = rhs - (num_lo - c_lo) - (num_hi - c_hi) - d_l - (pref_good[vhi+1] - pref_good[v+1]);
									if (0 <= d_r && c_r + d_r <= rbad) {
										//cerr << "push both" << '\n';
										ans +=
											dfs(lhs, vlo, v-1, c_lo, c_hi + c_r) * choose[c_hi + c_r][c_hi]
											* dfs(rhs, v+1, vhi, (num_lo - c_lo) + d_l, (num_hi - c_hi)) * choose[(num_lo - c_lo) + d_l][d_l]
											* choose[lbad - c_l - d_l + rbad - c_r - d_r][lbad - c_l - d_l];
										//cerr << "pop both" << '\n';
									}
								}
							}
						}
					}
				}
			}
		}
		//cerr << len << ' ' << vlo << ' ' << vhi << ' ' << num_lo << ' ' << num_hi << " -> " << ans << '\n';
		return mem[key] = ans;
	}

public:

	int count(string S_) {
		S = std::move(S_);

		int N = int(S.size());
		pref_good.assign(S.size()+1, 0);
		for (int i = 0; i < N; i++) {
			pref_good[i+1] = pref_good[i] + (S[i] == '1');
		}

		choose.resize(N+1);
		for (int i = 0; i <= N; i++) {
			choose[i].resize(i+1);
			choose[i][0] = choose[i][i] = 1;
			for (int j = 1; j < i; j++) {
				choose[i][j] = choose[i-1][j-1] + choose[i-1][j];
			}
		}

		fact.resize(N+1);
		fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);

		mem.clear();
		return int(dfs(int(S.size()), 0, int(S.size())-1, 0, 0));
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::BinarySearch;

// vim:ft=cpp
