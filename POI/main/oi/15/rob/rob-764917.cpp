#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

typedef pair<int, int> pii;
#define A first
#define B second

const pii BAD(-2, -2);

const int MAXN = 5e3;
int N;

char G[MAXN][MAXN];

pii cent;
int W[MAXN];
int d_up[MAXN];
int d_down[MAXN];

int prv[MAXN];
int nxt[MAXN];

int closed[MAXN][MAXN];
bool is_free[MAXN][MAXN];

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
	scanf("%d", &N);
	for(int i = 0; i < N; i++) scanf("%s", G[i]);
	cent = find_center();
	if(cent == BAD) {
		//cerr << "flipping\n";
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < i; j++) {
				swap(G[i][j], G[j][i]);
			}
		}
		cent = find_center();
	}
	/*
	cerr << cent.A << ' ' << cent.B << '\n';
	cerr << d_up[0] << ' ' << d_down[0] << '\n';
	cerr << W[cent.A] << '\n';
	*/

	for(int x = 0; x < N; x++) {
		prv[0] = -INF;
		int i = x - cent.A + N;
		for(int j = 0; j <= 2 * N; j++) {
			int y = j - N + cent.B;
			if(0 <= y && y < N && G[x][y] == 'X') prv[j] = j;
			prv[j+1] = prv[j];
		}
		nxt[2*N+1] = INF;
		for(int j = 2 * N; j >= 0; j--) {
			int y = j - N + cent.B;
			nxt[j] = nxt[j+1];
			if(0 <= y && y < N && G[x][y] == 'X') nxt[j] = j;
		}
		for(int j = 0; j <= 2 * N; j++) {
			int w = min(N+1, min(j - prv[j], nxt[j] - j));
			// (i - d_up[w], i + d_down[w]) is bad
			if(d_up[w] == 0 && d_down[w] == 0) continue;
			closed[max(i-d_down[w] + 1, 0)][j] ++;
			closed[min(i+d_up[w], 2*N+1)][j] --;
		}
	}
	for(int j = 0; j <= 2 * N; j++) {
		for(int i = 0; i <= 2 * N; i++) {
			closed[i+1][j] += closed[i][j];
		}
		assert(closed[2*N+1][j] == 0);
	}
	assert(!closed[N][N]);
	//for(int i = 0; i <= 2*N; i++) { for(int j = 0; j <= 2*N; j++) { cerr << bool(closed[i][j]); } cerr << '\n'; } cerr << '\n';

	for(int i = 0; i <= 2 * N; i++) {
		int l = 0, r = 0;
		if(N - cent.A <= i && i < N - cent.A + N) {
			l = N - cent.B - W[cent.A];
			r = N - cent.B + (N-1) + W[cent.A];
		} else if(i > 0 && i < N - cent.A) {
			int w = W[N - i];
			if(w) {
				l = N - cent.B - w;
				r = N - cent.B + (N-1) + w;
			}
		} else if(i < 2 * N && i >= 2 * N - cent.A) {
			int w = W[2 * N - 1 - i];
			if(w) {
				l = N - cent.B - w;
				r = N - cent.B + (N-1) + w;
			}
		}
		for(int j = 0; j <= l; j++) is_free[i][j] = true;
		for(int j = r; j <= 2 * N; j++) is_free[i][j] = true;
	}

	memset(dist, -1, sizeof(dist));
	queue<pii> q;
	dist[N][N] = 0;
	q.push(pii(N, N));
	while(!q.empty()) {
		pii p = q.front(); q.pop();
		int x = p.A, y = p.B;
		int d = dist[x][y];
		if(is_free[x][y]) {
			printf("%d\n", d);
			return 0;
		}
		if(x-1 >= 0 && !closed[x-1][y] && dist[x-1][y] == -1) {
			dist[x-1][y] = d+1;
			q.push(pii(x-1, y));
		}
		if(x+1 <= 2*N && !closed[x+1][y] && dist[x+1][y] == -1) {
			dist[x+1][y] = d+1;
			q.push(pii(x+1, y));
		}
		if(y-1 >= 0 && !closed[x][y-1] && dist[x][y-1] == -1) {
			dist[x][y-1] = d+1;
			q.push(pii(x, y-1));
		}
		if(y+1 <= 2*N && !closed[x][y+1] && dist[x][y+1] == -1) {
			dist[x][y+1] = d+1;
			q.push(pii(x, y+1));
		}
	}
	//for(int i = 0; i < N; i++) { for(int j = 0; j < N; j++) { cerr << dist[i][j] << ' '; } cerr << '\n'; } cerr << '\n';

	printf("NIE\n");
	return 0;
}
