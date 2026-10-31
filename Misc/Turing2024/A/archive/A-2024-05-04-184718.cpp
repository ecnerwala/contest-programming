#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; cin >> S;
	struct trie_node {
		std::array<int, 26> ch{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		int own_lis = 0;
		int subtree_lis = 0;
	};
	std::vector<trie_node> nodes;
	nodes.reserve(N * 2 + 2);
	nodes.push_back(trie_node{});

	std::vector<std::vector<std::pair<int, int>>> to_insert(N+1);

	int ans = 0;
	for (int x = 0; x < N; x++) {
		for (auto [px, v] : to_insert[x]) {
			int cur = 0;
			for (int i = px; i < x; i++) {
				setmax(nodes[cur].subtree_lis, v);
				int nxt = nodes[cur].ch[S[i] - 'a'];
				if (!nxt) {
					nxt = nodes[cur].ch[S[i] - 'a'] = int(nodes.size());
					nodes.push_back(trie_node{});
				}
				cur = nxt;
			}
			setmax(nodes[cur].subtree_lis, v);
			setmax(nodes[cur].own_lis, v);
		}
		{
			int cur = 0;
			int own_val = 0;
			for (int j = x+1; j <= N; j++) {
				int c = S[j-1] - 'a';
				setmax(own_val, nodes[cur].own_lis);
				for (int b = 0; b < c; b++) {
					if (nodes[cur].ch[b]) {
						setmax(own_val, nodes[nodes[cur].ch[b]].subtree_lis);
					}
				}
				setmax(ans, own_val+1);
				to_insert[j].push_back({x, own_val+1});
				if (!nodes[cur].ch[c]) break;
				cur = nodes[cur].ch[c];
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
