#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 105;
int N;
int M;
int D[MAXN][MAXN];
const int MAXK = 110;
const int K = 105;
int cnt[MAXN];
int S[MAXN][MAXN][MAXK];

int main() {
	scanf("%d %d", &N, &M);
	for(int i = 0; i < M; i++) {
		int a, b, l;
		scanf("%d %d %d", &a, &b, &l);
		a--, b--;
		assert(!D[a][b]);
		assert(l);
		D[a][b] = l;
	}

	memset(S, -1, sizeof(S));
	for(int i = 0; i < N; i++) {
		memset(cnt, 0, sizeof(cnt));
		priority_queue<pii, vector<pii>, greater<pii> > pq;
		pq.push(pii(0, i));
		while(!pq.empty()) {
			int len = pq.top().A, cur = pq.top().B;
			pq.pop();
			if(cnt[cur] == K) continue;
			S[i][cur][cnt[cur]++] = len;
			for(int j = 0; j < N; j++) {
				if(!D[cur][j]) continue;
				pq.push(pii(len + D[cur][j], j));
			}
		}
	}

	int Q; scanf("%d", &Q);
	while(Q--) {
		int c, d, k; scanf("%d %d %d", &c, &d, &k);
		c--, d--, k--;
		if(c == d) k++;
		printf("%d\n", S[c][d][k]);
	}
}

