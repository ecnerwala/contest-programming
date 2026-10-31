#include<bits/stdc++.h>

using namespace std;
typedef long long ll;

const int N = 200 + 5;
const ll lim = 1e9;

inline void checkmax(int &x, int y){
	if(y > x) x = y;
}

inline void checkmax(ll &x, ll y){
	if(y > x) x = y;
}

inline void checkmax(pair<int, int> &x, pair<int, int> y){
	if(y > x) x = y;
}

int n = 0, m = 0, k = 0, opt[N] = {}, a[N] = {}, b[N] = {}, w[N] = {};
int f[N][N][N] = {}, g[N] = {}, g_a[N][N][N] = {}, g_b[N][N][N] = {};
pair<int, int> dp[N][N * N] = {};
vector<vector<int> > G(N);

int main(){
	scanf("%d %d", &n, &m);
	for(int u = 1 ; u <= n ; u ++){
		scanf("%d", &opt[u]);
		if(opt[u] == 1) scanf("%d %d", &a[u], &b[u]);
		else if(opt[u] == 2) scanf("%d", &a[u]);
		else if(opt[u] == 3) scanf("%d", &b[u]);
		else if(opt[u] == 4) scanf("%d", &w[u]);
		k = max(k, max(a[u], b[u]));
	}
	for(int i = 1, u = 0, v = 0 ; i <= m ; i ++){
		scanf("%d %d", &u, &v);
		G[u].push_back(v);
	}
	memset(f, -1, sizeof(f));
	memset(g, -1, sizeof(g)), memset(g_a, -1, sizeof(g_a)), memset(g_b, -1, sizeof(g_b));
	memset(dp, -1, sizeof(dp));
	f[1][0][0] = 0;
	for(int u = 1 ; u <= n ; u ++){
		if(opt[u] == 1 && g[u] != -1){
			for(int x = a[u] ; x <= k ; x ++) checkmax(g_a[u][x][x - a[u]], g[u] + x * b[u]);
			for(int x = b[u] ; x <= k ; x ++) checkmax(g_b[u][x][x - b[u]], g[u] + x * a[u]);
			checkmax(dp[u][a[u]], make_pair(b[u], g[u]));
		}
		for(int v : G[u]){
			if(opt[v] == 1){
				for(int x = 0 ; x <= k ; x ++) for(int y = 0 ; y <= k ; y ++){
					if(f[u][x][y] != -1){
						checkmax(f[v][max(x, a[v])][max(y, b[v])], f[u][x][y] + a[v] * b[v]);
						checkmax(g[v], f[u][x][y]);
					}
					if(g_a[u][x][y] != -1){
						checkmax(g_a[v][max(x, a[v])][y], g_a[u][x][y] + a[v] * b[v]);
						if(!y){
							checkmax(g[v], g_a[u][x][y]);
							checkmax(f[v][x][b[v]], g_a[u][x][y] + a[v] * b[v]);
						}
					}
					if(g_b[u][x][y] != -1){
						checkmax(g_b[v][max(x, b[v])][y], g_b[u][x][y] + a[v] * b[v]);
						if(!y){
							checkmax(g[v], g_b[u][x][y]);
							checkmax(f[v][a[v]][x], g_b[u][x][y] + a[v] * b[v]);
						}
					}
				}
				for(int x = 0 ; x <= n * k ; x ++) if(dp[u][x] != make_pair(-1, -1)){
					int y = dp[u][x].first, z = dp[u][x].second;
					checkmax(dp[v][x], make_pair(y, z + a[v] * b[v]));
				}
			}
			else if(opt[v] == 2){
				for(int x = 0 ; x <= k ; x ++) for(int y = 0 ; y <= k ; y ++){
					if(f[u][x][y] != -1) checkmax(f[v][x][y], f[u][x][y] + a[v] * y);
					if(g_a[u][x][y] != -1 && y >= a[v]) checkmax(g_a[v][x][y - a[v]], g_a[u][x][y]);
					if(g_b[u][x][y] != -1) checkmax(g_b[v][x][y], g_b[u][x][y] + a[v] * x);
				}
				for(int x = 0 ; x <= n * k ; x ++) if(dp[u][x] != make_pair(-1, -1)){
					int y = dp[u][x].first, z = dp[u][x].second;
					checkmax(dp[v][x + a[v]], make_pair(y, z));
				}
			}
			else if(opt[v] == 3){
				for(int x = 0 ; x <= k ; x ++) for(int y = 0 ; y <= k ; y ++){
					if(f[u][x][y] != -1) checkmax(f[v][x][y], f[u][x][y] + x * b[v]);
					if(g_a[u][x][y] != -1) checkmax(g_a[v][x][y], g_a[u][x][y] + x * b[v]);
					if(g_b[u][x][y] != -1 && y >= b[v]) checkmax(g_b[v][x][y - b[v]], g_b[u][x][y]);
				}
				for(int x = 0 ; x <= n * k ; x ++) if(dp[u][x] != make_pair(-1, -1)){
					int y = dp[u][x].first, z = dp[u][x].second;
					checkmax(dp[v][x], make_pair(y + b[v], z));
				}
			}
			else{
				for(int x = 0 ; x <= k ; x ++) for(int y = 0 ; y <= k ; y ++){
					if(f[u][x][y] != -1) checkmax(f[v][x][y], f[u][x][y] + w[v]);
					if(g_a[u][x][y] != -1) checkmax(g_a[v][x][y], g_a[u][x][y] + w[v]);
					if(g_b[u][x][y] != -1) checkmax(g_b[v][x][y - b[v]], g_b[u][x][y] + w[v]);
				}
				for(int x = 0 ; x <= n * k ; x ++) if(dp[u][x] != make_pair(-1, -1)){
					int y = dp[u][x].first, z = dp[u][x].second;
					checkmax(dp[v][x], make_pair(y, z + w[v]));
				}
			}
		}
	}
	ll ans = f[n][0][0];
	for(int x = 0 ; x <= n * k ; x ++) if(dp[n][x] != make_pair(-1, -1)){
		int y = dp[n][x].first, z = dp[n][x].second;
		checkmax(ans, lim * x * y + z);
	}
	printf("%lld", ans);
	return 0;
}
