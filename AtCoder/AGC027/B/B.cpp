#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using num = __int128_t;

const int MAXN = 3e5;
int N;
ll V;
ll X[MAXN];
num pref[MAXN];

int main() {
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	cin >> N >> V;
	for (int i = 0; i < N; i++) {
		cin >> X[i];
	}

	reverse(X, X + N);

	pref[0] = 0;
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i] + X[i];
	}

	num ans = 8e18;
	for (int k = 1; k <= N; k++) {
		num v = k * V + N * V;
		for (int i = 0; i < N; i += k) {
			v += (pref[min(i + k, N)] - pref[i]) * max(5, 2 * (i / k) + 3);
		}
		ans = min(ans, v);
	}
	cout << ll(ans) << '\n';

	return 0;
}
