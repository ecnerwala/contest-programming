#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;

const int MAXN = 4e6;
int N;
int V[2][MAXN];
constexpr int *A = V[0];
constexpr int *B = V[1];

int loc[MAXN][2];

ll go() {
	for (int z = 0; z < 2; z++) {
	}
	return -1;
}

int main() {
	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cin >> N;
		memset(loc, 0, sizeof(loc));
		for (int z = 0; z < 2; z++) {
			cin >> V[z][0];
			int K; cin >> K;
			int st = 1;
			while (K--) {
				int en = st;
				int R, L; cin >> R >> L;
				assert(R >= 1);
				for (int i = 0; i < L; i++) {
					assert(en == st + i);
					cin >> V[z][en++];
				}
				for (int r = 1; r < R; r++) {
					for (int i = 0; i < L; i++) {
						V[z][en++] = V[z][st + i];
					}
				}
				st = en;
			}
			assert(st == N);
			for (int i = 1; i < N; i++) {
				// Convert from D to V
				V[z][i] += V[z][i - 1];
			}
			for (int i = 0; i < N; i++) V[z][i] --;
		}
		ll res = go() % MOD;
		if (res < 0) res += MOD;
		cout << "Case #" << T << ": " << res << '\n';
	}

	return 0;
}
