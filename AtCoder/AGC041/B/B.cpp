#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N, V, P; ll M;

ll A[MAXN];
ll pref[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> V >> P;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	sort(A, A + N);
	reverse(A, A + N);
	pref[0] = 0;
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i] + A[i];
	}

	int ans = P; // the first P are guaranteed
	for (int i = P; i < N; i++) {
		ll nExtra = max(V-P - (N-1-i), 0);
		if ((A[i] + M >= A[P-1]) && ((A[i] + M) * (i - (P-1)) >= (pref[i] - pref[P-1]) + M * nExtra)) {
			ans++;
		}
	}
	cout << ans << '\n';

	return 0;
}
