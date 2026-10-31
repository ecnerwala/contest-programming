#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	int ans = -1;
	for (int i = 1; i+1 < N; i++) {
		int j = 0;
		while (i-j-1 >= 0 && i+j+1 < N && A[i-j] > A[i-j-1] && A[i+j] > A[i+j+1] && A[i-j-1] == A[i+j+1]) {
			j++;
		}
		if (j > 0) {
			ans = max(ans, 2*j+1);
		}
	}
	cout << ans << '\n';

	return 0;
}
