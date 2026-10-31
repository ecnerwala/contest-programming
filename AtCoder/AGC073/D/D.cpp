#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K_; cin >> N >> K_;
	const int K = 4;
	assert(K == K_);
	std::array<int, K> W;
	for (auto& w : W) cin >> w;
	struct gem_t { int a; int b; };
	std::vector<gem_t> G(N);
	for (auto& [a, b] : G) { cin >> a >> b; a--; }

	return 0;
}
