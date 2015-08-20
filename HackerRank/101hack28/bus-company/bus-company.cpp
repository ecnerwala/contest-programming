#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 200;
const int MAXM = 200;
const int MAXK = 200;
const int MAXC = 200;

int N, M, K;
pii A[MAXN];
int C[MAXN];

bool vis[MAXN]; // whether or not it's used

int revcap[MAXM]; // linear reverse capacity

vector<int> oute[MAXM];
vector<int> ine[MAXM];

int prv[MAXM];
int dist[MAXM];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K;
	K = min(K, N);
	for(int i = 0; i < N; i++) {
		cin >> A[i].A >> A[i].B >> C[i];
		A[i].A --, A[i].B --;
		oute[A[i].A].push_back(i);
		ine[A[i].B].push_back(i);
	}

	int res = 0;
	for(int i = 0; i < K; i++) {
		// we need to find the longest path through this graph
		// should have no negative cycles
		priority_queue<pii> pq;
		for(int j = 1; j <= M; j++) dist[j] = -1;
		dist[0] = 0;
		pq.push(pii(0, 0));
		while(!pq.empty()) {
			int cur = pq.top().B;
			int d = pq.top().A;
			pq.pop();
			assert(d <= dist[cur]);
			if(d < dist[cur]) continue;

			if(cur < M) {
				if(d > dist[cur + 1]) {
					prv[cur + 1] = -1;
					dist[cur + 1] = d;
					pq.push(pii(dist[cur + 1], cur + 1));
				}
			}

			if(revcap[cur]) {
				assert(cur);
				if(d > dist[cur - 1]) {
					prv[cur - 1] = -2;
					dist[cur - 1] = d;
					pq.push(pii(dist[cur - 1], cur - 1));
				}
			}

			for(int e : oute[cur]) {
				if(vis[e]) continue;
				int nd = d + C[e];
				if(nd > dist[A[e].B]) {
					prv[A[e].B] = e;
					dist[A[e].B] = nd;
					pq.push(pii(dist[A[e].B], A[e].B));
				}
			}

			for(int e : ine[cur]) {
				if(!vis[e]) continue;
				int nd = d - C[e];
				if(nd > dist[A[e].A]) {
					prv[A[e].A] = e;
					dist[A[e].A] = nd;
					pq.push(pii(dist[A[e].A], A[e].A));
				}
			}
		}
		assert(dist[M] >= 0);
		if(dist[M] == 0) break;
		res += dist[M];
		for(int cur = M; cur != 0; ) {
			if(prv[cur] == -1) {
				revcap[cur] ++;
				cur --;
			} else if(prv[cur] == -2) {
				cur ++;
				assert(revcap[cur]);
				revcap[cur] --;
			} else {
				int e = prv[cur];
				vis[e] ^= 1;
				cur = A[e].A + A[e].B - cur;
			}
		}
	}
	cout << res << '\n';
	return 0;
}
