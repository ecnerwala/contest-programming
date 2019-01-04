#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;

int N;
ll L;
ll X[MAXN];
ll pref[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> L >> N;
	for (int i = 0; i < N; i++) {
		cin >> X[i];
	}
	ll ans = 0;
	for (int z = 0; z < 2; z++) {
		pref[0] = 0;
		for (int i = 0; i < N; i++) {
			pref[i+1] = pref[i] + X[i];
		}
		for (int i = 0; i < N; i++) {
			// we walk forward to i, then start to alternate
			ll cnd = 0;
			if ((N - i) % 2 == 0) {
				// we take an even number of steps
				// last step is backwards
				int mid = (N+i) / 2;
				cnd += ((pref[mid] - pref[i]) + (L * (mid - i))) * 2 - (pref[N] - pref[mid]) * 2 + X[mid] - L;
			} else {
				// last step if forwards
				int mid = (N+i+1) / 2;
				// (mid - i) * 2 - (N - mid) * 2 - 2
				cnd += ((pref[mid] - pref[i]) + (L * (mid - i))) * 2 - (pref[N] - pref[mid]) * 2 - (X[mid-1]+L) - L;
			}

			ans = max(ans, cnd);
		}

		for (int i = 0; i < N; i++) {
			X[i] = L - X[i];
		}
		reverse(X, X + N);
	}
	cout << ans << '\n';

	return 0;
}
