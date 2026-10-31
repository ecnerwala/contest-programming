#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N, P, K; cin >> N >> P >> K;
	map<int, int> mp;
	long long ans = 0;
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		int v = 1ll * a * a % P * a % P;
		v -= K;
		if (v < 0) v += P;
		v = 1ll * v * a % P;
		ans += mp[v]++;
	}
	cout << ans << '\n';

	return 0;
}
