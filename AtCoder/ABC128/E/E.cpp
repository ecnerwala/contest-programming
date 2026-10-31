#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 1e18;

const int S = 1 << 18;
ll seg[2*S];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, Q; cin >> N >> Q;
	vector<pair<pair<ll, ll>, ll>> blocks(N);
	for (int i = 0; i < N; i++) {
		ll s, t, x; cin >> s >> t >> x;
		s -= x, t -= x;
		blocks[i] = {{s,t}, x};
	}
	vector<ll> D(Q);
	for (int q = 0; q < Q; q++) {
		cin >> D[q];
	}

	for (int i = 0; i < 2 * S; i++) {
		seg[i] = INF;
	}

	for (int i = 0; i < N; i++) {
		ll s = blocks[i].first.first, t = blocks[i].first.second, x = blocks[i].second;
		int si = int(lower_bound(D.begin(), D.end(), s) - D.begin());
		int ti = int(lower_bound(D.begin(), D.end(), t) - D.begin());
		for (int a = S+si, b = S+ti; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				seg[a] = min(seg[a], x);
				a++;
			}
			if (b & 1) {
				--b;
				seg[b] = min(seg[b], x);
			}
		}
	}
	for (int i = 1; i < S; i++) {
		seg[2*i+0] = min(seg[2*i+0], seg[i]);
		seg[2*i+1] = min(seg[2*i+1], seg[i]);
	}
	for (int i = 0; i < Q; i++) {
		if (seg[S+i] == INF) cout << -1 << '\n';
		else cout << seg[S+i] << '\n';
	}
	return 0;
}
