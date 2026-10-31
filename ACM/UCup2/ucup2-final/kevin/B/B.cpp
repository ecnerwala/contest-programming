#include <bits/stdc++.h>
using namespace std;

template <typename T> void set_min(T& a, const T& b){
	if(b < a) a = b;
}
template <typename T> void set_max(T& a, const T& b){
	if(b > a) a = b;
}

using ll = int64_t;

void solve(){
	int N;
	cin >> N;
	vector<ll> A(N);
	for(int i = 0; i < N; i++) cin >> A[i];
	vector<ll> p(N+1);
	for(int i = 0; i < N; i++){
		p[i+1] = p[i] + A[i];
	}
	// cc p;
	int L;
	{
		vector<ll> x = p;
		sort(x.begin(), x.end());
		x.erase(unique(x.begin(), x.end()), x.end());
		L = (int)x.size();
		for(int i = 0; i <= N; i++){
			p[i] = lower_bound(x.begin(), x.end(), p[i]) - x.begin();
		}
	}
	int INF = 1e8;
	vector<int> dp(L, -INF);
	vector<int> dp2(L, -INF);
	if(p[0] > p[N]){
		cout << "No" << '\n';
		return;
	}
	for(int i = 0; i <= N; i++){
		int cdp = -INF;
		if(i == 0){
			cdp = 0;
		} else {
			cdp = dp2[p[i]] - 1;
		}
		for(int x = (int)p[i] + 1; x < L; x++){
			dp[x] += 1;
		}
		for(int x = 0; x+1 < L; x++){
			set_max(dp[x+1], dp[x]);
		}
		for(int x = 0; x <= L; x++){
			if(i+1 <= N && x <= p[i]){
				if(p[i+1] <= x){
					set_max(dp2[max(x, (int)p[i+1])], dp[x]);
				}
			}
		}
		for(int x = 0; x+1 < L; x++){
			set_max(dp2[x+1], dp2[x]);
		}
		for(int x = 0; x < p[i]; x++){
			dp2[x] += 1;
		}
		set_max(dp2[p[i]], cdp);

		if(i == N){
			cout << "Yes" << '\n';
			ll ans = N - cdp;
			cout << ans << '\n';
			return;
		}
	}
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}