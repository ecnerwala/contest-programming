#include<bits/stdc++.h>
using namespace std;

using i64 = int64_t;
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K; cin >> N >> K;
	vector<i64> A(N);
	for (int i = 0; i < N; i++) cin >> A[i];
	sort(A.begin(), A.end());

	vector<i64> pref(N+1);
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i] + A[i];
	}
	i64 ans = 1e18;
	for (int i = 0; i < N; i++) {
		if (i && A[i] == A[i-1]) continue;
		int j = i; while (j < N && A[j] == A[i]) j++;
		int cntDone = j-i;
		if (cntDone >= K) {
			ans = 0;
			break;
		}
		i64 vHi = (pref[N] - pref[j]) - (A[i]+1) * (N-j);
		i64 vLo = (A[i]-1) * i - pref[i];
		if (N-j >= K - cntDone) ans = min(ans, vHi + (K - cntDone));
		if (i >= K - cntDone) ans = min(ans, vLo + (K - cntDone));
		ans = min(ans, vLo + vHi + (K - cntDone));
	}

	for (int i = 0; i+1 < N; i++) {
		if (A[i+1] - A[i] < 2) continue;
		i64 vLo = (A[i]) * i - pref[i];
		i64 vHi = (pref[N] - pref[i+1]) - (A[i+1]) * (N-(i+1));
		ans = min(ans, vLo + vHi + min(i+1, N-(i+1)) * (A[i+1] - A[i] - 2) + K);
	}

	cout << ans << '\n';
	return 0;
}
