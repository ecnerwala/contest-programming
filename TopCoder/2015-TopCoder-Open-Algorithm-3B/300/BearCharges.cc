#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

class BearCharges {

	typedef double ld;

	inline ld hypot(ld a, ld b) { return sqrt(a * a + b * b); }

	static const int MAXN = 12;
	int N;
	ld dist[MAXN][MAXN];

	ld dp[MAXN][1 << MAXN];

public:

	double minTime(vector <int> x, vector <int> y) {
		N = int(x.size());
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				dist[i][j] = hypot(x[i] - x[j], y[i] - y[j]);
			}
		}
		for(int m = 0; m < (1 << N); m++) {
			for(int i = 0; i < N; i++) {
				if(!(m & (1 << i))) {
					continue;
				}
				if(m == (1 << i)) {
					dp[i][m] = 0;
					continue;
				}
				ld val = 1e99;
				for(int s = m; s; s = m & (s - 1)) {
					if(s & (1 << i)) continue;
					for(int j = 0; j < N; j++) {
						if(s & (1 << j)) {
							assert((m ^ s) < m);
							assert((m ^ s) & (1 << i));
							assert(i != j);
							assert(s < m);
							val = min(val, dist[i][j] + max(dp[i][m ^ s], dp[j][s]));
						}
					}
				}
				dp[i][m] = val;
			}
		}
		return dp[0][(1 << N) - 1];
	}
};

// vim:ft=cpp
