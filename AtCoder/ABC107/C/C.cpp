#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N, K;
int X[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) cin >> X[i];
	int res = 1e9;
	for (int i = 0; i + K <= N; i++) {
		int l = X[i], r = X[i + K - 1];
		int maxpos = max(0, max(l, r));
		int maxneg = max(0, max(-l, -r));
		res = min(res, maxpos + maxneg + min(maxpos, maxneg));
	}
	cout << res << '\n';

	return 0;
}
