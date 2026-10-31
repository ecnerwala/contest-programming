#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 1e18;

const int MAXN = 1.1e5;
int N;
ll H;
pair<ll, int> P[MAXN];

ll dp1[2][MAXN];
ll dp2[2][MAXN];

ll go() {
	cin >> H >> N;
	for (int i = 0; i < N; i++) {
		cin >> P[i].first >> P[i].second;
		P[i].second --;
	}
	sort(P, P + N);

	for (int i = 0; i <= N; i++) {
		dp1[0][i] = dp1[1][i] = dp2[0][i] = dp2[1][i] = (i == 0 ? 0 : -INF);
	}

	for (int i = 0; i < N; i++) {
		if (P[i].first >= H) break;
		int c = P[i].second;
		for (int z = N; z > 0; z--) {
			dp1[c][z] = max(dp1[c][z], dp1[c][z-1] + z-1);
		}
		for (int z = N; z >= 0; z--) {
			dp1[!c][0] = max(dp1[!c][0], dp1[c][z]);
		}
	}

	for (int i = N-1; i >= 0; i--) {
		if (P[i].first <= H) break;
		int c = P[i].second;
		for (int z = N; z > 0; z--) {
			dp2[c][z] = max(dp2[c][z], dp2[c][z-1] + z-1);
		}
		for (int z = N; z >= 0; z--) {
			dp2[!c][0] = max(dp2[!c][0], dp2[c][z]);
		}
	}


	// for (int c = 0; c < 2; c++) { for (int a = 0; a <= N; a++) { cerr << dp1[c][a] << ' '; } cerr << '\n'; }
	// for (int c = 0; c < 2; c++) { for (int a = 0; a <= N; a++) { cerr << dp2[c][a] << ' '; } cerr << '\n'; }
	ll ans = 0;
	for (int c = 0; c < 2; c++) {
		for (int a = 0; a <= N; a++) {
			for (int b = 0; b <= N; b++) {
				ans = max(ans, ll(a) * ll(b) + dp1[c][a] + dp2[c][b]);
			}
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cerr << "Case #" << t << '\n';
		ll ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
