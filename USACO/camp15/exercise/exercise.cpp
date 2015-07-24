#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
typedef pair<pii, ll> ppl;
#define A first
#define B second

const ll INF = 1e18;
const ll MOD = 1e9 + 7;
const ll HALF = (MOD + 1) / 2; // 1/2 % MOD

const int MAXN = 2000;
const int MAXM = 2000;

int N;
int M;
ppl edges[MAXM];
ll dist[MAXN][MAXN];

ll d[MAXN];

ll T;

// time [s,t), line mx + v
ll sumval(ll s, ll t, ll m, ll v) {
	if(t <= s) return 0;
	assert(s < t);

	ll a = t - s;
	a %= MOD;
	ll b = s + t - 1;
	b %= MOD;
	b *= m % MOD;
	b %= MOD;
	b += 2 * v;
	b %= MOD;
	a *= b;
	a %= MOD;
	a *= HALF;
	a %= MOD;
	return a;
}

ll gethull_slow() {
	ll res = 0;
	for(ll t = 0; t < T; t++) {
		ll best = INF;
		for(int i = 0; i < N; i++) {
			if(d[i] == INF) continue;
			best = min(best, d[i] + i * t);
		}
		assert(best < INF);
		res += best;
		if(res >= MOD) res -= MOD;
	}
	return res;
}

ll gethull() {
	ll res = 0;
	for(int i = 0; i < N; i++) {
		if(d[i] == INF) continue;
		ll s = 0, t = T;
		for(int j = 0; j < i; j++) {
			if(d[j] == INF) continue;
			// (i * x + d[i]) <= (j * x + d[j])
			// (i - j) * x <= d[j] - d[i]
			// x <= ((d[j] - d[i]) / (i - j))
			// x <= floor((d[j] - d[i]) / (i - j))
			// x < floor((d[j] - d[i]) / (i - j)) + 1
			if(d[j] - d[i] <= 0) {
				// this is always bigger
				t = -1;
				continue;
			}

			assert((i - j) > 0);
			assert((d[j] - d[i]) > 0);
			t = min(t, (d[j] - d[i]) / (i - j) + 1);
		}

		for(int j = i + 1; j < N; j++) {
			if(d[j] == INF) continue;
			// (i * x + d[i]) < (j * x + d[j])
			// d[i] - d[j] < (j - i) * x
			// x > ((d[i] - d[j]) / (j - i))
			// x > floor((d[i] - d[j]) / (j - i))
			// x >= floor((d[i] - d[j]) / (j - i)) + 1

			if(d[i] - d[j] <= 0) {
				// do nothing, this is always smaller
				continue;
			}

			assert((j - i) > 0);
			assert((d[i] - d[j]) > 0);
			s = max(s, (d[i] - d[j]) / (j - i) + 1);
		}

		if(s < t) {
			res += sumval(s, t, i, d[i]);
			res %= MOD;
		}

	}
	return res;
}


int main() {
	cin >> N >> M >> T;
	for(int i = 0; i < M; i++) {
		cin >> edges[i].A.A >> edges[i].A.B >> edges[i].B;
	}
	for(int l = 0; l < N; l++) {
		for(int i = 0; i < N; i++) {
			dist[i][l] = INF;
		}
	}
	dist[0][0] = 0;

	for(int l = 0; l < N; l++) {
		for(int i = 0; i < M; i++) {
			int a = edges[i].A.A, b = edges[i].A.B;
			ll v = edges[i].B;
			dist[b][l + 1] = min(dist[b][l + 1], dist[a][l] + v);
			dist[a][l + 1] = min(dist[a][l + 1], dist[b][l] + v);
		}
	}

	for(int l = 0; l < N; l++) d[l] = dist[N - 1][l];

	ll res = gethull();
	cout << res << '\n';

	return 0;

}
