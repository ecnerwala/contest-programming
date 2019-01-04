#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e3;; 
int N;
vector<pair<pii, pii> > adj[MAXN];
// (end, dist), (start, mod)

int K;
int X, Y;
int T;

int P[MAXN];
int R[MAXN];

int dist[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	cin >> K;
	cin >> X >> Y; X--, Y--;
	int h, m; cin >> h >> m;
	T = h * 60 + m;
	for(int z = 0; z < K; z++) {
		int L; cin >> L;
		int F; cin >> F;
		assert(60 % F == 0);
		for(int i = 0; i < L; i++) {
			cin >> P[i];
			P[i] --;
		}
		for(int i = 0; i < L - 1; i++) {
			cin >> R[i];
		}
		for(int w = 0; w < 2; w++) {
			int t = 0;
			for(int i = 0; i < L; i++) {
				adj[P[i]].push_back(make_pair(pii(P[i + 1], R[i]), pii(t, F)));
				t += R[i];
				t %= F;
			}
			reverse(P, P + L);
			reverse(R, R + L - 1);
		}
	}
	for(int i = 0; i < N; i++) {
		dist[i] = INF;
	}
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	dist[X] = T;
	pq.push(pii(T, X));
	while(!pq.empty()) {
		int d = pq.top().A;
		int cur = pq.top().B;
		pq.pop();
		if(dist[cur] != d) continue;
		if(cur == Y) {
			cout << d / 60 % 24 << ' ' << d % 60 << '\n';
			break;
		}

		for(__typeof(adj[cur].begin()) it = adj[cur].begin(); it != adj[cur].end(); it++) {
			int nxt = it->A.A;
			int nd = (d + it->B.B - 1 - it->B.A) / it->B.B * it->B.B + it->B.A;
			assert(nd % it->B.B == it->B.A);
			assert(nd >= d);
			assert(nd - it->B.B < d);
			nd += it->A.B;
			if(nd < dist[nxt]) {
				dist[nxt] = nd;
				pq.push(pii(nd, nxt));
			}
		}
	}
}
