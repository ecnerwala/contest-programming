#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e5;
const int MAXK = 200;
int N, K;

pii P[MAXN];
int dp[MAXN][MAXK];
int dp2[MAXN];

int go() {
	assert(N);
	assert(K <= N);
	if (K == N) return 0;

	assert(K < N);

	int all = P[0].B - P[0].A;
	for (int i = 1; i < N; i++) {
		all += P[i].B - max(P[i-1].B, P[i].A);
	}
	if (K == 0) return all;
	for (int i = 0; i + K + 1 < N; i++) {
		assert(P[i].B != P[i+K+1].A);
		if (P[i].B > P[i+K+1].A) {
			return all;
		}
	}

	P[N].A = P[N-1].B + 1;
	P[N].B = P[N].A + 1;

	dp2[0] = 0;
	for (int j = 1; j <= K; j++) dp2[j] = -all;
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= K; j++) {
			dp[i][j] = dp2[j] + (P[i].B - P[i].A);
		}
		for (int j = K; j > 0; j --) {
			dp2[j] = dp2[j-1];
		}
		// might as well let dp2[0] be
		for (int j = 0; j < i && j < K; j++) {
			// skip j things
			if (P[i].A < P[i-j-1].B && P[i-j-1].B < P[i+1].A) {
				for (int k = j; k <= K; k++) {
					dp[i][k] = max(dp[i][k], dp[i-j-1][k-j] + (P[i].B - P[i-j-1].B));
				}
				// also update dp2, skip j and yourself
				for (int k = j+1; k <= K; k++) {
					dp2[k] = max(dp2[k], dp[i-j-1][k-j-1]);
				}
			}
		}
		if (P[i].B < P[i+1].A) {
			for (int k = 0; k <= K; k++) {
				dp2[k] = max(dp2[k], dp[i][k]);
			}
		}
	}
	return dp[N][K]-1;
}

int main() {
	if(fopen("lifeguards.in", "r")) {
		freopen("lifeguards.in", "r", stdin);
		freopen("lifeguards.out", "w", stdout);
	}

	scanf("%d %d\n", &N, &K);

	for (int i = 0; i < N; i++) {
		scanf("%d %d", &P[i].A, &P[i].B);
	}
	sort(P, P + N);
	{
		assert(N);
		int N2 = 1;
		for (int i = 1; i < N; i++) {
			assert(P[N2-1].A < P[i].A);
			assert(P[N2-1].B != P[i].B);
			if (P[N2-1].B < P[i].B) {
				P[N2++] = P[i];
			} else {
				// skip this guy, and remove if from the K
				K--;
			}
		}
		N = N2;
	}
	if (K <= 0) {
		K = 0;
	}
	for (int i = 1; i < N; i++) {
		assert(P[i-1].A < P[i].A);
		assert(P[i-1].B < P[i].B);
	}

	cout << go() << '\n';

	return 0;
}
