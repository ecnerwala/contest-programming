#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	struct trie_t {
		std::array<int, 26> c{0,};
		int cnt = 0;
	};
	std::vector<trie_t> trie;
	trie.reserve(2e6);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N; cin >> N;
		trie.clear();
		trie.emplace_back();
		//trie.back().c.fill(0);
		//trie.back().cnt = 0;

		for (int z = 0; z < N; z++) {
			int M; cin >> M;
			std::vector<int> nodes(M);
			nodes[0] = 0;
			trie[nodes[0]].cnt++;
			for (int i = 1; i < M; i++) {
				int p; char c; cin >> p >> c;
				p--;
				c -= 'a';
				assert(0 <= p && p < i);

				int cur = nodes[p];
				if (trie[cur].c[c] == 0) {
					int idx = int(trie.size());
					trie[cur].c[c] = idx;
					trie.emplace_back();
					//trie.back().c.fill(0);
					//trie.back().cnt = 0;
				}
				int nxt = trie[cur].c[c];
				nodes[i] = nxt;
				trie[nxt].cnt++;
			}
		}

		auto choose3 = [](int64_t a) -> int64_t {
			return a * (a-1) * (a-2) / 6;
		};

		int64_t ans = 0;
		for (const auto& n : trie) {
			int c = n.cnt;
			ans += choose3(N) - choose3(N - c);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
