#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, std::array<int, 2>>> A(2*N);
	for (auto& a : A) {
		int x, y; cin >> x >> y;
		if (x > y) std::swap(x, y);
		a = {x+y, {x,y}};
	}
	std::sort(A.begin(), A.end());
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		ans += A[i+N].second[1] - A[i].second[0];
	}
	cout << ans << '\n';

	return 0;
}
