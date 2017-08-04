#include "wiring.h"
#include <bits/stdc++.h>

namespace ecnerwala {
using namespace std;

typedef long long ll;
const ll INF = 1e18;

struct wiring {
	static const int MAXN = int(1e5) + 200;
	int N[2];
	ll V[2][MAXN];
	int closest[2][MAXN];
	ll sums[2][MAXN];
	ll A[MAXN];

	ll dp[2][MAXN];
	pair<int, int> prv[2 * MAXN];
	ll go() {
		for (int i = 0; i <= N[0] + N[1]; i++) {
			prv[i] = pair<int, int>(-1, -1);
		}
		prv[N[0]] = pair<int, int>(0,0);
		dp[0][0] = dp[1][0] = 0;
		int cur[2] = {1, 1};
		while (true) {
			int z = (V[1][cur[1]] < V[0][cur[0]]);
			int i = cur[z];
			int j = closest[z][i];
			assert(j >= 1);
			assert(j <= N[!z] || i == N[z]+1);
			int diff = z ? (i - j + N[0]) : (j - i + N[0]);

			ll val = INF;
			assert(j-1 < cur[!z]);
			assert(closest[!z][j-1] <= i);
			if (closest[!z][j-1] >= i-1) {
				val = min(val, dp[!z][j-1]);
			}
			assert(i-1 < cur[z]);
			assert(closest[z][i-1] <= j);
			if (closest[z][i-1] >= j-1) {
				val = min(val, dp[z][i-1]);
			}
			if (prv[diff].first != -1) {
				int pz = prv[diff].first;
				int pi = prv[diff].second;
				int pj = closest[pz][pi];
				ll cnd = dp[pz][pi];
				assert(cnd < INF);
				if (pz != z) {
					pz = !pz;
					swap(pi, pj);
				}
				assert(pz == z);
				assert((i-j) == (pi - pj));
				if (pi == i) {
					// the other side was already computed, so just subtract it off
					cnd -= abs(V[pz][pi] - V[!pz][pj]);
				} else {
					assert(pi <= i-1);
					cnd += abs((sums[z][i-1] - sums[pz][pi]) - (sums[!z][j-1] - sums[!pz][pj]));
				}
				val = min(val, cnd);
			}
			if (i == N[z]+1) {
				assert(j == N[!z]+1);
				return val;
			}
			val += abs(V[z][i] - V[!z][j]);
			dp[z][i] = val;
			prv[diff] = pair<int, int>(z, i);
			cur[z] ++;
		}
		assert(false);
	}

	wiring(vector<int> r, vector<int> b) {
		N[0] = int(r.size());
		N[1] = int(b.size());
		V[0][0] = V[1][0] = -INF;
		for (int i = 0; i < N[0]; i++) {
			V[0][i+1] = r[i];
		}
		for (int i = 0; i < N[1]; i++) {
			V[1][i+1] = b[i];
		}
		V[0][N[0]+1] = V[1][N[1]+1] = INF;
		for (int z = 0; z < 2; z++) {
			for (int i = 0, j = 0; i <= N[z]+1; i++) {
				assert(j <= N[!z]+1);
				while (V[!z][j] < V[z][i]) {
					j++;
				}
				assert(j <= N[!z]+1);
				if (j == 0 || V[!z][j] - V[z][i] < V[z][i] - V[!z][j-1]) {
					closest[z][i] = j;
				} else {
					closest[z][i] = j-1;
				}
			}
		}
		for (int z = 0; z < 2; z++) {
			for (int i = 1; i <= N[z]; i++) {
				assert(1 <= closest[z][i] && closest[z][i] <= N[!z]);
			}
		}
		for (int z = 0; z < 2; z++) {
			sums[z][0] = 0;
			for (int i = 1; i <= N[z]; i++) {
				sums[z][i] = sums[z][i-1] + V[z][i];
			}
		}
	}
};
}

long long min_total_length(std::vector<int> r, std::vector<int> b) {
	ecnerwala::wiring w(r, b);
	return w.go();
}
