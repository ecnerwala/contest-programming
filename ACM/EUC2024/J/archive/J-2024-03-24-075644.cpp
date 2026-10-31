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

	int R, C; cin >> R >> C;
	std::array<std::vector<std::string>, 2> G;
	for (int z = 0; z < 2; z++) {
		G[z].resize(R);
		for (auto& r : G[z]) cin >> r;
	}

	std::vector<std::array<int, 2>> path;
	bool has_overlap = [&]() -> bool {
		for (int r = 0; r < R; r++) {
			for (int c = 0; c < C; c++) {
				if (G[0][r][c] == '*' && G[1][r][c] == '*') {
					path = {{r, c}};
					return true;
				}
			}
		}
		return false;
	}();
	if (!has_overlap) {
		std::vector<std::vector<bool>> vis(R, std::vector<bool>(C, false));
		bool has_path = [&]() -> bool {
			for (int r = 0; r < R; r++) {
				for (int c = 0; c < C; c++) {
					if (G[0][r][c] == '*') {
						if (std::y_combinator([&](auto self, std::array<int, 2> cur, std::array<int, 2> prv) -> bool {
							assert(!vis[cur[0]][cur[1]]);
							vis[cur[0]][cur[1]] = true;
							if (G[1][cur[0]][cur[1]] == '*') {
								path.push_back(cur);
								return true;
							}
							for (auto [dx, dy] : {std::array<int, 2>{0, -1}, {0, 1}, {-1, 0}, {1, 0}}) {
								std::array<int, 2> nxt{cur[0] + dx, cur[1] + dy};
								if (0 <= nxt[0] && nxt[0] < R && 0 <= nxt[1] && nxt[1] < C && G[0][nxt[0]][nxt[1]] == '.' && !vis[nxt[0]][nxt[1]]) {
									if (self(nxt, cur)) {
										path.push_back(cur);
										return true;
									}
								}
							}
							return false;
						})(std::array<int, 2>{r, c}, std::array<int, 2>{r, c})) return true;
					}
				}
			}
			return false;
		}();
		if (!has_path) {
			cout << "NO" << '\n';
			exit(0);
		}
	}
	assert(!path.empty());
	std::reverse(path.begin(), path.end());
	std::array<std::array<int, 2>, 2> starts;
	starts[0] = path.front();
	starts[1] = path.back();
	std::array<std::vector<std::array<int, 2>>, 2> bfs;
	for (int z = 0; z < 2; z++) {
		assert(G[z][starts[z][0]][starts[z][1]] == '*');
		std::vector<std::string> vis = G[z];
		vis[starts[z][0]][starts[z][1]] = '.';
		bfs[z].push_back(starts[z]);
		for (int t = 0; t < int(bfs[z].size()); t++) {
			std::array<int, 2> cur = bfs[z][t];
			for (auto [dx, dy] : {std::array<int, 2>{0, -1}, {0, 1}, {-1, 0}, {1, 0}}) {
				std::array<int, 2> nxt{cur[0] + dx, cur[1] + dy};
				if (0 <= nxt[0] && nxt[0] < R && 0 <= nxt[1] && nxt[1] < C && vis[nxt[0]][nxt[1]] == '*') {
					vis[nxt[0]][nxt[1]] = '.';
					bfs[z].push_back(nxt);
				}
			}
		}
	}
	assert(int(bfs[0].size()) == int(bfs[1].size()));

	std::reverse(bfs[0].begin(), bfs[0].end());
	std::vector<std::array<int, 2>> ops;

	if (!has_overlap) {
		assert(path.size() >= 2);
		bfs[0].insert(bfs[0].end(), path.begin() + 1, path.end() - 1);
		bfs[1].insert(bfs[1].begin(), path.begin() + 1, path.end() - 1);
		assert(int(bfs[0].size()) == int(bfs[1].size()));
		for (int i = 0; i < int(bfs[0].size()); i++) {
			ops.push_back(bfs[1][i]);
			ops.push_back(bfs[0][i]);
		}
	} else {
		assert(path.size() == 1);
		std::vector<std::string> cur_G = G[0];
		assert(int(bfs[0].size()) == int(bfs[1].size()));
		auto ins_it = bfs[1].begin();
		auto del_it = bfs[0].begin();
		while (ins_it != bfs[1].end()) {
			{
				auto cur = *ins_it;
				++ins_it;
				if (cur_G[cur[0]][cur[1]] == '*') {
					cur_G[cur[0]][cur[1]] = 'A';
					// already inserted
					continue;
				} else if (cur_G[cur[0]][cur[1]] == '.') {
					cur_G[cur[0]][cur[1]] = '*';
					ops.push_back(cur);
					// keep going
				} else assert(false);
			}
			while (true) {
				assert(del_it != bfs[0].end());
				auto cur = *del_it;
				++del_it;
				if (cur_G[cur[0]][cur[1]] == '*') {
					// delete it
					cur_G[cur[0]][cur[1]] = '.';
					ops.push_back(cur);
					break;
				} else if (cur_G[cur[0]][cur[1]] == 'A') {
					// skipped
					continue;
				} else assert(false);
			}
		}
	}

	cout << "YES" << '\n';
	assert(int(ops.size()) % 2 == 0);
	cout << ops.size()/2 << '\n';
	for (int i = 0; i < int(ops.size()); i += 2) {
		auto ins = ops[i], del = ops[i+1];
		cout << del[0]+1 << ' ' << del[1]+1 << ' ' << ins[0]+1 << ' ' << ins[1]+1 << '\n';
	}

	return 0;
}
