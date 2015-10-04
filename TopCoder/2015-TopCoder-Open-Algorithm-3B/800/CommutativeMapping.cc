#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class CommutativeMapping {

	static const ll MOD = 1e9 + 7;

	static const int MAXN = 3000;
	int N;
	int F[MAXN];
	vector<int> prv[MAXN];

	int indeg[MAXN];

	ll dp[MAXN][MAXN]; // number of ways for subtree at i -g> j
	ll dp2[MAXN][MAXN]; // number of ways for subtree at i -g> -f> j

	bool vis[MAXN];

public:

	int count(vector <int> f) {
		N = int(f.size());
		for(int i = 0; i < N; i++) {
			F[i] = f[i];
			prv[F[i]].push_back(i);
			indeg[F[i]] ++;
		}
		stack<int> proc;
		for(int i = 0; i < N; i++) {
			if(indeg[i] == 0) proc.push(i);
		}
		while(!proc.empty()) {
			int cur = proc.top(); proc.pop();
			if((--indeg[F[cur]]) == 0) proc.push(F[cur]);
			for(int img = 0; img < N; img++) {
				dp[cur][img] = 1;
				for(int ch : prv[cur]) {
					dp[cur][img] *= dp2[ch][img];
					dp[cur][img] %= MOD;
				}

				dp2[cur][F[img]] += dp[cur][img];
				dp2[cur][F[img]] %= MOD;
			}
		}

		for(int cur = 0; cur < N; cur++) {
			if(!indeg[cur]) continue;
			for(int img = 0; img < N; img++) {
				dp[cur][img] = 1;
				for(int ch : prv[cur]) {
					if(indeg[ch]) continue;
					dp[cur][img] *= dp2[ch][img];
					dp[cur][img] %= MOD;
				}
			}
		}

		ll res = 1;
		for(int st = 0; st < N; st++) {
			if(!indeg[st]) continue;
			assert(indeg[st] == 1);
			if(vis[st]) continue;
			int cur = st;
			do {
				vis[cur] = true;
				cur = F[cur];
			} while(cur != st);
			ll tot = 0;
			for(int img = 0; img < N; img++) {
				ll cnt = 1;
				cur = st;
				int curimg = img;
				do {
					cnt *= dp[cur][curimg];
					cnt %= MOD;
					cur = F[cur];
					curimg = F[curimg];
				} while(cur != st);
				assert(cur == st);
				if(curimg != img) continue;
				tot += cnt;
				tot %= MOD;
			}
			res *= tot;
			res %= MOD;
		}
		return res;
	}
};

// vim:ft=cpp
