#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
int A[MAXN];
int D[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 1; i <= N; i++) {
		cin >> A[i];
	}
	for (int i = 0; i <= N; i++) {
		D[i] = A[i+1] - A[i];
	}
	N++;

	using ll = long long;
	int ans = 0;
	// min amount of non-source-flow for ans+cost
	ll v0 = 0, v1 = 0; // v2 is 0
	for (int i = 0; i < N; i++) {
		// tot flow is A[i]
		int totFlow = A[i];
		if (D[i] == 0) {
			continue;
		} else if (D[i] > 0) {
			ll nv0 = v0 + D[i];
			ll nv1 = min(v0, v1 + D[i]);
			v0 = nv0;
			v1 = nv1;
		} else if (D[i] < 0) {
			// pay nothing to charge D[i], pay 1 to decrease non-source-flow, pay 2 to make it 0
			ll v = -D[i];
			ll nv0 = ll(1e18);
			ll nv1 = max(0ll, v0 - v);
			if (totFlow - v1 >= v) {
				nv1 = min(nv1, v1);
			}
			if (totFlow - v0 >= v) {
				nv0 = v0;
			} else {
				// not feasible to keep
				ans++;
				nv0 = nv1;
				nv1 = 0;
			}
			v0 = nv0;
			v1 = nv1;
		} else assert(false);
	}
	cout << ans << '\n';

	return 0;
}

// +1 +1 -1 +1 -1 -1
// +2 -1 +1 -1 +1 -2
