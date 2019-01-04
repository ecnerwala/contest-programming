#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
const int MAXN = int(1e3) * 200;
int N;
int P;
int S;
pii X[MAXN];
bool vis[MAXN];

ll sq(ll a) { return a * a; }

ll h(pii a, pii b) { return sq(a.first - b.first) + sq(a.second - b.second); }

ll res = 0;

void dfs(int cur) {
	vis[cur] = true;
	res = max(res, h(X[P], X[cur]));
	for (int i = 0; i < N; i++) {
		if (vis[i]) continue;
		if (h(X[cur], X[i]) <= sq(S)) {
			dfs(i);
		}
	}
}

int main() {
	scanf("%d %d %d", &N, &P, &S);
	P--;
	for (int i = 0; i < N; i++) scanf("%d %d", &X[i].first, &X[i].second);
	dfs(P);
	printf("%.3f\n", sqrt(double(res)) + S);
	return 0;
}
