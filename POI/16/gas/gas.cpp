#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2e5;
const int MAXK = 30;

int N, K;
ll S;

ll ans = 0;

ll cnt[MAXN][MAXK]; // number of chambers

vector<int> adj[MAXN];

void dfs(int a, int p = -1) {
	cnt[a][0] = 1;
	for(vector<int>::iterator it = adj[a].begin(); it != adj[a].end(); it++) {
		int n = *it;
		if(n == p) continue;
		dfs(n, a);
	}
	for(int i = 0; i <= K; i++) {
		if(i == K) {
			if(p == -1) {
				for(int i = 0; i < K; i++) {
					if(cnt[a][i] < 0) cnt[a][i] = 0;
					cnt[a][K] += cnt[a][i];
					cnt[a][i] = 0;
				}
			}

			if(cnt[a][K] > 0) {
				ll num = (cnt[a][K] + S - 1) / S;
				ans += num;
				cnt[a][K] -= num * S;
			}
		}
		for(int j = i; j >= ((p == -1) ? 0 : max(i-1,0)) && cnt[a][i] < 0; j--) {
			if(cnt[a][j] > 0) {
				ll v = min(cnt[a][j], -cnt[a][i]);
				cnt[a][i] += v;
				cnt[a][j] -= v;
			}
		}
	}

	if(p != -1) {
		for(int i = 0; i <= K; i++) {
			if(cnt[a][i] < 0) {
				if(i > 0) {
					cnt[p][i - 1] += cnt[a][i];
				}
			} else {
				if(i < K) {
					cnt[p][i + 1] += cnt[a][i];
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> S >> K;
	for(int i = 0; i < N - 1; i++) {
		int x, y; cin >> x >> y;
		x--, y--;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}

	dfs(0);
	cout << ans << '\n';
	return 0;
}
