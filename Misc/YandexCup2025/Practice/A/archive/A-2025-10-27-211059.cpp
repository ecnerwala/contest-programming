#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t A, B, C; cin >> A >> B >> C;
	int64_t N; cin >> N;
	const int64_t INF = 1e18;
	int64_t mi = 0, ma = INF+1;
	while (ma - mi > 1) {
		int64_t md = (mi + ma) / 2;
		int64_t cnt = md / std::lcm(A, B) + md / std::lcm(B, C) + md / std::lcm(A, C) - (md / std::lcm(A, std::lcm(B, C))) * 3;
		if (cnt >= N) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << (ma > INF ? -1 : ma) << '\n';

	return 0;
}
