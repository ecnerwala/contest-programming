#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N_pts; cin >> N_pts;
		int Y0; cin >> Y0;
		std::vector<std::pair<int, int>> pts(N_pts);
		for (int i = 0; i < N_pts; i++) {
			cin >> pts[i].first >> pts[i].second;
			pts[i].second = std::abs(pts[i].second - Y0);
		}
		std::sort(pts.begin(), pts.end());

		std::vector<int> Xs; Xs.reserve(N_pts);
		std::vector<std::vector<int>> Ys; Ys.reserve(N_pts);
		for (int i = 0, j = 0; i < N_pts; i = j) {
			int x = pts[i].first;
			Xs.push_back(x);

			j = i;
			while (j < N_pts && pts[j].first == x) j++;

			Ys.push_back(std::vector<int>(j - i));
			for (int k = i; k < j; k++) {
				Ys.back()[k-i] = pts[k].second;
			}
		}

		int L = int(Xs.size());
		std::vector<int> ans(2*L+1);
		for (int i = 0; i < L; i++) {
			ans[2*i+1] += int(Ys[i].size());
		}

		for (int rev = 0; rev < 2; rev++) {
			std::reverse(Xs.begin(), Xs.end());
			for (auto& x : Xs) x = -x;
			std::reverse(Ys.begin(), Ys.end());

			std::reverse(ans.begin(), ans.end());

			// Now, solve the sweep
			std::set<int> alive;
			ans[0] += 0;
			for (int i = 0; i < L; i++) {
				ans[2*i+1] += int(alive.size());
				// insert all of yourself
				for (int z = 0; z < int(Ys[i].size()); z++) {
					int v = Ys[i][z] - Xs[i];
					if (z + 1 < int(Ys[i].size()) && Ys[i][z] == Ys[i][z+1]) {
						// kill but don't insert
						z++;
						alive.erase(v);
					} else {
						auto it = alive.find(v);
						if (it != alive.end()) alive.erase(it);
						else alive.insert(v);
					}
				}
				ans[2*i+2] += int(alive.size());
			}
		}

		int mi = ans[0];
		int ma = ans[0];
		for (int i = 0; i < 2 * L + 1; i++) {
			if (i % 2 == 0 && i != 0 && i != 2 * L && Xs[i/2] - Xs[i/2-1] == 1) {
				continue;
			}
			mi = std::min(mi, ans[i]);
			ma = std::max(ma, ans[i]);
		}

		cout << mi << ' ' << ma << '\n';
	}

	return 0;
}
