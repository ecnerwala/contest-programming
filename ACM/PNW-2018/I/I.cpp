#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

using seg = pair<ll, ll>; // ax + b

const ll MAXN = 3e5;
const ll MAXK = 110;
ll N, K;

ll P[MAXN];

const ll MAXV = MAXN;
ll V;
ll inds[MAXV];
ll dp[MAXV];

ll pref[MAXN];
ll suff[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	V = 0;
	for (ll i = 0; i < N; i++) {
		cin >> P[i];
		if (P[i] == 0) {
			inds[V++] = i;
		}
	}
	dp[0] = 0;
	for (ll i = 1; i <= V; i++) {
		dp[i] = -INF;
	}

	ll tot = 0;
	for (ll k = K; k > 0; k--) {
		pref[0] = 0;
		suff[N] = 0;
		for (ll i = 0; i < N; i++) {
			pref[i+1] = pref[i];
			if (P[i] == 0) continue;
			if (P[i] > k) {
				pref[i+1]++;
			} else if (P[i] == k) {
				tot += pref[i];
			}
		}
		for (ll i = N-1; i >= 0; i--) {
			suff[i] = suff[i+1];
			if (P[i] == 0) continue;
			if (P[i] < k) {
				suff[i]++;
			}
		}

		deque<seg> dq;
		ll prefKnown = 0;
		for (ll i = 0; i <= V; i++) {
			// insert dp[i]
			if (dp[i] != -INF) {
				seg nxt(i, dp[i] - prefKnown - i * i);
				while (dq.size() >= 2) {
					seg cur = dq.back(); dq.pop_back();
					seg prv = dq.back();
					if (
							ll(prv.second - cur.second) * ll(nxt.first - cur.first)
							< ll(cur.second - nxt.second) * ll(cur.first - prv.first)
						) {
						dq.push_back(cur);
						break;
					} else {
						// leave it popped
					}
				}
				dq.push_back(nxt);
			}

			// query dp[i]
			while (dq.size() >= 2) {
				seg cur = dq.front(); dq.pop_front();
				if (cur.first * i + cur.second < dq.front().first * i + dq.front().second) {
					// leave it popped
				} else {
					dq.push_front(cur);
					break;
				}
			}

			assert(!dq.empty());
			ll ndp = prefKnown + dq.front().first * i + dq.front().second;
			assert(ndp >= dp[i]);
			dp[i] = ndp;

			if (i < V) {
				prefKnown += pref[inds[i]] + suff[inds[i]+1];
			}
		}
	}

	cout << dp[V] + tot << '\n';

	return 0;
}
