#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, M, X, Y, A, B, C, D; cin >> N >> M >> X >> Y >> A >> B >> C >> D;

	int64_t ans = X * max<int64_t>(0, A - N) + Y * max<int64_t>(0, B - M);
	for (int i = 1; N + i * C < A + C; i++) {
		ans = min(ans, X * max<int64_t>(0, A - (N + i * C)) + Y * max<int64_t>(0, B + i * D - M));
	}
	for (int i = 1; M + i * D < B + D; i++) {
		ans = min(ans, X * max<int64_t>(0, A + i * C - N) + Y * max<int64_t>(0, B - (M + i * D)));
	}
	cout << ans << '\n';

	return 0;
}
