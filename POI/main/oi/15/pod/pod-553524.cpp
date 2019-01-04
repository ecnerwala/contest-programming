#include<iostream>

using namespace std;

const int MAXN = 30;
const int MAXM = MAXN * (MAXN - 1) / 2;

int N, M;
int adj[MAXN];
int deg[MAXN];

int best = MAXM;
int bestType = 0;

int cost = 0;
int cnt0 = 0, cnt1 = 0;
int cur = 0;
int type = 0;

inline int digSum(int v) {
	int res = 0;
	for(; v; v &= (v - 1)) res ++;
	return res;
}

void dfs() {
	//cerr << cur << ' ' << cost << '\n';
	//for(int i = 0; i < N; i++) cerr << type[i]; cerr << '\n';
	if(cost >= best) return;
	if(cur == N) {
		best = cost;
		//for(int i = 0; i < N; i++) bestType[i] = type[i];
		bestType = type;
	} else {
		int v = digSum(adj[cur] & type);
		//for(int i = 0; i < cur; i++) if(adj[cur][i] && type[i]) v++;
		if(cur > 0 && cnt0 < N / 2) {
			cnt0 ++;
			cost += v;
			cur++;
			dfs();
			cur--;
			cnt0 --;
			cost -= v;
		}
		v = deg[cur] - v;
		if(cnt1 < N / 2) {
			cnt1 ++;
			cost += v;
			type += 1 << cur;
			cur++;
			dfs();
			cur--;
			type -= 1 << cur;
			cnt1 --;
			cost -= v;
		}
	}
}

int main() {
	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		adj[i] = 0;
	}
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		if(a > b) swap(a, b);
		adj[a] += 1 << b;
		adj[b] += 1 << a;
		deg[b] ++;
	}

	dfs();

	cerr << best << '\n';
	cout << 1;
	bestType /= 2;
	for(int i = 1; i < N; i++, bestType /= 2) {
		if(bestType & 1) cout << ' ' << i + 1;
	}
	cout << '\n';
	return 0;
}
