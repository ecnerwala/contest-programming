#include<bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int W, H; cin >> W >> H;

	int N; cin >> N;
	vector<array<array<int, 2>, 2>> rects(N);
	for (auto& r : rects) {
		cin >> r[0][0] >> r[1][0] >> r[0][1] >> r[1][1];
		r[0][0]--;
		r[1][0]--;
	}

	vector<bool> can_use(N, true);

	vector<pair<int, int>> seg_full(2*H);
	vector<pair<int, int>> seg_any(2*H);

	for (int md = 1; md < N; md++) {
		int l = md - (md & -md);
		int r = md + (md & -md);
		r = min(r, N);

		// l..md and md..r
		
		vector<pair<int, int>> lefts; lefts.reserve(md-l);
		for (int i = l; i < md; i++) {
			if (!can_use[i]) continue;
			lefts.emplace_back(rects[i][0][0], i);
		}

		vector<pair<int, int>> rights; rights.reserve(r-md);
		for (int i = md; i < r; i++) {
			if (!can_use[i]) continue;
			rights.emplace_back(rects[i][0][1], i);
		}

		sort(lefts.begin(), lefts.end());
		sort(rights.begin(), rights.end());

		int left_ind = 0;
		for (auto [x, i] : rights) {
			while (left_ind < int(lefts.size()) && lefts[left_ind].first < x) {
				int j = lefts[left_ind].second;

				for (int a = H+rects[j][1][0], b = H+rects[j][1][1]; a < b; a /= 2, b /= 2) {
					if (a & 1) setmax(seg_full[a++], {md, rects[j][0][1]});
					if (b & 1) setmax(seg_full[--b], {md, rects[j][0][1]});
				}
				for (int a = H+rects[j][1][0]; a; a /= 2) {
					setmax(seg_any[a], {md, rects[j][0][1]});
				}
				for (int a = H+rects[j][1][1]-1; a; a /= 2) {
					setmax(seg_any[a], {md, rects[j][0][1]});
				}

				left_ind++;
			}

			pair<int, int> max_inter;
			for (int a = H+rects[i][1][0], b = H+rects[i][1][1]; a < b; a /= 2, b /= 2) {
				if (a & 1) setmax(max_inter, seg_any[a++]);
				if (b & 1) setmax(max_inter, seg_any[--b]);
			}
			for (int a = H+rects[i][1][0]; a; a /= 2) {
				setmax(max_inter, seg_full[a]);
			}
			for (int a = H+rects[i][1][1]-1; a; a /= 2) {
				setmax(max_inter, seg_full[a]);
			}
			if (max_inter > pair<int, int>{md, rects[i][0][0]}) {
				can_use[i] = false;
			}
		}
	}

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		if (can_use[i]) {
			ans += int64_t(rects[i][0][1] - rects[i][0][0]) * int64_t(rects[i][1][1] - rects[i][1][0]);
		}
		cout << ans << '\n';
	}

	return 0;
}
