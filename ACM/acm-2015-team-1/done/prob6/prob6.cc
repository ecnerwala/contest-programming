#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define A first
#define B second

const int MAXN = 2e5;
const int MAXM = 2e6;

int N, M;
ll K;
vector<pll> adj[MAXN];
ll dist[MAXN][2]; // shortest and shortest != first

int S, T;

ll go() {
	memset(dist, -1, sizeof(dist));
	priority_queue<pll, vector<pll>, greater<pll>> pq;
	pq.push(pll(0, S));
	while(!pq.empty()) {
		ll d = pq.top().A;
		ll cur = pq.top().B;
		pq.pop();
		if(cur == T && d % K != 0) return d;
		if(dist[cur][0] == -1) {
			dist[cur][0] = d;
		} else if(dist[cur][1] == -1) {
			assert(d >= dist[cur][0]);
			if(d % K == dist[cur][0] % K) {
				continue;
			} else {
				dist[cur][1] = d;
			}
		} else {
			assert(d >= dist[cur][1]);
			continue;
		}
		for(pll edge : adj[cur]) {
			int nxt = edge.A;
			ll nd = d + edge.B;
			pq.push(pll(nd, nxt));
		}
	}
	return -1;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K;
	S = 0, T = N - 1;
	for(int i = 0; i < M; i++) {
		ll a, b, d; cin >> a >> b >> d;
		adj[a].push_back(pll(b, d));
		adj[b].push_back(pll(a, d));
	}
	cout << go() << '\n';
	return 0;
}
