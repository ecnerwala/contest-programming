#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9) + 7;

const int MAXN = 2000;
int N;
char G[3][MAXN];

int go() {
	assert(N > 0);
	assert(N + 1 < MAXN);
	if (N % 2 == 1) return 0;
	if (G[0][0] == '#') return 0;
	if (G[2][N-1] == '#') return 0;

	for (int i = 0; i < N; i++) {
		if (G[1][i] == '#') return 0;
	}

	long long res = 1;
	for (int i = 1; i < N-1; i+=2) {
		int c = int(G[0][i] == '.' && G[0][i+1] == '.') + int(G[2][i] == '.' && G[2][i+1] == '.');
		res *= c;
		res %= MOD;
	}
	return int((res % MOD + MOD) % MOD);
}

int main() {
	int NUM_CASES; cin >> NUM_CASES;
	for (int t = 1; t <= NUM_CASES; t++) {
		cin >> N;
		assert(N + 1 < MAXN);
		for (int i = 0; i < 3; i++) {
			cin >> G[i];
		}
		int res = go();
		cout << "Case #" << t << ": ";
		cout << res << '\n';
	}

	return 0;
}
