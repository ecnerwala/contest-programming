#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e6;
int N;
ll A[MAXN];
ll prefA2[MAXN];
ll suffA[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) cin >> A[i];
	prefA2[0] = 0;
	for (int i = 0; i < N; i++) {
		prefA2[i+1] = prefA2[i] + A[i] * A[i];
	}
	suffA[N] = 0;
	for (int i = N-1; i >= 0; i--) {
		suffA[i] = suffA[i+1] + A[i];
	}
	ll ans = 0;
	for (int i = 0; i <= N; i++) {
		ans = max(ans, prefA2[i] * suffA[i]);
	}
	cout << ans << '\n';

	return 0;
}
