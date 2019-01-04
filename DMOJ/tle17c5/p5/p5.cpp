#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = ll(1e17) + 7;

const int MAXN = int(2e5) + 200;
int N;

int opts[MAXN][30];

int ch[MAXN][30];
int par[MAXN];

vector<ll> dfs(int cur, int depth) {
	unordered_map<ll, int> vals;
	vector<ll> res;
	res.push_back(26);
	for (int c = 0; c < 26; c++) {
		if (!ch[cur][c]) continue;
		for (ll hs : dfs(ch[cur][c], depth+1)) {
			res.push_back((hs * 27 + c) % MOD);
			if (vals.count(hs)) {
				opts[depth][c] ++;
				opts[depth][vals[hs]] ++;
				vals[hs] = 28; // unused slot
			} else {
				vals[hs] = c;
			}
		}
	}
	return res;
}

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N-1; i++) {
		int a, b; char c; scanf("%d %d %c", &a, &b, &c);
		assert(a < b);
		assert(0 <= a && a < N);
		assert(0 <= b && b < N);
		assert('a' <= c && c <= 'z');
		assert(par[b] == 0);
		par[b] = a;
		assert(ch[a][c-'a'] == 0);
		ch[a][c-'a'] = b;
	}

	dfs(0, 0);
	int res = N;
	for (int i = 0; i <= N; i++) {
		for (int c = 0; c < 26; c++) {
			res = min(res, N - opts[i][c]);
		}
	}
	printf("%d\n", res);

	return 0;
}
