#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e6;
int N;
int T;
pair<int, int> P[MAXN];
ll dp[MAXN];

int L = 0;
vector<int> hull[MAXN];

int main() {
	if(fopen("mowing.in", "r")) {
		freopen("mowing.in", "r", stdin);
		freopen("mowing.out", "w", stdout);
	}
	cin >> N >> T;
	for (int i = 0; i < N; i++) {
		cin >> P[i].first >> P[i].second;
	}
	P[N++] = {0, 0};
	P[N++] = {T, T};
	sort(P, P + N);
	N--;
	assert(P[N].first == T);
	assert(P[N].second == T);

	for (int i = 0; i <= N; i++) {
		int mi = 0, ma = L+1;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (P[i].second > P[hull[md].back()].second) {
				mi = md;
			} else {
				ma = md;
			}
		}
		//cerr << i << ' ' << ma << '\n';
		L = max(L, ma);
		hull[ma].push_back(i);
		if (i == 0) {
			dp[i] = 0;
			continue;
		} else {
			assert(mi >= 1);
		}
		// first or last
		{
			int j = hull[mi].back();
			dp[i] = dp[j] + ll(P[i].first - P[j].first) * ll(P[i].second - P[j].second);
		}
		//cerr << i << ' ' << dp[i] << '\n';
		int mi2 = -1, ma2 = int(hull[mi].size()) - 1;
		while (ma2 - mi2 > 1) {
			int md2 = (mi2 + ma2) / 2;
			int j = hull[mi][md2];
			if (P[i].second > P[j].second) {
				dp[i] = min(dp[i], dp[j] + ll(P[i].first - P[j].first) * ll(P[i].second - P[j].second));
				ma2 = md2;
			} else {
				mi2 = md2;
			}
		}
		if (int(hull[mi].size()) - ma2 <= 10000) {
			for (int z = ma2; z < int(hull[mi].size()); z ++) {
				int j = hull[mi][z];
				dp[i] = min(dp[i], dp[j] + ll(P[i].first - P[j].first) * ll(P[i].second - P[j].second));
			}
		}
		//cerr << i << ' ' << dp[i] << '\n';
	}
	cout << dp[N] << '\n';

	return 0;
}
