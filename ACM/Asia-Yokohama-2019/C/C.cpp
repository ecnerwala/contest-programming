#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

const int S = 1 << 19;
ll seg[7][2*S];
// half-open
ll query(int z, int l, int r) {
	ll ans = -INF;
	for (int a=S+l, b = S+r; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			ans = max(ans, seg[z][a++]);
		}
		if (b & 1) {
			ans = max(ans, seg[z][--b]);
		}
	}
	return ans;
}
void update(int z, int i, ll v) {
	for (int a = S+i; a; a /= 2) {
		seg[z][a] = max(seg[z][a], v);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	ll N; int M; ll X, Y; cin >> N >> M >> X >> Y;

	vector<pair<pair<ll, ll>, int>> robots;
	vector<ll> coords;
	for (int i = 0; i < M; i++) {
		int c; ll l, r; cin >> c >> l >> r; l--; c--;
		robots.push_back({{l,r},c});
		coords.push_back(r);
	}
	sort(robots.begin(), robots.end());
	sort(coords.begin(), coords.end());
	coords.resize(unique(coords.begin(), coords.end()) - coords.begin());

	for (int z = 0; z < 7; z++) {
		for (int i = 0; i < 2 * S; i++) {
			seg[z][i] = -INF;
		}
	}

	ll ans = 0;
	for (auto it : robots) {
		ll l = it.first.first, r = it.first.second; int c = it.second;
		int li = int(upper_bound(coords.begin(), coords.end(), l) - coords.begin());
		int ri = int(upper_bound(coords.begin(), coords.end(), r) - coords.begin());

		ll v = 0;
		v = max(v, query(6, 0, li));
		v = max(v, query(3+c, li, ri) + X * l);
		v = max(v, query((c+1)%3, li, ri) + (X + X + Y) * l);
		v = max(v, query((c+2)%3, li, ri) + (X + X + Y) * l);
		v += (r - l) * X;

		ans = max(ans, v);

		update(6, ri-1, v);
		update(3+c, ri-1, v - X * r);
		update(c, ri-1, v - (X + X + Y) * r);
	}
	cout << ans << '\n';

	return 0;
}
