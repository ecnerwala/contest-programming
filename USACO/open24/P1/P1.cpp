#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	struct node {
		std::array<int, 2> c{0, 0};
		int cost = 0;
	};
	std::vector<node> trie;
	trie.reserve(int(2e6));
	trie.push_back(node{});

	int N; cin >> N;
	while (N--) {
		std::string S; cin >> S;
		int cur = 0;
		for (char c : S) {
			if (!trie[cur].c[c - '0']) {
				trie[cur].c[c - '0'] = int(trie.size());
				trie.push_back(node{});
			}
			cur = trie[cur].c[c - '0'];
		}
		trie[cur].cost++;
	}

	auto update_cost = [&](int cur) -> void {
		if (trie[cur].c[0] == 0 && trie[cur].c[1] == 0) {
			// assume it has 1
			trie[cur].cost = 2;
		} else {
			trie[cur].cost = 1 + std::min(trie[cur].c[0] ? trie[trie[cur].c[0]].cost : 0, trie[cur].c[1] ? trie[trie[cur].c[1]].cost : 0);
		}
	};

	int ans = 0;
	for (int cur = int(trie.size()) - 1; cur >= 0; cur--) {
		int cnt = std::exchange(trie[cur].cost, 0);
		if (trie[cur].c[0] == 0 && trie[cur].c[1] == 0) {
			assert(cnt > 0);
			cnt--;
		}
		update_cost(cur);
		while (cnt--) {
			std::y_combinator([&](auto self, int v) -> void {
				if (trie[v].c[0] == 0 && trie[v].c[1] == 0) {
					ans += 2;
					trie[v].c[0] = int(trie.size());
					trie.push_back(node{{0, 0}, 2});
					trie[v].c[1] = int(trie.size());
					trie.push_back(node{{0, 0}, 2});
				} else if (trie[v].c[0] == 0) {
					ans += 1;
					trie[v].c[0] = int(trie.size());
					trie.push_back(node{{0, 0}, 2});
				} else if (trie[v].c[1] == 0) {
					ans += 1;
					trie[v].c[1] = int(trie.size());
					trie.push_back(node{{0, 0}, 2});
				} else {
					if (trie[trie[v].c[0]].cost <= trie[trie[v].c[1]].cost) {
						ans += 1;
						self(trie[v].c[0]);
					} else {
						ans += 1;
						self(trie[v].c[1]);
					}
				}
				update_cost(v);
			})(cur);
		}
	}
	cout << ans << '\n';

	return 0;
}
