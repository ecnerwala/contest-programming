#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2000;

int N;

int par[MAXN];
int ch[MAXN];
int val[MAXN];
int sz[MAXN];
int sum[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> par[i] >> val[i];
		par[i] --;
		if(i) ch[par[i]] ++;
		sz[i] = 1;
		sum[i] = val[i];
	}

	for(int t = 0; t < N; t++) {
		for(int i = 1; i < N; i++) {
			if(ch[i]) continue;
			int p = par[i];
			sum[p] += sum[i];
			sz[p] += sz[i];
			ch[p] --;
			ch[i] = 1;
		}
	}

	double res = 0;
	for(int i = 0; i < N; i++) {
		res = max(res, double(sum[i]) / sz[i]);
	}
	cout << fixed << setprecision(3) << res << '\n';
	return 0;
}
