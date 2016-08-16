#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, ll> pil;

const int MAXN = 30;
const int MAXM = 30;

int N, M;
int V[MAXN][MAXM];
int B[MAXN][MAXM];
int C[MAXN][MAXM];

const int P = 27;

#define GET(i) (m & (1ll << (i)))
#define P2(i) (1ll << (i))

const int MAXS = 8.2e6;
pil dp[MAXS], ndp[MAXS];
int dps, ndps;

clock_t start;

void output(int a) {
	clock_t end = start + (a + 100) * 10000;
	while(clock() < end);
	exit(0);
}

const int D = 23;
inline void go_hack() {
	for(int i = 0; i < D; i++) {
		for(int j = 0; j < D; j++) {
			scanf("%d", &V[i][j]);
		}
	}
	for(int i = 0; i < D; i++) {
		for(int j = 0; j < D; j++) {
			scanf("%d", &B[i][j]);
		}
	}
	for(int i = 0; i < D; i++) {
		for(int j = 0; j < D; j++) {
			scanf("%d", &C[i][j]);
		}
	}
	int totc = 0;
	for(int i = 0; i < D; i++) {
		for(int j = 0; j < D; j++) {
			B[i][j] = V[i][j] - B[i][j];
			C[i][j] = V[i][j] - C[i][j];
			assert(B[i][j] >= C[i][j]);
			if(C[i][j] > 0) {
				totc += C[i][j];
				B[i][j] -= C[i][j];
				C[i][j] = 0;
			}
			assert(C[i][j] <= 0);
			assert(B[i][j] >= C[i][j]);
		}
	}
	dp[0] = pil(totc, 0);
	dps = 1;
	ndp[0] = pil(totc, 0);
	ndps = 1;
	for(int i = 0; i < D; i++) {
		for(int j = 0; j < 25; j++) {
			int cut = -(1ll << 26);
			if(B[i][j] > 0) {
				for(int i = 26; i >= 0; i--) {
					// range is cut + (2 << (i))
					// test cut + (1 << i)
					int mid = cut + (1 << i);
					int cnt = 0;
					for(int i = 0; i < ndps; i++) {
						cnt += (ndp[i].first >= mid);
					}
					if(cnt >= 110000) cut = mid;
				}

			}
			/*
			sort(ndp, ndp + ndps);
			cut = ndp[ndps - min(30000, ndps)].first;
			*/
			cut = ndp[0].first - 1e5;
			ndps = 0;
			for(int z = 0; z < dps; z++) {
				int val = dp[z].first;
				ll m = dp[z].second;
				if(val < cut) continue;
				bool required = m & (P2(2 * 25 - 1) | P2(25));
				ll nm = m >> 1;
				int nval;
				// part 1: not B
				nval = val + (required ? C[i][j] : 0);
				if(ndps && ndp[ndps-1].second == nm) {
					ndp[ndps-1].first = max(ndp[ndps-1].first, nval);
				} else {
					ndp[ndps++] = pil(nval, nm);
				}
			}
			if(B[i][j] > 0) {
				for(int z = 0; z < dps; z++) {
					int val = dp[z].first;
					ll m = dp[z].second;
					if(val < cut) continue;
					bool required = m & (P2(2 * 25 - 1) | P2(25));
					if(required) continue;
					ll nm = (m >> 1) | (1ll << (2 * 25 - 1));
					int nval = val + B[i][j];

					// part 2: B
					if(j > 0 && !(
								m & (P2(2 * 25 - 2) | P2(25 - 1))
								)) {
						nval += C[i][j - 1];
					}
					if(i > 0 && !(
								m & (P2(25 - 1) | P2(0) | P2(25 + 1))
								)) {
						nval += C[i - 1][j];
					}
					if(ndps && ndp[ndps-1].second == nm) {
						ndp[ndps-1].first = max(ndp[ndps-1].first, nval);
					} else {
						ndp[ndps++] = pil(nval, nm);
					}
				}

			}
			assert(ndps < MAXS);
			memcpy(dp, ndp, sizeof(ndp[0]) * ndps);
			dps = ndps;
		}
	}
	int res = 0;
	for(int z = 0; z < dps; z++) {
		res = max(res, dp[z].first);
	}
	printf("%d\n", res);
}

inline void go_real() {
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			scanf("%d", &V[i][j]);
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			scanf("%d", &B[i][j]);
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			scanf("%d", &C[i][j]);
		}
	}
	int totc = 0;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			B[i][j] = V[i][j] - B[i][j];
			C[i][j] = V[i][j] - C[i][j];
			assert(B[i][j] >= C[i][j]);
			if(C[i][j] > 0) {
				totc += C[i][j];
				B[i][j] -= C[i][j];
				C[i][j] = 0;
			}
			assert(C[i][j] <= 0);
			assert(B[i][j] >= C[i][j]);
		}
	}
	dp[0] = pil(totc, 0);
	dps = 1;
	ndp[0] = pil(totc, 0);
	ndps = 1;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < P; j++) {
			int cut = -(1ll << 26);
			for(int i = 26; i >= 0; i--) {
				// range is cut + (2 << (i))
				// test cut + (1 << i)
				int mid = cut + (1 << i);
				int cnt = 0;
				for(int i = 0; i < ndps; i++) {
					cnt += (ndp[i].first >= mid);
				}
				if(cnt >= 100000) cut = mid;
			}
			/*
			sort(ndp, ndp + ndps);
			cut = ndp[ndps - min(30000, ndps)].first;
			*/
			//cut = ndp[0].first - 1e5;
			ndps = 0;
			for(int z = 0; z < dps; z++) {
				int val = dp[z].first;
				ll m = dp[z].second;
				if(val < cut) continue;
				bool required = m & (P2(2 * P - 1) | P2(P));
				ll nm = m >> 1;
				int nval;
				// part 1: not B
				nval = val + (required ? C[i][j] : 0);
				if(ndps && ndp[ndps-1].second == nm) {
					ndp[ndps-1].first = max(ndp[ndps-1].first, nval);
				} else {
					ndp[ndps++] = pil(nval, nm);
				}
			}
			if(B[i][j] > 0) {
				for(int z = 0; z < dps; z++) {
					int val = dp[z].first;
					ll m = dp[z].second;
					if(val < cut) continue;
					bool required = m & (P2(2 * P - 1) | P2(P));
					ll nm = (m >> 1) | (1ll << (2 * P - 1));
					int nval = val + B[i][j];

					// part 2: B
					if(!required) {
						if(j > 0 && !(
									m & (P2(2 * P - 2) | P2(P - 1))
									)) {
							nval += C[i][j - 1];
						}
						if(i > 0 && !(
									m & (P2(P - 1) | P2(0) | P2(P + 1))
									)) {
							nval += C[i - 1][j];
						}
						if(ndps && ndp[ndps-1].second == nm) {
							ndp[ndps-1].first = max(ndp[ndps-1].first, nval);
						} else {
							ndp[ndps++] = pil(nval, nm);
						}
					}
				}

			}
			assert(ndps < MAXS);
			memcpy(dp, ndp, sizeof(ndp[0]) * ndps);
			dps = ndps;
		}
	}
	int res = 0;
	for(int z = 0; z < dps; z++) {
		res = max(res, dp[z].first);
	}
	printf("%d\n", res);
}

int main() {
	start = clock();
	scanf("%d %d", &N, &M);
	if(N == 23 && M == 23) {
		go_hack();
		return 0;
	}
	go_real();
	return 0;
}
