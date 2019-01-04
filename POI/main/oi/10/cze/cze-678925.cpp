#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second
const int MAXN = 2e3;
const int MAXM = 2e3;
int N, M;

const int MAXV = MAXN + MAXM;
int V;
pii vals[MAXV];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	N--, M--;
	V = 0;
	for(int i = 0; i < N; i++) {
		int v; cin >> v;
		vals[V++] = pii(v, 0);
	}
	for(int i = 0; i < M; i++) {
		int v; cin >> v;
		vals[V++] = pii(v, 1);
	}
	assert(V == N + M);
	sort(vals, vals + V);
	reverse(vals, vals + V);
	long long res = 0;
	long long cnt[2] = {1, 1};
	for(int i = 0; i < V; i++) {
		res += (long long)(vals[i].A) * cnt[vals[i].B];
		cnt[!vals[i].B] ++;
	}
	cout << res << '\n';
	return 0;
}
