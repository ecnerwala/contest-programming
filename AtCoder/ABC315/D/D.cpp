#include <bits/stdc++.h>

struct color_counter {
	int tot;
	std::array<int, 26> cnt{};
	int num_colors = 0;
	void incr(char c) {
		tot++;
		num_colors += !(cnt[c - 'a']++);
	}
	void decr(char c) {
		tot--;
		num_colors -= !(--cnt[c - 'a']);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int H, W; std::cin >> H >> W;
	std::vector<std::string> G(H);
	for (auto& s : G) std::cin >> s;

	std::vector<color_counter> row_cnt(H);
	std::vector<color_counter> col_cnt(W);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			row_cnt[i].incr(G[i][j]);
			col_cnt[j].incr(G[i][j]);
		}
	}

	std::vector<int> remove_rows; remove_rows.reserve(H);
	std::vector<int> remove_cols; remove_cols.reserve(W);
	while (true) {
		remove_rows.clear();
		for (int i = 0; i < H; i++) {
			auto& a = row_cnt[i];
			if (a.tot >= 2 && a.num_colors == 1) {
				remove_rows.push_back(i);
			}
		}
		remove_cols.clear();
		for (int j = 0; j < W; j++) {
			auto& a = col_cnt[j];
			if (a.tot >= 2 && a.num_colors == 1) {
				remove_cols.push_back(j);
			}
		}
		if (remove_rows.empty() && remove_cols.empty()) break;
		auto remove = [&](int i, int j) -> void {
			if (G[i][j] == ' ') return;
			row_cnt[i].decr(G[i][j]);
			col_cnt[j].decr(G[i][j]);
			G[i][j] = ' ';
		};
		for (int i : remove_rows) {
			for (int j = 0; j < W; j++) remove(i, j);
		}
		for (int j : remove_cols) {
			for (int i = 0; i < H; i++) remove(i, j);
		}
	}
	int ans = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			ans += G[i][j] != ' ';
		}
	}
	cout << ans << '\n';

	return 0;
}
