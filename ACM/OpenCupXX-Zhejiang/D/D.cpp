#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXS = 1 << 16;
vector<pair<pair<int, int>, ll>> ops[2*MAXS];
vector<int> ys[2*MAXS];
vector<ll> seg[2*MAXS];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M1, M2;
	cin >> N >> M1 >> M2;

	int S = 1; while (S < N) S *= 2;

	for (int z = 0; z < M1; z++) {
		int x1, y1, x2, y2; ll w; cin >> x1 >> y1 >> x2 >> y2 >> w;
		x1--, y1--;
		for (int a = S+x1, b = S+x2; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				ops[a++].push_back({{y1, y2}, w});
			}
			if (b & 1) {
				ops[--b].push_back({{y1, y2}, w});
			}
		}
	}

	for (int s = 1; s < 2 * S; s++) {
		ys[s].reserve(ops[s].size()*2);
		for (auto it : ops[s]) {
			ys[s].push_back(it.first.first);
			ys[s].push_back(it.first.second);
		}
		sort(ys[s].begin(), ys[s].end());
		ys[s].resize(unique(ys[s].begin(), ys[s].end()) - ys[s].begin());
		int curS = 1; while (curS < int(ys[s].size())) curS *= 2;

		seg[s].resize(curS*2);

		for (auto it : ops[s]) {
			int y1 = it.first.first;
			int y2 = it.first.second;
			ll w = it.second;

			cerr << s << ' ' << y1 << ' ' << y2 << ' ' << w << '\n';

			y1 = max(0, int(upper_bound(ys[s].begin(), ys[s].end(), y1) - ys[s].begin()) - 1);
			y2 = int(lower_bound(ys[s].begin(), ys[s].end(), y2) - ys[s].begin());


			for (int a = curS + y1, b = curS + y2; a < b; a /= 2, b /= 2) {
				if (a & 1) {
					seg[s][a++] += w;
				}
				if (b & 1) {
					seg[s][--b] += w;
				}
			}
		}

		for (int i = 1; i < curS; i++) {
			seg[s][2*i+0] += seg[s][i];
			seg[s][2*i+1] += seg[s][i];
		}
		for (int i = curS - 1; i > 0; i--) {
			seg[s][i] = max(seg[s][2*i+0], seg[s][2*i+1]);
		}
	}

	for (int z = 0; z < M2; z++) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		x1--, y1--;

		ll ans = 0;
		auto queryY = [&](int s) {
			int curS = int(seg[s].size()) / 2;
			int compY1 = max(0, int(upper_bound(ys[s].begin(), ys[s].end(), y1) - ys[s].begin()) - 1);
			int compY2 = int(lower_bound(ys[s].begin(), ys[s].end(), y2) - ys[s].begin());
			for (int a = curS + compY1, b = curS + compY2; a < b; a /= 2, b /= 2) {
				if (a & 1) ans = max(ans, seg[s][a++]);
				if (b & 1) ans = max(ans, seg[s][--b]);
			}
		};
		for (int a = S+x1, b = S+x2; a < b; a /= 2, b /= 2) {
			if (a & 1) queryY(a++);
			if (b & 1) queryY(--b);
		}
		cout << ans << '\n';
	}

	return 0;
}
