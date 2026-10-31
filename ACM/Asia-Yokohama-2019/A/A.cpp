#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	ll T; cin >> T;
	T++;

	ll ans = -1;
	while (T >= 5) {
		ans += 2; T -= 2;
		ans += T % 3;
		T -= T % 3;
		T /= 3;
	}
	ans += T;
	cout << ans << '\n';

	return 0;
}
