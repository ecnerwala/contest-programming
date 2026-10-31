#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> X(N);
	for (auto& x : X) cin >> x;

	int ans = int(1e9);
	for (int p = 1; p <= 100; p++) {
		int cnd = 0;
		for (auto x : X) {
			cnd += (x-p) * (x-p);
		}
		ans = min(ans, cnd);
	}
	cout << ans << '\n';

	return 0;
}
