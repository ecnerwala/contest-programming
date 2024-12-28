#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::string> S(N);
		for (auto& s : S) cin >> s;
		int tot_len = 0;
		for (const auto& s : S) tot_len += int(s.size());
		std::vector<std::vector<int>> factors(N+1);
		for (int i = 1; i <= N; i++) {
			for (int j = i; j <= N; j += i) factors[j].push_back(i);
		}
		std::vector<std::array<int, 26>> trie; trie.reserve(tot_len + 1);
		std::vector<int> trie_sz; trie_sz.reserve(tot_len + 1);
		trie.emplace_back(); trie.back().fill(-1);
		trie_sz.push_back(0);

		int ans = 0;
		std::vector<int> vals(N+1, 0);
		for (int i = 0; i < N; i++) {
			int cur = 0;
			for (char c : S[i]) {
				c -= 'a';
				if (trie[cur][c] == -1) {
					trie[cur][c] = int(trie.size());
					trie.emplace_back(); trie.back().fill(-1);
					trie_sz.push_back(0);
				}
				cur = trie[cur][c];
				for (int f : factors[++trie_sz[cur]]) {
					ans ^= vals[f];
					vals[f] += f;
					ans ^= vals[f];
				}
			}
			cout << ans << " \n"[i+1==N];
		}
	}

	return 0;
}
