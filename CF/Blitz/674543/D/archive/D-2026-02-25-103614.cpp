#include <bits/stdc++.h>

int main() {
	int N; std::cin >> N;
	std::vector<int> ans;
	ans.reserve(N);
	int tot = 1;
	for (int i = 1; i < N; i++) {
		if (std::gcd(i, N) == 1) {
			ans.push_back(i);
			tot = int64_t(tot) * int64_t(i) % N;
		}
	}
	if (tot == 1) {
	} else if (tot == N-1) {
		ans.pop_back();
	} else assert(false);
	std::cout << ans.size() << '\n';
	for (int i = 0; i < int(ans.size()); i++) std::cout << ans[i] << " \n"[i+1==int(ans.size())];
}
