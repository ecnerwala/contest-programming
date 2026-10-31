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
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
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
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
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

struct subtree {
	int v;
	int sz;
	vector<int> distCnt;
};

class DistancePermutation {

public:

	int solve(vector <int> parent) {
		parent.insert(parent.begin(), -1);
		int N = int(parent.size());

		vector<vector<int>> ch(N);
		for (int i = 1; i < N; i++) {
			ch[parent[i]].push_back(i);
		}

		vector<int> sz(N);

		for (int i = N-1; i >= 0; i--) {
			sz[i] ++;
			if (i) sz[parent[i]] += sz[i];
		}

		vector<vector<int>> furthestDown(N, vector<int>({1}));
		for (int i = N-1; i > 0; i--) {
			if (furthestDown[parent[i]].size() < furthestDown[i].size() + 1) {
				furthestDown[parent[i]].resize(furthestDown[i].size() + 1);
			}
			for (int v = 0; v < int(furthestDown[i].size()); v++) {
				furthestDown[parent[i]][v+1] += furthestDown[i][v];
			}
		}
		vector<vector<int>> furthestUp(N);
		furthestUp[0] = {};

		for (int i = 1; i < N; i++) {
			furthestUp[i] = furthestUp[parent[i]];
			furthestUp[i].insert(furthestUp[i].begin(), 1);

			for (int j : ch[parent[i]]) {
				if (j == i) continue;
				if (furthestUp[i].size() < furthestDown[j].size() + 1) {
					furthestUp[i].resize(furthestDown[j].size() + 1);
				}
				for (int v = 0; v < int(furthestDown[j].size()); v++) {
					furthestUp[i][v+1] += furthestDown[j][v];
				}
			}
		}
		
		vector<vector<subtree>> subtrees(N);

		for (int i = 1; i < N; i++) {
			int p = parent[i];
			subtrees[i].push_back({p, N - sz[i], furthestUp[i]});
			subtrees[p].push_back({i, sz[i], furthestDown[i]});
		}

		vector<num> fact(N+1);
		fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
		vector<num> ifact(N+1);
		ifact[N] = fact[N].inv();
		for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
		auto choose = [&](int n, int k) -> num {
			if (0 <= k && k <= n) return fact[n] * ifact[k] * ifact[n-k];
			else return 0;
		};

		vector<num> pickKweight(N+1);
		for (int k = 0; k <= N; k++) {
			for (int i = 1; i <= k; i++) {
				pickKweight[k] += choose(k, i) / choose(N, i) * fact[N];
			}
		}
		//cerr << "pickKweight" << '\n'; for (int k = 0; k <= N; k++) { cerr << k << ' ' << pickKweight[k] << '\n'; } cerr << '\n';

		num ans = (N > 1) ? num(N) * fact[N] : 0;
		for (int c = 0; c < N; c++) {
			for (const auto& sub : subtrees[c]) {
				//cerr << c << ' '  << sub.v << ' ' << sub.sz << ' ' << int(sub.distCnt.size()) - 1 << '\n';

				for (int d = 0; d < int(sub.distCnt.size()); d++) {
					assert(sub.distCnt[d] > 0);
					num val;
					if (sub.distCnt[d] > 1) {
						// as long as it doesn't include this subtree, we're always good
						val = pickKweight[N - sub.sz];
						// subtract out cases that don't contain the center, i.e. are entirely in one subtree
						for (const auto& sub2 : subtrees[c]) {
							if (sub2.v == sub.v) continue;
							val -= pickKweight[sub2.sz];
						}
					} else {
						vector<int> goods;
						vector<int> bads;
						for (const auto& sub2 : subtrees[c]) {
							if (sub2.v == sub.v) continue;
							if (d < int(sub2.distCnt.size())) {
								// including this guy would be ok too
								goods.push_back(sub2.sz);
							} else {
								bads.push_back(sub2.sz);
							}
						}
						if (goods.empty()) {
							val = 0;
						} else {
							val = pickKweight[N - sub.sz];

							// must not be missing none

							vector<num> dp(N - sub.sz + 1);
							dp[0] = 1;
							for (const auto& sub2 : goods) {
								int v = sub2;
								for (int i = N - sub.sz; i >= v; i--) {
									dp[i] -= dp[i-v];
								}
							}

							for (int i = 0; i <= N - sub.sz; i++) {
								val -= dp[i] * pickKweight[N - sub.sz - i];
							}

							if (goods.size() >= 2) {
								for (const auto& sub2 : goods) {
									val -= pickKweight[sub2];
								}
							}
							for (const auto& sub2 : bads) {
								val -= pickKweight[sub2];
							}
						}
					}

					//cerr << d << ' ' << sub.distCnt[d] << ' ' << val << '\n';
					ans += val * num(sub.distCnt[d]);
				}
			}
		}
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::DistancePermutation;

// vim:ft=cpp
