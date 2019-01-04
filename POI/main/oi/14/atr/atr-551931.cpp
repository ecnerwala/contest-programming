#include<iostream>
#include<vector>
#include<queue>

using namespace std;

typedef long long ll;

typedef pair<int, int> pii;

#define A first
#define B second

const int INF = 1e9;

const int MAXN = 2e4 + 20;
const int MAXM = 2e5 + 20;
const int MAXK = 22;
int N, M;
int P[MAXM], Q[MAXM], L[MAXM];
vector<pii> adj[MAXN];

int K;
int minDist[MAXK][MAXN];

const int MAXG = MAXK * MAXK;
int G;
int R[MAXG], S[MAXG];
int reqs[MAXK];

const int MAXB = (1 << 20) + 20;
int dp[MAXB * MAXK / 2];
int bst[MAXB]; // start positions of each bitmask

void search(int st) {
	for(int i = 0; i < N; i++) {
		minDist[st][i] = INF;
	}
	priority_queue<pii> pq;

	pq.push(pii(-0, st));
	minDist[st][st] = 0;

	while(!pq.empty()) {
		pii val = pq.top(); pq.pop();
		int cur = val.B;
		int dist = -val.A;
		if(dist == minDist[st][cur]) {
			for(__typeof(adj[cur].begin()) it = adj[cur].begin(); it != adj[cur].end(); it++) {
				int nxt = it->A;
				int nDist = dist + it->B;
				if(nDist < minDist[st][nxt]) {
					minDist[st][nxt] = nDist;
					pq.push(pii(-nDist, nxt));
				}
			}
		}
	}
}

inline int dsum(int b) {
	int res = 0;
	while(b) {
		res++;
		b &= b - 1;
	}
	return res;
}

int maxVal[MAXK];

int go() {
	for(int i = 0; i < M; i++) {
		adj[P[i]].push_back(pii(Q[i], L[i]));
		adj[Q[i]].push_back(pii(P[i], L[i]));
	}
	for(int i = 0; i <= K; i++) {
		search(i);
	}

	//for(int i = 0; i <= K; i++) for(int j = 0; j <= K + 1; j++) cerr << minDist[i][j] << ' '; cerr << '\n';

	for(int i = 0; i < K; i++) reqs[i] = 0;
	for(int i = 0; i < G; i++) {
		reqs[S[i] - 1] |= (1 << (R[i] - 1));
	}

	bst[0] = 0;
	for(int b = 0; b < (1 << K); b++) {
		bst[b + 1] = bst[b] + dsum(b);
	}

	for(int i = 0; i < bst[1 << K]; i++) {
		dp[i] = INF;
	}

	for(int i = 0; i < K; i++) {
		if(reqs[i] == 0) {
			dp[bst[1 << i]] = minDist[0][i + 1];
		}
	}
	
	for(int b = 1; b < (1 << K); b++) {
		bool reach = false;
		//for(int c = 0; c < K; c++) {
			//if(dp[b][c] != INF) reach = true;
		//}
		for(int i = 0, c = bst[b]; i < K; i++) {
			if(b & (1 << i)) {
				maxVal[i] = dp[c];
				c++;
				if(maxVal[i] != INF) reach = true;
			} else {
				maxVal[i] = INF;
			}
		}
		if(reach) {
			for(int n = 0; n < K; n++) {
				if(((b & reqs[n]) == reqs[n]) && ((b & (1 << n)) == 0)) {
					int v = INF;
					for(int c = 0; c < K; c++) {
						if(maxVal[c] != INF) {
							v = min(v, maxVal[c] + minDist[c + 1][n + 1]);
						}
					}
					dp[bst[b | (1 << n)] + dsum(b & ((1 << n) - 1))] = v;
					//dp[b | (1 << n)][n] = v;
					//cerr << b << ' ' <<  n << ' ' << v << '\n';
				}
			}
		}
	}

	int res = INF;
	for(int i = 0; i < K; i++) {
		res = min(res, dp[bst[(1 << K) - 1] + i] + minDist[i + 1][N - 1]);
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K;
	for(int i = 0; i < M; i++) {
		cin >> P[i] >> Q[i] >> L[i];
		P[i] --, Q[i] --;
	}
	cin >> G;
	for(int i = 0; i < G; i++) {
		cin >> R[i] >> S[i];
		R[i] --, S[i] --;
	}

	cout << go() << '\n';
	return 0;
}
