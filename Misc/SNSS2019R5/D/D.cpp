#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;
const int MAXN = int(1e6) + 20;
vector<int> factors[MAXN];
int pfac[MAXN];

bool inSet[MAXN];
int cnt[MAXN][21];
int v[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	for (int i = 1; i < MAXN; i++) {
		for (int j = i; j < MAXN; j += i) {
			factors[j].push_back(i);
		}
		if (factors[i].size() == 2) {
			for (long long q = i; q < MAXN; q *= i) {
				for (long long j = q; j < MAXN; j += q) {
					pfac[j] ++;
				}
			}
		}
	}

	/*
	int a = 0;
	for (int i = 0; i < MAXN; i++) {
		a = max(a, int(factors[i].size()));
	}
	cerr << a << '\n';
	*/
	for (int i = 0; i < MAXN; i++) {
		v[i] = 21;
	}

	int Q; cin >> Q;
	while (Q--) {
		char t; cin >> t;
		if (t == 'I') {
			int x; cin >> x;
			if (!inSet[x]) {
				inSet[x] = true;
				for (int f : factors[x]) {
					cnt[f][pfac[x]]++;
					v[f] = min(v[f], pfac[x]);
				}
			}
		} else if (t == 'D') {
			int x; cin >> x;
			if (inSet[x]) {
				inSet[x] = false;
				for (int f : factors[x]) {
					--cnt[f][pfac[x]];
					if (!cnt[f][pfac[x]] && pfac[x] == v[f]) {
						while (v[f] < 21 && !cnt[f][v[f]]) v[f]++;
					}
				}
			}
		} else if (t == 'Q') {
			int x; cin >> x;
			int ans = INF;
			for (int f : factors[x]) {
				if (v[f] < 21) {
					ans = min(ans, pfac[x] - pfac[f] + v[f] - pfac[f]);
				}
			}
			if (ans == INF) {
				cout << -1 << '\n';
			} else {
				cout << ans << '\n';
			}
		}
	}
	return 0;
}
