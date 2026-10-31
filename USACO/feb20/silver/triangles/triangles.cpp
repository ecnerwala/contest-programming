#include<bits/stdc++.h>
using namespace std;

const int MOD = int(1e9)+7;

int main() {
	if (fopen("triangles.in", "r")) {
		freopen("triangles.in", "r", stdin);
		freopen("triangles.out", "w", stdout);
	}

	using ll = long long;

	int N; cin >> N;
	vector<pair<int, int>> pts(N);
	unordered_map<int, vector<pair<int, ll>>> xtoy;
	unordered_map<int, vector<pair<int, ll>>> ytox;

	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		pts[i] = {x,y};
		xtoy[x].emplace_back(y, 0);
		ytox[y].emplace_back(x, 0);
	}

	auto solveLine = [&](vector<pair<int, ll>>& v) {
		sort(v.begin(), v.end());
		int L = int(v.size());
		ll prefSum = 0;
		for (int i = 0; i < L; i++) {
			v[i].second += ll(v[i].first) * i - prefSum;
			v[i].second %= MOD;
			prefSum += v[i].first;
			prefSum %= MOD;
		}
		ll suffSum = 0;
		for (int i = L-1; i >= 0; i--) {
			suffSum += v[i].first;
			suffSum %= MOD;
			v[i].second += suffSum- ll(v[i].first) * (L-i);
			v[i].second %= MOD;
		}
	};

	auto lookupLine = [&](vector<pair<int, ll>>& v, int c) {
		return lower_bound(v.begin(), v.end(), pair<int, ll>(c, -1))->second;
	};

	for (auto& it : xtoy) solveLine(it.second);
	for (auto& it : ytox) solveLine(it.second);

	ll ans = 0;
	for (auto p : pts) {
		int x = p.first, y = p.second;
		ans += lookupLine(xtoy[x], y) * lookupLine(ytox[y], x);
		ans %= MOD;
	}
	ans %= MOD;
	if (ans < 0) ans += MOD;
	cout << ans << '\n';

	return 0;
}
