#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

typedef pair<int, int> pii;
#define A first
#define B second

const pii BAD(-2, -2);

const int MAXN = 3e3;
int N;

char G[MAXN][MAXN];

pii cent;
int W[MAXN];
int d_up[MAXN];
int d_down[MAXN];

int prv[MAXN][MAXN];
int nxt[MAXN][MAXN];

int closed[MAXN][MAXN];

// assumes it's vertical and it's definitely connected/convex
// we just need to determine if it's the right axis of symmetry
pii find_center() {
	int maxw = -1;
	int r = -1;
	int c = -1;
	for(int i = 0; i < N; i++) {
		W[i] = 0;
		int minc = N + 1, maxc = -1;
		for(int j = 0; j < N; j++) {
			if(G[i][j] == 'r') {
				minc = min(minc, j);
				maxc = max(maxc, j);
			}
		}
		if(minc > maxc) continue;
		if((maxc - minc) % 2 != 0) return BAD;
		int mid = (minc + maxc) / 2;
		if(c != -1 && c != mid) {
			return BAD;
		}
		c = mid;

		int w = (maxc - minc) / 2 + 1;
		if(w > maxw) {
			r = i;
			maxw = w;
		}
		W[i] = w;
	}
	assert(r >= 0);
	assert(c >= 0);
	d_up[N+1] = 0;
	d_down[N+1] = 0;
	for(int i = N; i >= 0; i--) {
		d_up[i] = d_up[i+1], d_down[i] = d_down[i+1];
		while(r - d_up[i] >= 0 && W[r-d_up[i]] > i) d_up[i] ++;
		while(r + d_down[i] < N && W[r+d_down[i]] > i) d_down[i] ++;
	}
	// can't do (i - d_down, i + d_up) if width (dist to next) is w
	return pii(r, c);
}

int dist[MAXN][MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) cin >> G[i];
	cent = find_center();
	if(cent == BAD) {
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < i; j++) {
				swap(G[i][j], G[j][i]);
			}
		}
		cent = find_center();
	}
	//cerr << cent.A << ' ' << cent.B << '\n';
	//cerr << d_up[0] << ' ' << d_down[0] << '\n';
	//cerr << W[cent.A] << '\n';
	for(int i = 0; i < N; i++) {
		prv[i][0] = -INF;
		for(int j = 0; j < N; j++) {
			if(G[i][j] == 'X') prv[i][j] = j;
			prv[i][j+1] = prv[i][j];
		}
		nxt[i][N] = INF;
		for(int j = N-1; j >= 0; j--) {
			nxt[i][j] = nxt[i][j+1];
			if(G[i][j] == 'X') nxt[i][j] = j;
		}
	}

	for(int j = 0; j < N; j++) {
		for(int i = 0; i <= N; i++) closed[i][j] = 0;
		for(int i = 0; i < N; i++) {
			int w = min(N+1, min(j - prv[i][j], nxt[i][j] - j));
			// (i - d_up[w], i + d_down[w]) is bad
			if(d_up[w] == 0 && d_down[w] == 0) continue;
			closed[max(i-d_down[w] + 1, 0)][j] ++;
			closed[min(i+d_up[w], N)][j] --;
		}
		for(int i = 0; i < N; i++) {
			closed[i+1][j] += closed[i][j];
		}
		assert(closed[N][j] == 0);
	}
	//for(int i = 0; i < N; i++) { for(int j = 0; j < N; j++) { cerr << bool(closed[i][j]); } cerr << '\n'; } cerr << '\n';
	assert(!closed[cent.A][cent.B]);
	memset(dist, -1, sizeof(dist));
	queue<pii> q;
	dist[cent.A][cent.B] = 0;
	q.push(cent);
	while(!q.empty()) {
		pii p = q.front(); q.pop();
		int x = p.A, y = p.B;
		int d = dist[x][y];
		if(x-1 >= 0 && !closed[x-1][y] && dist[x-1][y] == -1) {
			dist[x-1][y] = d+1;
			q.emplace(x-1, y);
		}
		if(x+1 < N && !closed[x+1][y] && dist[x+1][y] == -1) {
			dist[x+1][y] = d+1;
			q.emplace(x+1, y);
		}
		if(y-1 >= 0 && !closed[x][y-1] && dist[x][y-1] == -1) {
			dist[x][y-1] = d+1;
			q.emplace(x, y-1);
		}
		if(y+1 < N && !closed[x][y+1] && dist[x][y+1] == -1) {
			dist[x][y+1] = d+1;
			q.emplace(x, y+1);
		}
	}
	//for(int i = 0; i < N; i++) { for(int j = 0; j < N; j++) { cerr << dist[i][j] << ' '; } cerr << '\n'; } cerr << '\n';

	int res = INF;
	for(int i = 0; i < N; i++) {
		if(dist[i][0] != -1) res = min(res, dist[i][0] + W[cent.A]);
		if(dist[i][N-1] != -1) res = min(res, dist[i][N-1] + W[cent.A]);
		if(dist[0][i] != -1) res = min(res, dist[0][i] + d_down[0]);
		if(dist[N-1][i] != -1) res = min(res, dist[N-1][i] + d_up[0]);
	}
	if(res == INF) {
		cout << "NIE\n";
	} else {
		cout << res << '\n';
	}
	return 0;
}
