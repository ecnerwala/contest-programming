#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e6;
ll N;
int A[MAXN];

ll M, K, L;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	//cin >> N;
	cin >> N >> M;
	//cin >> N >> M >> K;
	//cin >> N >> M >> K >> L;
	//cin >> N; for (int i = 0; i < N; i++) { cin >> A[i]; }
	
	ll ans = min(M * !!(N % 3), N * !!(M % 3));
	for (int z = 0; z < 2; z++, swap(N, M)) {
		for (int i = 1; i < N; i++) {
			vector<ll> v({M * i, M / 2 * (N-i), (M - M / 2) * (N-i)});
			sort(v.begin(), v.end());
			ans = min(ans, v[2]-v[0]);
		}
	}

	cout << ans << '\n';

	return 0;
}
