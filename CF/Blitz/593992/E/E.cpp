#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t A, B, C, D; cin >> A >> B >> C >> D;
	assert(1 <= A && A <= B && B <= C && C <= D);
	D = std::min(D, B + C - 1);

	int64_t RZ = D - C;
	int64_t RY = C - B;
	// sum_{a=0..RZ}_{b=0..RY} B - max(A, RZ + RY)
	
	int64_t ans = 0;
	for (int64_t s = 0; s <= RY + RZ; s++) {
		int64_t cnt_s = std::min<int64_t>({1 + s, RY+1, RZ+1, RY + RZ + 1 - s});
		// x > RY + RZ
		// x >= A
		// x <= B
		ans += int64_t(cnt_s) * std::max(B - std::max({A, s + 1}) + 1, int64_t(0));
	}
	cout << ans << '\n';

	return 0;
}
