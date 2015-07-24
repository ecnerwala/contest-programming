#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

const int MAXN = 2e6;
int par[MAXN][2];

unordered_map<int, int> vals[MAXN][2];
unordered_set<int> els[MAXN][2];

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	while(T--) {
		int N, Q;
		cin >> N >> Q;

		ll ans = 0;
		ll res = 0;

		for(int i = 0; i < N; i++) {
			par[i][0] = par[i][1] = i;
			els[i][0].clear(), els[i][1].clear();
			els[i][0].insert(i), els[i][1].insert(i);
			vals[i][0].clear(), vals[i][1].clear();
			vals[i][0][i]++, vals[i][1][i]++;
		}

		for(int q = 1; q <= Q; q++) {
			int t, a, b; cin >> t >> a >> b;
			t--, a--, b--;

			a = par[a][t];
			b = par[b][t];

			assert(!els[a][t].empty());
			assert(!els[b][t].empty());

			if(a != b) {
				if(els[a][t].size() > els[b][t].size()) swap(a, b);

				vals[a][t].clear();

				for(int v : els[a][t]) {
					res += vals[b][t][par[v][!t]];
				}

				for(int v : els[a][t]) {
					par[v][t] = b;
					vals[par[v][!t]][!t][a]--;
					vals[par[v][!t]][!t][b]++;
					els[b][t].insert(v);
					vals[b][t][par[v][!t]]++;
				}

				els[a][t].clear();
			}

			ans = (ans + (res % MOD) * (q % MOD)) % MOD;
		}
		cout << ans << '\n';
	}
	return 0;
}
