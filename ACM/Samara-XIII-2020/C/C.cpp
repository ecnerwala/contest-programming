#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	i64 K; cin >> K;

	vector<pair<int, int>> pts(N);
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		pts[i] = {x+y,x-y};
	}
	sort(pts.begin(), pts.end());

	vector<int> ys;
	for (int i = 0; i < N; i++) {
		ys.push_back(pts[i].second);
	}
	sort(ys.begin(), ys.end());
	ys.resize(unique(ys.begin(), ys.end()) - ys.begin());

	int S = 1;
	while (S < int(ys.size())) S *= 2;
	vector<int> seg(2*S);
	auto reset = [&]() { fill(seg.begin(), seg.end(), 0); };
	auto incr = [&](int i, int v) {
		for (int a = S+i; a; a /= 2) seg[a] += v;
	};
	auto query = [&](int l, int r) {
		int res = 0;
		for (int a = S+l, b = S+r; a < b; a /= 2, b /= 2) {
			if (a & 1) res += seg[a++];
			if (b & 1) res += seg[--b];
		}
		return res;
	};

	vector<int> ptInds(N);
	for (int i = 0; i < N; i++) {
		ptInds[i] = int(lower_bound(ys.begin(), ys.end(), pts[i].second) - ys.begin());
	}

	auto cntLeq = [&](int len) -> i64 {
		i64 ans = 0;
		reset();
		for (int i = 0, j = 0, k = 0; i < N; i++) {
			while (j < N && pts[j].first <= pts[i].first + len) {
				incr(ptInds[j], +1);
				j++;
			}
			while (k < j && pts[k].first < pts[i].first - len) {
				incr(ptInds[k], -1);
				k++;
			}
			ans += query(
				int(lower_bound(ys.begin(), ys.end(), pts[i].second - len) - ys.begin()),
				int(upper_bound(ys.begin(), ys.end(), pts[i].second + len) - ys.begin())
			);
		}
		return (ans - N) / 2;
	};
	int mi = -1, ma = int(5e8);
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (cntLeq(md) >= K) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';

	return 0;
}
