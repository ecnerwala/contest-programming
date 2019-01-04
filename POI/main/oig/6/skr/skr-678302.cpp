#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.2e6;
int N, M;
bool vis[MAXN];

const int MAXK = MAXN;
int K;
int W[MAXK];
int main() {
	scanf("%d %d", &N, &M);
	vis[1] = true;
	int numvis = 1;
	queue<int> unvis;
	for(int i = 2; i <= N; i++) unvis.push(i);
	for(int i = 0; i < M; i++) {
		scanf("%d", &K);
		for(int j = 0; j < K; j++) {
			scanf("%d", &W[j]);
		}
		int cnt = 0;
		for(int j = 0; j < K; j++) {
			if(vis[W[j]]) cnt++;
		}

		if(cnt < numvis) {
			for(int j = int(unvis.size()), k = 0; j; j--) {
				int v = unvis.front(); unvis.pop();
				while(k < K && vis[W[k]]) k++;
				if(k < K && W[k] == v) {
					unvis.push(v);
					k++;
				} else {
					vis[v] = true;
					numvis++;
				}
			}
		}


		if(vis[N]) {
			printf("%d\n", i);
			return 0;
		}
	}
	printf("%d\n", M);
	return 0;
}
