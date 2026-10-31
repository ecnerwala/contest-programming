#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	struct pt_t {
		int x, y, c;
	};
	vector<pt_t> pts(N);
	for (auto& pt : pts) { cin >> pt.x >> pt.y >> pt.c; pt.c--; }

	// sort them by x coordinate
	sort(pts.begin(), pts.end(), [&](const auto& a, const auto& b) { return a.x < b.x; });

	const int S = 1 << 19;
	auto is_good = [&](int L) -> bool {
		struct seg_t {
			int lazy = 0;
			bool has = true;
		};
		vector<seg_t> seg(2*S);
		auto update_node = [&](int a) {
			assert(seg[a].lazy >= 0);
			if (seg[a].lazy) seg[a].has = false;
			else if (a >= S) {
				seg[a].has = true;
			} else {
				seg[a].has = seg[2*a].has || seg[2*a+1].has;
			}
		};
		auto update_range = [&](int l, int r, int v) -> void {
			l += L+1;
			if (l >= r) return;
			assert(0 <= l && l < r && r <= S);
			for (int a = S+l, b = S+r; a < b; a /= 2, b /= 2) {
				if (a & 1) {
					seg[a].lazy += v;
					update_node(a);
					a++;
				}
				if (b & 1) {
					--b;
					seg[b].lazy += v;
					update_node(b);
				}
			}
			for (int a = S+l; a; a /= 2) {
				update_node(a);
			}
			for (int b = S+r-1; b; b /= 2) {
				update_node(b);
			}
		};
		vector<set<pair<int, int>>> alive(K);
		for (int k = 0; k < K; k++) {
			alive[k].insert({-L-1, -1});
			alive[k].insert({S, -2});
			update_range(-L-1, S, +1);
		}
		for (int i = 0, j = 0; i < N; i++) {
			for (; j < i && pts[j].x < pts[i].x - L; j++) {
				// erase point j
				int k = pts[j].c;
				auto it = alive[k].find({pts[j].y, j});
				assert(it != alive[k].end() && next(it) != alive[k].end());
				assert(it != alive[k].begin());
				update_range(prev(it)->first, it->first, -1);
				update_range(it->first, next(it)->first, -1);
				update_range(prev(it)->first, next(it)->first, +1);
				alive[k].erase(it);
			}
			// insert point i
			{
				int k = pts[i].c;
				auto it = alive[k].insert({pts[i].y, i}).first;
				assert(it != alive[k].end() && next(it) != alive[k].end());
				assert(it != alive[k].begin());
				update_range(prev(it)->first, next(it)->first, -1);
				update_range(prev(it)->first, it->first, +1);
				update_range(it->first, next(it)->first, +1);
			}
			if (seg[1].has) return true;
		}
		return false;
	};

	int mi = -1;
	int ma = 1 << 18; // bigger than all coordinates
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (is_good(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}

	cout << ma << '\n';

	return 0;
}
