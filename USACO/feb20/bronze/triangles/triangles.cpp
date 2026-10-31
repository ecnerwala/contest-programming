#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	if (fopen("triangles.in", "r")) {
		freopen("triangles.in", "r", stdin);
		freopen("triangles.out", "w", stdout);
	}

	int N; cin >> N;
	vector<pair<int, int>> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].first >> pts[i].second;
	}
	ll ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				ll x1 = pts[j].first - pts[i].first;
				ll y1 = pts[j].second - pts[i].second;
				ll x2 = pts[k].first - pts[i].first;
				ll y2 = pts[k].second - pts[i].second;

				if ((x1 == 0 || x2 == 0) && (y1 == 0 || y2 == 0)) {
					ll area = abs(x1 * y2 - x2 * y1);
					ans = max(ans, area);
				}
			}
		}
	}

	cout << ans << '\n';
	return 0;
}
