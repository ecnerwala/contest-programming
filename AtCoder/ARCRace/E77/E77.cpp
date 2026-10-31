#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int S = 1 << 17;
// first is constant term
pair<ll, ll> seg[2 * S];
//inclusive
void incr(int l, int r, int v1, int v2) {
	for (int a=S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			seg[a].first += v1;
			seg[a].second += v2;
			++a;
		}
		if (b & 1) {
			--b;
			seg[b].first += v1;
			seg[b].second += v2;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M;
	cin >> N >> M;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i]; A[i] --;
	}
	for (int i = 1; i < N; i++) {
		int prv = A[i-1], nxt = A[i];
		assert(prv != nxt);
		seg[1].first += (nxt + M - prv) % M;
		// ans -= a - (prv+1) from prv+1 to nxt
		if (prv < nxt) {
			incr(prv+1, nxt, prv+1, -1);
		} else {
			incr(prv+1, M-1, prv+1, -1);
			// ans -= (a+M) - (prv+1)
			incr(0, nxt, prv+1 - M, -1);
		}
	}

	for (int i = 1; i < S; i++) {
		seg[2*i].first += seg[i].first;
		seg[2*i].second += seg[i].second;
		seg[2*i+1].first += seg[i].first;
		seg[2*i+1].second += seg[i].second;
	}
	
	ll ans = 1e18;
	for (int i = 0; i < M; i++) {
		ans = min(ans, seg[S+i].first + seg[S+i].second * i);
	}
	cout << ans << '\n';

	return 0;
}
