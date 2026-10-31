#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<pair<ll, ll>> P(N);
	for (int i = 0; i < N; i++) {
		cin >> P[i].first >> P[i].second;
	}
	sort(P.begin(), P.end());
	if (N % 2 == 1) {
		auto m = P[N/2];
		cout << m.first - ll(1e17) << ' ' << m.second - 1 << '\n';
		cout << m.first + ll(1e17) << ' ' << m.second + 1 << '\n';
	} else {
		auto l = P[N/2-1];
		auto r = P[N/2];

		ll mx = l.first + r.first;
		ll my = l.second + r.second;
		ll dx = 2e17;
		if ((mx & 1) != (dx & 1)) {
			dx++;
		}
		ll dy = 1;
		if ((my & 1) != (dy & 1)) {
			dy++;
		}
		cout << (mx - dx) / 2 << ' ' << (my - dy) / 2 << '\n';
		cout << (mx + dx) / 2 << ' ' << (my + dy) / 2 << '\n';
	}

	return 0;
}
