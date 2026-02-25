#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	int N, M, K; std::cin >> N >> M >> K;
	std::vector<int> A(N);
	for (auto& a : A) std::cin >> a;
	std::reverse(A.begin(), A.end());
	int cur = 0;
	int ans = 0;
	for (auto a : A) {
		if (cur < a) {
			if (M == 0) break;
			M--;
			cur = K;
		}
		if (cur < a) break;
		cur -= a;
		ans++;
	}
	std::cout << ans << '\n';
}
