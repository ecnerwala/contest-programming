#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int H, W; cin >> H >> W;

		int V = H*W;

		std::vector<bool> is_black(V);
		for (int i = 0; i < H; i++) {
			std::string S; cin >> S;
			for (int j = 0; j < W; j++) {
				assert(S[j] == '0' || S[j] == '1');
				is_black[i*W+j] = S[j] == '0';
			}
		}
		std::vector<int> nxt(V);
		for (int i = 0; i < H; i++) {
			std::string S; cin >> S;
			for (int j = 0; j < W; j++) {
				int n = i * W + j;
				char c = S[j];
				if (c == 'U') n -= W;
				else if (c == 'D') n += W;
				else if (c == 'L') n -= 1;
				else if (c == 'R') n += 1;
				else assert(false);
				nxt[i*W+j] = n;
			}
		}

		struct node_t {
			int st;
			int idx;
			int len;
		};
		std::vector<node_t> cyc_idx(V, {-1, -1, -1});
		std::vector<std::vector<bool>> can_black(V);
		for (int v = 0; v < V; v++) {
			[&](this auto&& self, int cur, int depth) -> void {
				if (cyc_idx[cur].st != -1) {
				} else if (cyc_idx[cur].len != -1) {
					cyc_idx[cur].len = depth - cyc_idx[cur].len;
					can_black[cur].resize(cyc_idx[cur].len);
					cyc_idx[cur].idx = 0;
					cyc_idx[cur].st = cur;
				} else {
					cyc_idx[cur].len = depth;
					self(nxt[cur], depth+1);
					cyc_idx[cur] = cyc_idx[nxt[cur]];
					cyc_idx[cur].idx++;
					if (cyc_idx[cur].idx == cyc_idx[cur].len) cyc_idx[cur].idx = 0;
				}
				if (is_black[cur]) can_black[cyc_idx[cur].st][cyc_idx[cur].idx] = true;
			}(v, 0);
		}
		int n_all = 0, n_black = 0;
		for (const auto& v : can_black ) {
			for (bool b : v) {
				n_all++;
				n_black += b;
			}
		}
		cout << n_all << ' ' << n_black << '\n';
	}

	return 0;
}
