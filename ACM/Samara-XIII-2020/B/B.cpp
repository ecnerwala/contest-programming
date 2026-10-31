#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, T; cin >> N >> T;
	vector<int> X(N);
	for (int i = 0; i < N; i++) {
		cin >> X[i];
	}
	sort(X.begin(), X.end());
	int ans = 0;
	for (int z = 0; z < 2; z++) {
		int l = 0, r = 0;
		for (; l < N; l++) {
			r = max(r, l);
			while (r < N && T - 2 * max(0, -X[l]) >= max(0, X[r])) {
				ans = max(ans, r - l + 1);
				r++;
			}
		}
		reverse(X.begin(), X.end()); for (auto& a : X) { a = -a; }
	}
	cout << ans << '\n';

	return 0;
}
