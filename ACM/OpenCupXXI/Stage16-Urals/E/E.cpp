#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int R, C; cin >> R >> C;
	vector<string> G(R); for (auto& s : G) { s.reserve(C); cin >> s; }
	/*
	int R = 1e6;
	int C = 5;
	vector<string> G(R, string(C, '.'));
	*/

	// rows in decreasing order of how far right
	vector<pair<int, int>> rows(R); for (int i = 0; i < R; i++) rows[i] = {i, 0};

	vector<int> row_id(R);
	struct bound_t {
		int mi;
		int ma;
		int msk;
	};
	vector<array<int, 2>> vs(R);

	int ans = -1;

	for (int j = C-1; j >= 0; j--) {
		{
			int sz = 0;
			for (int i = 0; i < int(rows.size()); i++) {
				if (G[rows[i].first][j] == '.') {
					rows[i].second++;
					rows[sz++] = rows[i];
				}
			}
			for (int r = 0; r < R; r++) {
				if (G[r][j] == '#') {
					rows[sz++] = {r, 0};
				}
			}
			assert(sz == R);
		}

		for (int i = 0; i < R; ) {
			//bounds[i] = {R, -1, 0};
			vs[i] = {R, -1};
			if (G[i][j] == '#') {
				row_id[i] = -1;
				i++;
			} else {
				int v = i;
				for (; i < R && G[i][j] == '.'; i++) row_id[i] = v;
			}
		}

		for (auto [r, l] : rows) {
			if (l <= 1) break;
			assert(l >= 2);
			bound_t b{vs[row_id[r]][0] & ((1 << 20) - 1), vs[row_id[r]][1], vs[row_id[r]][0] >> 20};
			if (b.mi > b.ma) {
				vs[row_id[r]] = {r, r};
				continue;
			}

			bound_t nb{min(b.mi, r), max(b.ma, r), 0};
			auto process_pt = [&](int a) -> void {
				if (a == nb.mi || a == nb.ma) return;
				nb.msk |= 1 << ((a - nb.mi > 1) + (nb.ma - a > 1) * 2);
			};
			process_pt(r);
			process_pt(b.mi);
			process_pt(b.ma);

			if (b.msk & 1) {
				process_pt(b.mi+1);
			}
			if (b.msk & 2) {
				process_pt(b.ma-1);
			}
			if (b.msk & 4) {
				process_pt(b.mi+1);
			}
			if (b.msk & 8) {
				nb.msk |= 8;
			}

			b = nb;
			vs[row_id[r]] = {b.mi + (b.msk << 20), b.ma};

			if (b.msk & 8) {
				ans = max(ans, 3 * (l-1) + b.ma - b.mi + 1);
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
