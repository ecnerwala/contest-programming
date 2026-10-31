#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

// vector of length M+1
std::vector<int> solve_rows(int N, int M, std::vector<std::string> G) {
	std::vector<int> max_height(M+1, N);
	std::vector<std::pair<int, int>> by_height(M);
	std::vector<std::pair<int, int>> nxt_by_height; nxt_by_height.reserve(M);
	std::vector<int> nxt(M+1);
	std::vector<int> prv(M+1);
	std::vector<int> num_width(M+1);
	std::vector<int> pref_blocked(M+1);

	for (int z = 0; z < 2; z++, std::reverse(G.begin(), G.end())){
		// decreasing by height
		for (int j = 0; j < M; j++) {
			by_height[j] = {0, j};
		}

		for (int r = 0; r < N; r++) {
			{
				for (auto [h, j] : by_height) {
					if (G[r][j] == '.') {
						nxt_by_height.push_back({h+1, j});
					}
				}
				for (auto [h, j] : by_height) {
					if (G[r][j] == '#') {
						nxt_by_height.push_back({0, j});
					}
				}
				std::swap(by_height, nxt_by_height);
				nxt_by_height.clear();
			}

			for (int j = 0; j < M; j++) {
				pref_blocked[j+1] = pref_blocked[j] + (G[r][j] == '.' && (r+1==N || G[r+1][j] == '#'));
			}
			int num_blocked = pref_blocked[M];

			if (num_blocked == 0) continue;

			std::fill(num_width.begin(), num_width.end(), 0);
			num_width[0] = num_blocked;

			std::iota(nxt.begin(), nxt.end(), 0);
			std::iota(prv.begin(), prv.end(), 0);

			int cur_width = 0;
			for (auto [h, j] : by_height) {
				// merge j and j+1
				int p = prv[j];
				int n = nxt[j+1];
				assert(p != -1 && n != -1);
				assert(nxt[p] == j && prv[n] == j+1);

				if (pref_blocked[j] > pref_blocked[p]) num_width[j-p]--;
				if (pref_blocked[n] > pref_blocked[j+1]) num_width[n-(j+1)]--;

				if (pref_blocked[j+1] > pref_blocked[j]) num_width[0]--;

				nxt[p] = n;
				prv[j] = -1;
				nxt[j+1] = -1;
				prv[n] = p;
				if (pref_blocked[n] > pref_blocked[p]) num_width[n-p]++;

				while(num_width[cur_width] == 0) {
					cur_width++;
					setmin(max_height[cur_width], h);
				}
			}
			assert(num_width[M] == 1);
			assert(cur_width == M);
		}
	}
	return max_height;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::string> G(N);
	for (auto& r : G) { cin >> r; assert(int(r.size()) == M); }

	std::vector<int> max_height = solve_rows(N, M, G);

	std::vector<std::string> G2(M, std::string(N, '0'));
	for (int j = 0; j < M; j++) {
		for (int i = 0; i < N; i++) {
			G2[j][i] = G[i][j];
		}
	}
	std::vector<int> max_width = solve_rows(M, N, G2);
	assert(int(max_width.size()) == N+1);
	for (int i = 0; i < N; i++) {
		assert(max_width[i] >= max_width[i+1]);
		for (int j = max_width[i]; j > max_width[i+1]; j--) {
			setmin(max_height[j], i);
		}
	}

	int ans = std::accumulate(max_height.begin()+1, max_height.end(), 0);
	cout << ans << '\n';

	return 0;
}
