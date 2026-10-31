#include<bits/stdc++.h>
using namespace std;

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N; cin >> N;
	auto coeff = [&](int i) { return (i % 2 == N % 2) ? -1 : 1; };
	i64 ans = 0;
	for (int i = 0; i < N; i++) {
		i64 a; cin >> a; ans += a * coeff(i);
	}

	int Q; cin >> Q;
	while (Q--) {
		int l, r, v; cin >> l >> r >> v; l--, r--;
		if (l % 2 == r % 2) {
			ans += v * coeff(l);
		}
		cout << ans << '\n';
	}

	return 0;
}
