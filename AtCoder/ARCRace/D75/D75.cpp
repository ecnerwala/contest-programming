#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	ll A, B; cin >> A >> B; A -= B;
	vector<ll> H(N);
	for (int i = 0; i < N; i++) cin >> H[i];

	ll mi = 0, ma = 0;
	for (int i = 0; i < N; i++) ma += (H[i] + B - 1) / B;
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		ll cnt = 0;
		for (int i = 0; i < N; i++) {
			cnt += (max(0ll, H[i] - B * md) + A - 1) / A;
		}
		if (cnt <= md) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';

	return 0;
}
