#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N, K;

bool A[MAXN]; // what we have

const int MAXR = int(1e5) + 200;
int R;

struct reaction {
	int S;
	int s[11];
	int P;
	int p[11];
	int rem;
} V[MAXR];

vector<int> needs[MAXN];

int main() {
	scanf("%d %d %d", &N, &K, &R);
	queue<int> made;
	for (int i = 0; i < K; i++) {
		int v; scanf("%d", &v);
		A[v] = true;
		made.push(v);
	}
	for (int i = 0; i < R; i++) {
		scanf("%d", &V[i].S);
		for (int j = 0; j < V[i].S; j++) {
			scanf("%d", &V[i].s[j]);
			needs[V[i].s[j]].push_back(i);
		}
		scanf("%d", &V[i].P);
		for (int j = 0; j < V[i].P; j++) {
			scanf("%d", &V[i].p[j]);
		}
		V[i].rem = V[i].S;
	}

	int res = 0;
	while (!made.empty()) {
		int s = made.front(); made.pop();
		for (vector<int>::iterator it = needs[s].begin(); it != needs[s].end(); it++) {
			int cur = *it;
			V[cur].rem--;
			if (!V[cur].rem) {
				for (int j = 0; j < V[cur].P; j++) {
					int p = V[cur].p[j];
					if (A[p]) continue;
					A[p] = true;
					made.push(p);
					res++;
				}
			}
		}
	}
	printf("%d\n", res);

	return 0;
}
