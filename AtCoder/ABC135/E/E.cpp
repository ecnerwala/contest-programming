#include<bits/stdc++.h>
using namespace std;

using ll = long long;

ll signum(ll a) {
	return (a > 0) - (a < 0);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	ll X, Y, K; cin >> X >> Y >> K;
	ll v = (abs(X) + abs(Y) + K-1) / K;
	assert(v * K >= abs(X) + abs(Y));
	if (K * v % 2 != (abs(X) + abs(Y)) % 2) v++;

	if (K * v % 2 != (abs(X) + abs(Y)) % 2) {
		cout << -1 << '\n';
		exit(0);
	}

	ll dx = abs(X);
	ll dy = K * v - dx;

	assert(dy >= abs(Y));
	assert(dy % 2 == abs(Y) % 2);
	assert(dy - abs(Y) < 2 * K);

	vector<pair<ll, ll>> trans;
	vector<pair<ll, ll>> ans;


	cout << ans.size() << '\n';
	for (auto it : ans) { cout << it.first << ' ' << it.second << '\n'; }

	return 0;
}
