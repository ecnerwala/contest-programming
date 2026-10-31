#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<pair<int, int>> A(N);
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		int64_t t, d; cin >> t >> d;
		ans = max(ans, t);
		ans += d;
	}
	cout << ans << '\n';

	return 0;
}
