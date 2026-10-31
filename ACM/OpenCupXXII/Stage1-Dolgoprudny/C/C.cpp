#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;

void chk_min(ll& x, ll y){
	x = min(x, y);
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int n;
	ll horiz_cost;
	cin >> n >> horiz_cost;
	vector<int> h(n);
	vector<ll> vert_cost(n);
	for(int i = 0; i < n; i++){
		cin >> h[i] >> vert_cost[i];
	}
	vector<int> hsum = {0};
	for(int i = 0; i < n; i++){
		hsum.push_back(hsum.back() + h[i]);
	}

	int P = hsum.back();

	vector<vector<int> > nright(n, vector<int>(P, -1));
	vector<vector<int> > nleft(n, vector<int>(P, -1));
	for(int i = 1; i < n; i++){
		nleft[i] = nleft[i-1];
		for(int j = 0; j < h[i-1]; j++){
			nleft[i][j] = i-1;
		}
	}
	for(int i = n-1; i > 0; i--){
		nright[i-1] = nright[i];
		for(int j = 0; j < h[i]; j++){
			nright[i-1][j] = i;
		}
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < P; j++){
			cerr << nright[i][j] << ' ';
		}
		cerr << '\n';
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < P; j++){
			cerr << nleft[i][j] << ' ';
		}
		cerr << '\n';
	}

	const ll V = 1e18;

	vector<vector<vector<ll> > > ddp(n+1, vector<vector<ll> > (n+1, vector<ll>(P, V)));
	vector<vector<vector<ll> > > ldp(n+1, vector<vector<ll> > (n+1, vector<ll>(P, V)));
	vector<vector<vector<ll> > > rdp(n+1, vector<vector<ll> > (n+1, vector<ll>(P, V)));

	vector<vector<vector<ll> > > udp(n+1, vector<vector<ll> > (n+1, vector<ll>(P, V)));
	for(int r = 0; r <= n; r++){
		for(int l = r; l >= 0; l--){
			for(int cur = l; cur < r; cur++){
				for(int bh = 0; bh < h[cur]; bh++){
					int id = hsum[cur] + bh;
					if(bh > 0){
						ll below = hsum[r] - hsum[l] - h[cur] + bh;
						chk_min(ddp[l][r][id], ddp[l][r][id-1] + vert_cost[cur] * below * (P - below));
					} else if(r == l+1){
						chk_min(ddp[l][r][id], 0ll);
					}
					{
						int nj = nright[cur][bh];
						// if(l == 0 && r == 5){
						// 	cerr << "nj " << nj << "cur " << cur << ' ' << id << '\n';
						// }
						if(nj >= l && nj < r){
							for(int mid = cur+1; mid <= nj; mid++){
								ll below = hsum[r] - hsum[mid];
								chk_min(ddp[l][r][id], ddp[l][mid][id] + ldp[mid][r][hsum[nj] + bh] + horiz_cost * below * (P - below));
								// if(ddp[l][mid][id] + ldp[mid][r][hsum[nj] + bh] + horiz_cost * below * (P - below) == ddp[l][r][id] && l == 0 && r == 5){
								// 	cerr << "cur " << cur << ' ' << mid << ' ' << id << '\n';
								// }
							}
						}
					}
					{
						int nj = nleft[cur][bh];
						if(nj >= l && nj < r){
							for(int mid = nj+1; mid <= cur; mid++){
								ll below = hsum[mid] - hsum[l];
								chk_min(ddp[l][r][id], ddp[mid][r][id] + rdp[l][mid][hsum[nj] + bh] + horiz_cost * below * (P - below));
							}
						}
					}
					for(int mid = cur+1; mid <= r; mid++){
						chk_min(ldp[l][r][id], ddp[l][mid][id] + udp[mid][r][id]);
					}
					for(int mid = l; mid <= cur; mid++){
						chk_min(rdp[l][r][id], udp[l][mid][id] + ddp[mid][r][id]);
					}
				}
			}
			for(int cur = 0; cur < n; cur++){
				if(cur >= l && cur < r) continue;
				for(int bh = h[cur]-1; bh >= 0; bh--){
					int id = hsum[cur] + bh;
					{
						ll below = hsum[r] - hsum[l] + h[cur] - (bh + 1);
						if(bh + 1 < h[cur]){
							chk_min(udp[l][r][id], udp[l][r][id + 1] + vert_cost[cur] * below * (P - below));
						} else if(l == r){
							chk_min(udp[l][r][id], 0);
						}
					}
					if(cur < l){
						int nj = nright[cur][bh];
						if(nj >= l && nj < r){
							for(int mid = nj+1; mid <= r; mid++){
								ll below = (hsum[mid] - hsum[l]);
								chk_min(udp[l][r][id], ldp[l][mid][hsum[nj] + bh] + udp[mid][r][id] + horiz_cost * below * (P - below));
							}
						}
					} else if(cur >= r){
						int nj = nleft[cur][bh];
						if(nj >= l && nj < r){
							for(int mid = l; mid <= nj; mid++){
								ll below = (hsum[r] - hsum[mid]);
								chk_min(udp[l][r][id], udp[l][mid][id] + rdp[mid][r][hsum[nj] + bh] + horiz_cost * below * (P - below));
							}
						}
					}
				}
			}
			// cerr << "test " << l << ' ' << r << '\n';
			// cerr << "u ";
			// for(int i = 0; i < P; i++){
			// 	cerr << udp[l][r][i] << ' ';
			// }
			// cerr << '\n';
			// cerr << "l ";
			// for(int i = 0; i < P; i++){
			// 	cerr << ldp[l][r][i] << ' ';
			// }
			// cerr << '\n';
			// cerr << "r ";
			// for(int i = 0; i < P; i++){
			// 	cerr << rdp[l][r][i] << ' ';
			// }
			// cerr << '\n';
			// cerr << "d ";
			// for(int i = 0; i < P; i++){
			// 	cerr << ddp[l][r][i] << ' ';
			// }
			// cerr << '\n';
		}
	}
	ll ans = V;
	for(int cur = 0; cur < n; cur++){
		int id = hsum[cur] + h[cur] - 1;
		ans = min(ans, ddp[0][n][id]);
	}
	cout << ans << '\n';
}
