#include<bits/stdc++.h>
using namespace std;

int N;
int K;
const int MAXK = 5;
const int MAXN = 20;
const int MAXM = 1e6;

bool W[MAXN][MAXN];
int dp[MAXM]; // stores mask of which players can win a tournament with a playing
int best[MAXN];
int worst[MAXN];

int digsum[MAXM];

int get_place(int wins) {
	return (wins == K) ? 1 : ((1 << (K - 1 - wins)) + 1);
}

int main() {
	digsum[0] = 0;
	for(int i = 0; i < MAXM; i++) {
		digsum[i] = (i & 1) + digsum[i / 2];
	}

	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		cin >> N;
		K = 0;
		while((1 << K) < N) K++;
		assert(N == (1 << K));
		assert(K <= 4);
		for(int i = 0; i < N; i++) {
			best[i] = 0;
			worst[i] = K;
		}
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				cin >> W[i][j];
				if(i != j && !W[i][j]) worst[i] = 0;
			}
		}

		for(int i = 0; i < N; i++) {
			dp[1 << i] = 1 << i;
		}

		for(int l = 2, c = 1; l <= N; l *= 2, c++) {
			for(int m = 0; m < (1 << N); m++) {
				if(digsum[m] != l) continue;
				int v = 0;
				for(int a = m; a > 0; a = (a - 1) & m) {
					if(digsum[a] != l / 2) continue;
					int b = m ^ a;
					if(a > b) continue;

					for(int i = 0; i < N; i++) {
						if(!(dp[a] & (1 << i))) continue;
						for(int j = 0; j < N; j++) {
							if(!(dp[b] & (1 << j))) continue;
							if(W[i][j]) {
								v |= (1 << i);
								best[i] = c;
							} else {
								v |= (1 << j);
								best[j] = c;
							}
						}
					}
				}
				dp[m] = v;
			}
		}

		cout << "Case #" << T << ": \n";
		for(int i = 0; i < N; i++) {
			cout << get_place(best[i]) << ' ' << get_place(worst[i]) << '\n';
		}
	}

	return 0;
}
