#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;
namespace ecnerwala {

using ll = long long;
using lll = __int128_t;
struct frac {
	ll p, q;
	frac() : p(0), q(1) {}
	frac(ll p_, ll q_) : p(p_), q(q_) {
		while (p_) {
			ll m = q_ % p_;
			q_ = p_;
			p_ = m;
		}
		p /= q_, q /= q_;
	}
	friend frac operator + (const frac& a, const frac& b) {
		return frac(a.p * b.q + b.p * a.q, a.q * b.q);
	}
	friend frac operator - (const frac& a, const frac& b) {
		return frac(a.p * b.q - b.p * a.q, a.q * b.q);
	}
	friend bool operator < (const frac& a, const frac& b) {
		return a.p * b.q < b.p * a.q;
	}
	friend bool operator == (const frac& a, const frac& b) {
		return a.p == b.p && a.q == b.q;
	}
	friend ostream& operator << (ostream& o, const frac& f) { return o << f.p << '/' << f.q;}
};


const int MAXN = 150;
int maxL[MAXN];
frac dp[MAXN][MAXN];
frac dist0[MAXN];
frac dist1[MAXN];

class FillInTheDAG {

public:

	vector<ll> findWay(int N, vector <int> F, vector <int> T) {
		assert(F.size() == T.size());
		int M = int(F.size());
		vector<vector<int>> outEdges(N);
		vector<vector<int>> inEdges(N);
		for (int e = 0; e < M; e++) {
			outEdges[F[e]].push_back(T[e]);
			inEdges[T[e]].push_back(F[e]);
		}

		int numDone = 2;
		vector<bool> done(N);
		vector<frac> ans(N);
		done[0] = done[N-1] = true;
		ans[0] = frac{0,1};
		ans[N-1] = frac{1,1};

		while (numDone < N) {
			for (int i = 0; i <= N; i++) maxL[i] = 0;
			maxL[0] = 0;
			for (int i = 0; i < N; i++) {
				for (int j : outEdges[i]) {
					if (done[i] && done[j]) {
						maxL[j] = max(maxL[j], maxL[i]);
					} else {
						maxL[j] = max(maxL[j], maxL[i]+1);
					}
				}
			}
			for (int i = 0; i <= N; i++) {
				for (int l = 0; l <= maxL[i]; l++) {
					dp[i][l] = frac{0,1};
				}
			}

			for (int i = 0; i < N; i++) {
				for (int j : outEdges[i]) {
					for (int l = 0; l <= maxL[i]; l++) {
						if (done[i] && done[j]) {
							dp[j][l] = max(dp[j][l], dp[i][l] - ans[i] + ans[j]);
						} else {
							dp[j][l+1] = max(dp[j][l+1], dp[i][l]);
						}
					}
				}
			}

			assert(maxL[N-1] >= 1);
			frac v = frac{1,1};
			for (int l = 1; l <= maxL[N-1]; l++) {
				frac cnd = frac{1,1} - dp[N-1][l];
				cnd = frac(cnd.p, cnd.q * l);
				v = min(v, cnd);
			}

			for (int i = 0; i < N; i++) {
				if (done[i]) {
					dist0[i] = ans[i], dist1[i] = frac{1,1} - ans[i];
				} else {
					dist0[i] = dist1[i] = frac{0,1};
				}
			}
			for (int i = 0; i < N; i++) {
				for (int j : outEdges[i]) {
					if (done[i] && done[j]) {
					} else {
						dist0[j] = max(dist0[j], dist0[i] + v);
					}
				}
			}
			for (int i = N-1; i > 0; i--) {
				for (int j : inEdges[i]) {
					if (done[i] && done[j]) {
					} else {
						dist1[j] = max(dist1[j], dist1[i] + v);
					}
				}
			}
			for (int i = 0; i < N; i++) {
				if (done[i]) continue;
				if (dist0[i] + dist1[i] == frac{1,1}) {
					ans[i] = dist0[i];
					done[i] = true;
					numDone++;
				}
			}
		}

		vector<ll> realAns; realAns.reserve(2*N);
		for (int i = 0; i < N; i++) {
			realAns.push_back(ans[i].p);
			realAns.push_back(ans[i].q);
		}
		return realAns;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FillInTheDAG;

// vim:ft=cpp
