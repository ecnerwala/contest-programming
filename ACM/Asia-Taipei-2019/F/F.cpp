#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e6;
int N;

ll K;
pair<ll, ll> A[MAXN];

ll gcd(ll a, ll b) {
	while (a) {
		b %= a;
		swap(a,b);
	}
	return b;
}

const int MAXP = 11;
int P;
ll dp[1<<MAXP][MAXP+1];
ll pdp[1<<MAXP][MAXP+1];
ll maxDelta[1<<MAXP];

void computeMaxDelta() {
	for (int m = 0; m < (1 << P); m++) {
		maxDelta[m] = 0;
	}
	for (int m = 0; m < (1<<P); m++) {
		for (int d = m; d; d = (d-1)&m) {
			for (int cnt = 0; cnt < P; cnt++) {
				maxDelta[d] = max(maxDelta[d], dp[m][cnt+1] - dp[m^d][cnt]);
			}
		}
	}
}

ll tmp[1<<MAXP];
vector<int> goodMasks;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> A[i].second;
	}
	for (int i = 0; i < N; i++) {
		cin >> A[i].first;
	}

	sort(A, A + N);

	ll G = 0;
	for (int i = 0; i < N; i++) {
		G = gcd(G, A[i].second);
	}

	vector<ll> pfacs;
	for (ll p = 2; G > 1; p++) {
		if (p * p > G) p = G;
		if (G % p == 0) {
			pfacs.push_back(p);
			while (G % p == 0) {
				G /= p;
			}
		}
	}

	P = int(pfacs.size());

	const ll INF = 1e17;

	for (int m = 0; m < (1<<P); m++) {
		for (int cnt = 0; cnt <= P; cnt++) {
			dp[m][cnt] = m ? INF : 0;
		}
	}

	computeMaxDelta();

	unordered_set<ll> bads;
	vector<ll> qs(P);
	for (int i = 0; i < N; i++) {
		ll a_orig = A[i].second;
		ll a_new = 1;
		for (int z = 0; z < P; z++) {
			ll p = pfacs[z];
			ll q = 1;
			while (a_orig % p == 0) {
				a_orig /= p;
				q *= p;
			}
			qs[z] = q;
			a_new *= q;
		}
		if (bads.count(a_new)) continue;

		tmp[0] = 1;
		goodMasks.clear();
		for (int z = 0; z < P; z++) {
			for (int m = 0; m < (1<<z); m++) {
				tmp[m|(1<<z)] = tmp[m] * qs[z];
				if (tmp[m|(1<<z)] <= K && A[i].first < maxDelta[m|(1<<z)]) {
					goodMasks.push_back(m|(1<<z));
				}
			}
		}

		if (goodMasks.empty()) {
			bads.insert(a_new);
			continue;
		}

		for (int m = 0; m < (1<<P); m++) {
			for (int cnt = 0; cnt <= P; cnt++) {
				pdp[m][cnt] = dp[m][cnt];
			}
		}
		for (int cnt = P-1; cnt >= 0; cnt--) {
			for (int d : goodMasks) {
				int rem = (1<<P) - 1 - d;
				for (int m = rem; true; m = (m-1)&rem) {
					//cerr << (m|d) << ' ' << pdp[m][cnt] << ' ' << A[i].first << '\n';
					dp[m|d][cnt+1] = min(dp[m|d][cnt+1], pdp[m][cnt] + A[i].first);
					if (m == 0) break;
				}
			}
		}

		computeMaxDelta();
	}

	ll ans = INF;
	for (int cnt = 0; cnt <= P; cnt++) {
		//cerr << cnt << ' ' << dp[(1<<P)-1][cnt] << '\n';
		if (dp[(1<<P)-1][cnt] == INF) continue;
		ans = min(ans, dp[(1<<P)-1][cnt] * cnt);
	}
	if (ans == INF) {
		cout << -1 << '\n';
	} else {
		cout << ans << '\n';
	}
	return 0;
}
