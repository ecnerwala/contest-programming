#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N; ll C;
ll X[MAXN];
ll V[MAXN];

ll cost[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> C;
	for (int i = 0; i < N; i++) {
		cin >> X[i] >> V[i];
	}

	ll ans = 0;
	X[N] = C;
	for (int z = 0; z < 2; z++) {
		cost[N] = 0;
		for (int i = N-1; i >= 0; i--) {
			cost[i] = cost[i+1] - 2 * (X[i+1]-X[i]) + V[i];
		}

		for (int i = N-1; i >= 0; i--) {
			cost[i] = max(cost[i], cost[i+1]);
		}

		ll sum = 0;
		for (int i = 0; i < N; i++) {
			sum += V[i];
			ans = max(ans, sum - X[i] + cost[i+1]);
		}

		for (int i = 0; i < N; i++) {
			X[i] = C-X[i];
		}
		reverse(X,X+N);
		reverse(V,V+N);
	}
	cout << ans << '\n';

	return 0;
}
