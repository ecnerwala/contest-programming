#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int L = int(1e6);
	int64_t ans = 1;

	array<int, 2> N; cin >> N[0] >> N[1];
	array<array<vector<pair<int, int>>, 2>, 2> locs;
	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < N[z]; i++) {
			int p, l, r; cin >> p >> l >> r;
			if (l == 0 && r == L) {
				ans++;
			}

			if (l == 0) {
				locs[z][0].push_back({p, r});
			} else if (r == L) {
				locs[z][1].push_back({p, L-l});
			} else assert(false);
		}
	}

	for (int vx = 0; vx < 2; vx++) {
		for (int vy = 0; vy < 2; vy++) {
			vector<pair<int, int>>& x = locs[0][vx];
			vector<pair<int, int>>& y = locs[1][vy];

			auto do_flips = [&]() {
				if (vx) for (auto& l : y) { l.first = L - l.first; }
				if (vy) for (auto& l : x) { l.first = L - l.first; }
			};
			do_flips();

			vector<tuple<int, bool, int>> evts; evts.reserve(x.size() + y.size());
			for (auto& l : x) {
				evts.push_back({l.first, 0, l.second});
			}
			for (auto& l : y) {
				evts.push_back({l.second, 1, l.first});
			}
			sort(evts.begin(), evts.end(), std::greater());

			vector<int> bit(L+1, 0);
			for (auto [z, t, v] : evts) {
				if (t == 1) {
					// insert
					for (int a = v; a <= L; a |= a+1) {
						bit[a]++;
					}
				} else {
					for (int a = v+1; a; a &= a-1) {
						ans += bit[a-1];
					}
				}
			}

			do_flips();
		}
	}

	cout << ans << '\n';

	return 0;
}

// answer is 1 + # of intersections + # of full lines
