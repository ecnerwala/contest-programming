#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	vector<pair<int, int>> P(N);
	vector<pair<int, int>> seg(2*N);
	for (int i = 0; i < N; i++) {
		cin >> P[i].first, P[i].second = i;
		seg[N+i].first = P[i].first, seg[N+i].second = 1;
	}
	auto merge = [&](const pair<int, int>& x, const pair<int, int>& y) -> pair<int, int> {
		if (x.first == y.first) return {x.first, x.second + y.second};
		else if (x.second >= y.second) return {x.first, x.second - y.second};
		else return {y.first, y.second - x.second};
	};
	for (int a = N-1; a; a--) {
		seg[a] = merge(seg[2*a], seg[2*a+1]);
	}

	std::sort(P.begin(), P.end());

	while (Q--) {
		int l, r; cin >> l >> r; l--;
		pair<int, int> v(0,0);
		for (int a = l+N, b = r+N; a < b; a /= 2, b /= 2) {
			if (a & 1) v = merge(v, seg[a++]);
			if (b & 1) v = merge(v, seg[--b]);
		}
		int cnt = int(lower_bound(P.begin(), P.end(), pair<int, int>(v.first, r)) - lower_bound(P.begin(), P.end(), pair<int, int>(v.first, l)));
		if (cnt * 2 > (r - l)) {
			cout << v.first << '\n';
		} else {
			cout << 0 << '\n';
		}
	}

	return 0;
}
