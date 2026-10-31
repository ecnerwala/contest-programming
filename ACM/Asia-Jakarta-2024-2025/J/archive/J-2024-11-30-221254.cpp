#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> A(N); for (auto& a : A) cin >> a;
	std::vector<int> X(M); for (auto& x : X) cin >> x;

	struct trie_node {
		std::array<int, 2> ch{-1, -1};
		int sz = 0;
	};
	int best_xor = 1 << 30;
	{
		std::vector<trie_node> A_trie; A_trie.reserve(N * 30);
		A_trie.emplace_back();
		for (auto a : A) {
			if (A_trie[0].sz > 0) {
				int cur = 0;
				int msk = 0;
				for (int b = 29; b >= 0; b--) {
					assert(cur != -1);
					int d = (a >> b) & 1;
					if (A_trie[cur].ch[d] == -1) {
						msk |= 1 << b;
						d = !d;
					}
					cur = A_trie[cur].ch[d];
				}
				best_xor = std::min(best_xor, msk);
			}
			{
				int cur = 0;
				for (int b = 29; b >= 0; b--) {
					A_trie[cur].sz++;
					int d = (a >> b) & 1;
					if (A_trie[cur].ch[d] == -1) {
						A_trie[cur].ch[d] = int(A_trie.size());
						A_trie.emplace_back();
					}
					cur = A_trie[cur].ch[d];
				}
				A_trie[cur].sz++;
			}
		}
	}
	int64_t ans = 0;
	{
		std::vector<trie_node> X_trie; X_trie.reserve(N * 30);
		X_trie.emplace_back();
		for (int x : X) {
			{
				int cur = 0;
				for (int b = 29; b >= 0; b--) {
					int d = ((best_xor ^ x) >> b) & 1;
					if ((best_xor >> b) & 1) {
						// can try going the other way
						if (X_trie[cur].ch[!d] != -1) {
							ans += X_trie[X_trie[cur].ch[!d]].sz;
						}
					}
					cur = X_trie[cur].ch[d];
					if (cur == -1) break;
					if (b == 0) {
						ans += X_trie[cur].sz;
					}
				}
			}
			{
				int cur = 0;
				for (int b = 29; b >= 0; b--) {
					X_trie[cur].sz++;
					int d = (x >> b) & 1;
					if (X_trie[cur].ch[d] == -1) {
						X_trie[cur].ch[d] = int(X_trie.size());
						X_trie.emplace_back();
					}
					cur = X_trie[cur].ch[d];
				}
				X_trie[cur].sz++;
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
