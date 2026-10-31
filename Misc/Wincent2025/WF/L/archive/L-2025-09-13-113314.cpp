#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	assert(N == 100);
	int ans = 0;
	for (int i = -2; i < 13; i++) {
		for (int j = -2; j < 13; j++) {
			if (std::gcd(i, j) == 1) ans++;
		}
	}
	cerr << ans << '\n';

	return 0;
}
