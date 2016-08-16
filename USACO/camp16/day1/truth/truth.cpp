#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

const int MAXN = 100;
const int MAXM = 100;
const int MAXV = MAXN + MAXM;

const int MAXS = 1e4;

int M, N, S;
int I[MAXS], J[MAXS], Q[MAXS];
ll K[MAXS];

int V;
ll dist[MAXV][MAXV];

void setmin(ll &a, ll b) { if(b < a) a = b; }

void all_sp() {
	for(int k = 0; k < V; k++) {
		for(int i = 0; i < V; i++) {
			for(int j = 0; j < V; j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
}

bool is_good(int p) {
	for(int i = 0; i < V; i++) {
		for(int j = 0; j < V; j++) {
			dist[i][j] = ((i == j) ? 0 : INF);
		}
	}
	for(int i = 0; i < p; i++) {
		if(Q[i] == 1) {
			setmin(dist[I[i]][J[i] + M], -K[i]);
		} else {
			setmin(dist[J[i] + M][I[i]], K[i]);
		}
	}
	all_sp();
	for(int i = 0; i < V; i++) {
		for(int j = 0; j < V; j++) {
		}
	}
	for(int i = 0; i < V; i++) {
		if(dist[i][i] < 0) return false;
	}
	return true;
}

int go() {
	cin >> M >> N >> S;
	for(int i = 0; i < S; i++) {
		cin >> I[i] >> J[i] >> Q[i] >> K[i];
		I[i] --, J[i] --;
	}
	V = N + M;
	int mi = 0;
	int ma = S + 1;
	while(ma - mi > 1) {
		int md = (mi + ma) / 2;
		if(is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return mi;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for(int i = 0; i < T; i++) {
		int res = go();
		cout << res << '\n';
	}

	return 0;
}
