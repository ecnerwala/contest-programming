#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i]; A[i]--;
	}

	vector<int> cnt(N);
	long long ans = 0;
	for (int i = 0; i < N; i++) {
		ans += cnt[A[i]]++;
	}
	for (int i = 0; i < N; i++) {
		cout << ans - (cnt[A[i]]-1) << '\n';
	}

	return 0;
}
