#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	const int B = 30;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	struct trie_t {
		std::array<int, 2> ch{-1, -1};
		int last_idx = -1;
	};
	std::vector<trie_t> trie; trie.reserve(B * N + 1);
	trie.emplace_back();
	int prv_bad = -1;
	int last_good = -1;
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		int cur = 0;
		for (int b = B-1; b >= 0; b--) {
			int va = ((A[i] >> b) & 1);
			int vk = ((K >> b) & 1);
			if (vk) {
				// could differ here
				int v = trie[cur].ch[va ^ vk ^ 1];
				if (v != -1) {
					prv_bad = std::max(prv_bad, trie[v].last_idx);
				}
			}
			trie[cur].last_idx = i;
			if (trie[cur].ch[va ^ vk] == -1) {
				trie[cur].ch[va ^ vk] = int(trie.size());
				trie.emplace_back();
			}
			cur = trie[cur].ch[va ^ vk];
		}
		last_good = std::max(last_good, trie[cur].last_idx);
		trie[cur].last_idx = i;
		assert(last_good <= i-1);
		assert(prv_bad <= i-1);
		ans += std::max(0, last_good - prv_bad);
	}
	cout << ans << '\n';

	return 0;
}
