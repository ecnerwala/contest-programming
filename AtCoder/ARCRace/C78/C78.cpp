#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	long long tot = 0;
	vector<long long> cnd;
	for (int i = 0; i < N; i++) {
		if (i) cnd.push_back(tot);
		int a; cin >> a; tot += a;
	}
	long long ans = 1e18;
	for (long long v : cnd) {
		ans = min(ans, abs(2 * v - tot));
	}
	cout << ans << '\n';

	return 0;
}
