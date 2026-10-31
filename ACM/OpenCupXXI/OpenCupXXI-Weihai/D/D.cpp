#include <bits/stdc++.h>
using namespace std;

using ld = long double;

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int M = 110;
	vector<vector<ld> > ncr(M, vector<ld>(M, 0));
	ncr[0][0] = 1;
	for(int i = 1; i < M; i++){
		for(int j = 0; j < M; j++){
			ncr[i][j] += ncr[i-1][j];
			if(j) ncr[i][j] += ncr[i-1][j-1];
		}
	}

	int n, m;
	cin >> n >> m;
	vector<int> a(n);
	for(int& x : a) cin >> x;
	vector<vector<ld> > ways(m+1, vector<ld>(1<<n, 0));
	ways[0][0] = 1;
	for(int i = 1; i < (1 << n); i++){
		int f = (int)__builtin_ctz(i);
		for(int j = 0; j <= m; j++){
			if(ways[j][i ^ (1 << f)] == 0) continue;
			for(int r = 0; r < a[f] && r + j <= m; r++){
				ways[j + r][i] += ways[j][i ^ (1 << f)] * ncr[j+r][r];
			}
		}
	}

	vector<vector<ld> > prob(m+1, vector<ld>(1 << n, 0));
	// what is alive
	prob[0][(1 << n) - 1] = 1;
	for(int t = 0; t < m; t++){
		for(int b = 0; b < (1 << n); b++){
			if(prob[t][b] == 0) continue;
			int tfree = t;
			for(int dead = 0; dead < n; dead++){
				if(b & (1 << dead)) continue;
				tfree -= a[dead];
			}
			assert(b > 0);
			ld pthis = ld(1) / ld(__builtin_popcount(b));
			for(int nxt = 0; nxt < n; nxt++){
				if(b & (1 << nxt)){
					ld pkill = 0;
					if(tfree >= a[nxt] - 1){
						pkill = ncr[tfree][a[nxt] - 1] * ways[tfree - (a[nxt] - 1)][b ^ (1 << nxt)] / ways[tfree][b];
					}
					prob[t+1][b] += pthis * (1 - pkill) * prob[t][b];
					prob[t+1][b ^ (1 << nxt)] += pthis * pkill * prob[t][b];					
				}
			}
		}
	}

	ld ans = 0;
	for(int b = 0; b < (1 << n); b++){
		ans += prob[m][b] * (n - __builtin_popcount(b));
	}
	cout << fixed << setprecision(10);
	cout << ans << '\n';
}
