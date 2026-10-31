#include<bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 1600;
int N;
char A[MAXN][MAXN];

int par[MAXN];
int val[MAXN];
int getPar(int a) {
	return par[a] == -1 ? a : (par[a] = getPar(par[a]));
}
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	val[a] ^= val[b];
	par[b] = a;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 1; i < N; i++) {
		cin >> (A[i]+1);
	}

	memset(par, -1, sizeof(par));

	int ans = 0;

	for (int z = 0; z < 2; z++) {
		int numRect = (N - z) / 2 + 1;
		int numEdge = 0;
		int numBad = 0;
		for (int i = 1; i < N; i++) {
			for (int j = i+z; j < N && i + j <= N; j += 2) {
				int u = j - i;
				int v = i + j;
				bool canOn = true;
				bool canOff = true;
				for (char c : {A[i][j], A[j][i], A[N-i][N-j], A[N-j][N-i]}) {
					if (c == 'x') canOn = false;
					else if (c == 'o') canOff = false;
				}
				if (canOn && canOff) {
					// it's an edge
					numEdge ++;
					numBad += merge(u, v);
				} else if (canOn) {
					val[getPar(u)] ^= 1;
					val[getPar(v)] ^= 1;
				} else if (canOff) {
					// nothing
				} else {
					cout << 0 << '\n';
					exit(0);
				}
			}
		}
		if (numRect % 2 == 0) {
			ans += numEdge;
		} else {
			ans += numEdge - numBad;
			for (int u = z; u <= N; u += 2) {
				if (par[u] != -1) continue;
				if (val[u]) {
					cout << 0 << '\n';
					exit(0);
				}
			}
		}
	}
	int realAns = 1;
	while (ans--) { realAns <<= 1; if (realAns >= MOD) realAns -= MOD; }
	cout << realAns << '\n';

	return 0;
}
