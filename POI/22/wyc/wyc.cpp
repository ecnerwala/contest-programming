#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 4e18;

const int MAXN = 45;
int N;
const int MAXC = 3;
const int C = 3;

const int MAXV = MAXN * MAXC + 1;
int V;

struct mat {
	ll v[MAXV][MAXV];
	mat() {}
	explicit mat (ll a) {
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				v[i][j] = 0;
			}
		}
		for (int i = 0; i < V; i++) {
			v[i][i] = a;
		}
	}
	friend mat operator * (const mat &a, const mat &b) {
		mat c;
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				c.v[i][j] = 0;
				for (int k = 0; k < V; k++) {
					ll x = a.v[i][k], y = b.v[k][j];
					assert(x >= 0 && y >= 0);
					if (x == 0 || y == 0) continue;
					assert(x > 0 && y > 0);
					if (INF / x < y) {
						c.v[i][j] = INF;
						break;
					}
					c.v[i][j] += x * y;
					if (c.v[i][j] >= INF) {
						c.v[i][j] = INF;
						break;
					}
				}
			}
		}
		return c;
	}
};


mat trans[70];
ll eval(const mat &m) {
	ll res = 0;
	for (int i = 0; i < N; i++) {
		res += m.v[i][V-1];
		if (res >= INF) return INF;
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int M;
	ll K;
	cin >> N >> M >> K;
	V = N * C + 1;

	// We include the paths of length 0
	K += N;

	trans[0] = mat(0);
	trans[0].v[V-1][V-1] ++;
	for (int i = 0; i < N; i++) {
		trans[0].v[i][V-1] ++;
		for (int c = 1; c < C; c++) {
			trans[0].v[c*N+i][(c-1)*N+i]++;
		}
	}
	for (int i = 0; i < M; i++) {
		int u, v, c; cin >> u >> v >> c; u--, v--;
		assert(1 <= c && c <= C);
		trans[0].v[u][(c-1)*N + v] ++;
	}
	int b = 0;
	while (b <= 63 && eval(trans[b]) < K) {
		b++;
		trans[b] = trans[b-1] * trans[b-1];
	}
	// all 2^64 routes don't work
	if (b == 64) {
		cout << -1 << '\n';
		return 0;
	}
	ll r = 0;
	mat m(1);
	assert(eval(m) == 0);
	assert(eval(m) < K);
	for (b--; b >= 0; b--) {
		if (eval(m * trans[b]) < K) {
			m = m * trans[b];
			r |= (1ll << b);
		}
	}
	assert(eval(m) < K);
	cout << r << '\n';
	return 0;
}
