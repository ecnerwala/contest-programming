#include<bits/stdc++.h>

const int X = int(1e7) + 100;
std::array<std::array<int64_t, X>, 2> delta_slope;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	delta_slope[0].fill(0);
	delta_slope[1].fill(0);
	array<int64_t, 2> v0{0, 0};
	array<int64_t, 2> s0{0, 0};

	for (int i = 0; i < N; i++) {
		array<array<int64_t, 2>, 2> rect;
		for (int p = 0; p < 2; p++) {
			for (int d = 0; d < 2; d++) {
				cin >> rect[d][p];
			}
		}
		for (int d = 0; d < 2; d++) {
			auto r = rect[d];
			assert(0 <= r[0] && r[0] < r[1] && r[1] < X);
			v0[d] += r[0];
			s0[d] -= 1;
			delta_slope[d][r[0]] += 2;
			delta_slope[d][(r[0]+r[1])/2]--;
			delta_slope[d][(r[0]+r[1]+1)/2]--;
			delta_slope[d][r[1]] += 2;
		}
	}

	array<int64_t, 2> best = v0;
	for (int d = 0; d < 2; d++) {
		for (int x = 0; x < X; x++) {
			best[d] = min(best[d], v0[d]);
			s0[d] += delta_slope[d][x];
			v0[d] += s0[d];
		}
		best[d] = min(best[d], v0[d]);
	}
	cout << best[0] + best[1] << '\n';

	return 0;
}
