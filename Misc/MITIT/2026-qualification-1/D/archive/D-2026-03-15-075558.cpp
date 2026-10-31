#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q, K; std::cin >> N >> Q >> K;
		std::vector<uint64_t> A(N); for (auto& a : A) std::cin >> a;

		struct trie_node {
			std::array<int, 2> ch{0, 0};
			int tot_cnt;
			bool is_full;
			int best_val;
		};

		std::vector<trie_node> nodes;
		nodes.reserve(2 + K * (N+Q));
		nodes.push_back(trie_node{{0, 0}, 0, false, 0});
		nodes.push_back(trie_node{{0, 0}, 1, true, 0});
		auto update_node = [&](int n) -> void {
			assert(n > 1);
			auto [l, r] = nodes[n].ch;
			nodes[n].tot_cnt = nodes[l].tot_cnt + nodes[r].tot_cnt;
			nodes[n].is_full = nodes[l].is_full && nodes[r].is_full;
			nodes[n].best_val = nodes[n].is_full ? 0 : std::max(nodes[l].best_val, nodes[r].best_val) + nodes[n].tot_cnt;
		};
		assert(K >= 1);
		int root = 0;
		auto insert_node = [&](this auto&& self, int& n, uint64_t v, int depth) -> void {
			assert(n != 1);
			if (depth == K) {
				assert(n == 0);
				n = 1;
				return;
			}
			int idx = n;
			if (n == 0) {
				idx = n = int(nodes.size());
				nodes.push_back(nodes[0]);
			}
			self(nodes[idx].ch[(v >> depth) & 1], v, depth+1);
			update_node(idx);
		};
		auto delete_node = [&](this auto&& self, int& n, uint64_t v, int depth) -> void {
			assert(n != 0);
			if (depth == K) {
				assert(n == 1);
				n = 0;
				return;
			}
			int idx = n;
			self(nodes[idx].ch[(v >> depth) & 1], v, depth+1);
			update_node(idx);
		};
		for (auto a : A) {
			insert_node(root, a, 0);
		}
		assert(!nodes[root].is_full);
		std::cout << nodes[root].best_val << '\n';
		for (int q = 0; q < Q; q++) {
			int i; uint64_t v; std::cin >> i >> v; i--;
			delete_node(root, A[i], 0);
			A[i] = v;
			insert_node(root, A[i], 0);
			assert(!nodes[root].is_full);
			std::cout << nodes[root].best_val << '\n';
		}
	}

	return 0;
}
