#include<bits/stdc++.h>
using namespace std;

typedef double ld;

int n;
int h;
ld p;
int x[2222];
ld dp[2222][2222][2][2];

int main() {
	cin >> n >> h >> p;
	for(int i = 1; i <= n; i++) { cin >> x[i]; }
	sort(x + 1, x + n + 1);
	x[0] = x[1] - h;
	x[n + 1] = x[n] + h;

	dp[1][n][0][0] = 1;

	ld res = 0;

	for(int l = n; l > 0; l--) {
		for(int s = 1, t = l; t <= n; s++, t++) {
			for(int ds = 0; ds < 2; ds++) {
				for(int dt = 0; dt < 2; dt++) {
					ld prob = dp[s][t][ds][dt];

					ld sl = 0.5 * p, sr = 0.5 * (1 - p), tl = 0.5 * p, tr = 0.5 * (1 - p);

					// cascade through
					if(ds && x[s] - x[s - 1] < h) {
						sr = 1, sl = tl = tr = 0;
					}

					// cascade through
					if(dt && x[t + 1] - x[t] < h) {
						tl = 1, sl = sr = tr = 0;
					}

					if(s == t) {

						sl += tl;
						tl = 0;

						tr += sr;
						sr = 0;

					}

					//assert(sl + sr + tl + tr == 1);

					sl *= prob;
					sr *= prob;
					tl *= prob;
					tr *= prob;

					res += sl * min(max(x[s] - x[s - 1] - (ds ? h : 0), 0), h);
					dp[s + 1][t][0][dt] += sl;

					res += sr * min(x[s + 1] - x[s], h);
					int ns = s + 1;
					while(ns < t && x[ns] - x[ns - 1] < h && h == 100) {
						res += sr * min(x[ns + 1] - x[ns], h);
						ns++;
					}
					dp[ns][t][1][dt] += sr;

					res += tl * min(x[t] - x[t - 1], h);
					int nt = t - 1;
					while(nt > s && x[nt + 1] - x[nt] < h && h == 100) {
						res += tl * min(x[nt] - x[nt - 1], h);
						nt--;
					}
					dp[s][nt][ds][1] += tl;

					res += tr * min(max(x[t + 1] - x[t] - (dt ? h : 0), 0), h);
					dp[s][t - 1][ds][0] += tr;

				}
			}
		}
	}

	cout << fixed << setprecision(9) << res << '\n';

	return 0;
}
