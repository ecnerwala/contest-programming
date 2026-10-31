#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Q; cin >> Q;
	while (Q--) {
		using ll = long long;
		ll N, A, B, T; cin >> N >> A >> B >> T;
		A += B;
		T += B * N;
		ll ans = (T + A - 1) / A;
		if (ans > N) cout << -1 << '\n';
		else cout << ans << '\n';
	}

	return 0;
}
