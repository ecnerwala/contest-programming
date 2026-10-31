#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
int P[MAXN];
int invP[MAXN];
const int S = 1 << 18;
pair<int, int> seg[2][2 * S];
pair<int, int> query(int z, int l, int r) {
	assert(l <= r);
	pair<int, int> ans(1e9,1e9);
	for (int a = S + l, b = S + r + 1; a < b; a /= 2, b /= 2 ){
		if (a & 1) {
			ans = min(ans, seg[z][a]);
			a++;
		}
		if (b & 1) {
			--b;
			ans = min(ans, seg[z][b]);
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) cin >> P[i], invP[P[i]] = i;
	for (int z = 0; z < 2; z++) {
		for (int i = S; i < 2 * S; i++) {
			seg[z][i] = {1e9, 1e9};
		}
	}
	for (int i = 0; i < N; i++) {
		seg[i&1][S+i] = {P[i], i};
	}
	for (int z = 0; z < 2; z++) {
		for (int i = S-1; i > 0; i--) {
			seg[z][i] = min(seg[z][2*i], seg[z][2*i+1]);
		}
	}

	using pii = pair<int, int>;
	using evt = pair<pii, pii>;
	priority_queue<evt, vector<evt>, greater<evt>> pq;
	auto solve = [&](int L, int R) {
		if (L > R) return;
		assert((R - L) & 1);
		int i = query(L&1, L, R-1).second;
		int j = query(!(i&1), i+1, R).second;
		pq.push({{P[i], P[j]}, {L, R}});
	};

	solve(0, N-1);
	while (!pq.empty()) {
		auto e = pq.top(); pq.pop();
		int l = e.second.first, r = e.second.second;
		int i = invP[e.first.first], j = invP[e.first.second];
		cout << P[i] << ' ' << P[j] << ' ';
		solve(l, i-1);
		solve(i+1, j-1);
		solve(j+1, r);
	}
	cout << '\n';

	return 0;
}
