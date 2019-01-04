#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1.5e6;
int N;
const int MAXM = 1.5e6;
int M;

typedef pair<ll, ll> pll;
typedef pair<int, int> pii;
typedef pair<pll, pair<pii, double>> bus;

bus busses[MAXM];

map<ll, double> dp[MAXN];

ll K;

int main() {
	scanf(" %d %d", &M, &N);
	scanf("%lld", &K);
	for (int i = 0; i < M; i++) {
		int A, B;
		scanf(" %d %d", &A, &B);
		ll s, t;
		scanf(" %lld %lld", &s, &t);
		double p;
		scanf(" %lf", &p);
		busses[i] = {{s, t}, {{A, B}, p}};
	}
	for (int i = 0; i < N; i++) {
		dp[i][K+1] = 0;
	}
	// If we ever get to the airport, we're good.
	dp[1][K+1] = 1;

	sort(busses, busses + M);
	reverse(busses, busses + M);
	for (int i = 0; i < M; i++) {
		ll s = busses[i].first.first;
		ll t = busses[i].first.second;
		int a = busses[i].second.first.first;
		int b = busses[i].second.first.second;
		double p = busses[i].second.second;
		double v = (1-p) * dp[a].upper_bound(s)->second + p * dp[b].upper_bound(t)->second;

		// Something is later and better, just give up
		if (dp[a].lower_bound(s)->second >= v) {
			continue;
		}
		auto it = dp[a].upper_bound(s);
		while (it != dp[a].begin()) {
			it --;
			assert(it->first <= s);
			if (it->second <= v) {
				it = dp[a].erase(it);
			} else {
				break;
			}
		}
		dp[a][s] = v;
	}

	double res = dp[0].begin()->second;
	printf("%.10lf\n", res);

	return 0;
}
