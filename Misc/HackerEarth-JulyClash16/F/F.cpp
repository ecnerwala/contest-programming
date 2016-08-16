#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

typedef long long ll;

const int MAXN = 500;

const int MAXK = MAXN * MAXN;

int N, K;
ll M[MAXN][MAXN];
ll S[MAXN];

ll eval(const vector<pii> &a) {
	int d[MAXN][MAXN];
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			d[i][j] = N + 1;
		}
		d[i][i] = 0;
	}
	for(pii it : a) {
		d[it.first][it.second] = 1;
	}
	for(int k = 0; k < N; k++) {
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
			}
		}
	}
	ll res = 0;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			res += M[i][j] * d[i][j];
		}
	}
	return res;
}

void print(const vector<pii> &a) {
	for(pii it : a) {
		cout << it.first + 1 << ' ' << it.second + 1 << '\n';
	}
}

struct ds {
	int par[MAXN];
	inline int get_par(int a) { return (par[a] == -1) ? a : (par[a] = get_par(par[a])); }
	inline bool merge(int a, int b) {
		a = get_par(a), b = get_par(b);
		if(a == b) return false;
		par[a] = b;
		return true;
	}
	inline bool merge(pii a) { return merge(a.first, a.second); }
	ds() {
		memset(par, -1, sizeof(par));
	}
};

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			cin >> M[i][j];
		}
	}
	vector<pair<ll, pii>> edges;
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			edges.emplace_back(M[i][j] + M[j][i], pii(i, j));
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			S[i] += M[i][j] + M[j][i];
		}
	}
	int best = 0;
	for(int i = 1; i < N; i++) {
		if(S[i] > S[best]) best = i;
	}

	sort(edges.begin(), edges.end());
	reverse(edges.begin(), edges.end());
	vector<pii> res;
	ds cc;
	for(int i = 0; i < N; i++) {
		if(i == best) continue;
		pii cur(i, best);
		cc.merge(cur);
		res.push_back(cur);
	}
	int num_extra = K - (N - 1);
	for(auto it : edges) {
		if(it.second.first == best || it.second.second == best) continue;
		if(cc.merge(it.second)) {
			res.push_back(it.second);
		} else if(num_extra) {
			res.push_back(it.second);
			num_extra --;
		}
	}
	print(res);
	return 0;
}
