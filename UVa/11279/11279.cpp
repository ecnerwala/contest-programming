#include <bits/stdc++.h>

const std::vector<std::string> QWERTY_GRID = {
	"`1234567890-=",
	" qwertyuiop[]\\",
	" asdfghjkl;'",
	" zxcvbnm,./",
};
const std::vector<std::string> QWERTY_SHIFT_GRID = {
	"~!@#$%^&*()_+",
	" QWERTYUIOP{}|",
	" ASDFGHJKL:\"",
	" ZXCVBNM<>?",
};
const std::vector<std::string> DVORAK_GRID = {
	"`123qjlmfp/[]",
	" 456.orsuyb;=\\",
	" 789aehtdck-",
	" 0zx,inwvg'",
};
const std::vector<std::string> DVORAK_SHIFT_GRID = {
	"~!@#QJLMFP?{}",
	" $%^>ORSUYB:+|",
	" &*(AEHTDCK_",
	" )ZX<INWVG\"",
};

std::array<double, 128> get_weights(
	const std::vector<std::string>& grid,
	const std::vector<std::string>& shift_grid,
	const std::string& home_row
) {
	std::array<std::array<int, 2>, 128> locs; locs.fill({-1, -1});
	assert(grid.size() == shift_grid.size());
	for (int i = 0; i < int(grid.size()); i++) {
		assert(grid[i].size() == shift_grid[i].size());
		for (int j = 0; j < int(grid[i].size()); j++) {
			for (char c : {grid[i][j], shift_grid[i][j]}) {
				if (c == ' ') continue;
				assert((locs[c] == std::array<int, 2>{-1,-1}));
				locs[c] = {i, j};
			}
		}
	}

	const double INF = 1e99;
	std::array<double, 128> dist; dist.fill(0);
	for (int i = 0; i < 128; i++) {
		if (locs[i] == std::array<int, 2>{-1, -1}) continue;
		dist[i] = INF;
		for (char c : home_row) {
			assert((locs[c] != std::array<int, 2>{-1, -1}));
			dist[i] = std::min(dist[i], 2 * std::hypot(locs[i][0] - locs[c][0], locs[i][1] - locs[c][1]));
		}
	}
	return dist;
}

std::map<char, char> get_shift_map(const std::vector<std::string>& grid, const std::vector<std::string>& shift_grid) {
	std::map<char, char> res;
	assert(grid.size() == shift_grid.size());
	for (int i = 0; i < int(grid.size()); i++) {
		assert(grid[i].size() == shift_grid[i].size());
		for (int j = 0; j < int(grid[i].size()); j++) {
			char a = grid[i][j];
			char b = shift_grid[i][j];
			if ('a' <= a && a <= 'z') assert(b - a == 'A' - 'a');
			res[a] = b;
		}
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	assert(get_shift_map(QWERTY_GRID, QWERTY_SHIFT_GRID) == get_shift_map(DVORAK_GRID, DVORAK_SHIFT_GRID));

	auto w1 = get_weights(QWERTY_GRID, QWERTY_SHIFT_GRID, "asdfjkl;");
	auto w2 = get_weights(QWERTY_GRID, QWERTY_SHIFT_GRID, "fghj");
	auto w3 = get_weights(DVORAK_GRID, DVORAK_SHIFT_GRID, "ehtd");

	string LINE;
	while (std::getline(cin, LINE)) {
		double r1 = 0, r2 = 0, r3 = 0;
		for (char c : LINE) {
			r1 += w1[c];
			r2 += w2[c];
			r3 += w3[c];
		}
		cout << fixed << setprecision(2) << r1 << ' ';
		cout << fixed << setprecision(2) << r2 << ' ';
		cout << fixed << setprecision(2) << r3 << '\n';
	}

	return 0;
}
