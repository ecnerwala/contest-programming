#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 3e3;

int N;

ll X[MAXN];
ll S, M, P;

ll dp[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> S >> M >> P;
	for(int i = 0; i < N; i++) {
		cin >> X[i];
	}
	dp[0] = 0;
	for(int i = 1; i <= N; i++) {
		ll sum_hi = 0, sum_lo = 0;
		priority_queue<ll, vector<ll>, less<ll>> lo;
		priority_queue<ll, vector<ll>, greater<ll>> hi;
		ll mid = X[i-1];
		dp[i] = dp[i - 1];
		for(int j = i - 2; j >= 0; j--) {
			if(X[j] >= mid) {
				hi.push(X[j]);
				sum_hi += X[j];
				while((lo.size() + 1) * P < hi.size() * M) {
					lo.push(mid);
					sum_lo += mid;
					mid = hi.top();
					hi.pop();
					sum_hi -= mid;
				}
			} else {
				lo.push(X[j]);
				sum_lo += X[j];
				while((hi.size() + 1) * M < lo.size() * P) {
					hi.push(mid);
					sum_hi += mid;
					mid = lo.top();
					lo.pop();
					sum_lo -= mid;
				}
			}
			ll v = dp[j];
			v -= (sum_hi - mid * int(hi.size())) * M;
			v -= (mid * int(lo.size()) - sum_lo) * P;
			v += (i - j - 1) * S;
			dp[i] = max(dp[i], v);
			//cerr << j << ' ' << i << ' ' << mid << ' ' << v - dp[j] << '\n';
		}
		//cerr << dp[i] << '\n';
	}
	cout << dp[N] << '\n';
	return 0;
}
