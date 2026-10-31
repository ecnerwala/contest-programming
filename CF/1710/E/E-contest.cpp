#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> A(N); for (auto& a : A) cin >> a;
	std::vector<int> B(M); for (auto& b : B) cin >> b;
	const int A_init_val = A[0];
	const int B_init_val = B[0];
	std::sort(A.begin(), A.end());
	std::sort(B.begin(), B.end());

	auto alice_wins = [&](int v) -> bool {
		int A_beats = int(std::upper_bound(A.begin(), A.end(), v - B_init_val) - A.begin());
		int B_beats = int(std::upper_bound(B.begin(), B.end(), v - A_init_val) - B.begin());

		int A_init = int(std::upper_bound(A.begin(), A.end(), v - B[B_beats]) - A.begin());
		int B_init = int(std::upper_bound(B.begin(), B.end(), v - A[A_beats]) - B.begin());

		// can alice force sum <= v
		// First, find the value of the overall min cut
		std::vector<std::array<int, 2>> coords; coords.reserve(N+M+1);
		for (int i = 0, j = M; i < N || j > 0; ) {
			coords.push_back({i, j});
			if (i < N && (j == 0 || A[i] + B[j-1] <= v)) {
				i++;
			} else {
				j--;
			}
		}
		coords.push_back({N, 0});
		assert(int(coords.size()) == N+M+1);

		int64_t cur_cut = 0;
		for (int z = 0; z < N+M; z++) {
			if (coords[z][0] < coords[z+1][0]) {
				cur_cut += coords[z][1];
			}
		}

		int64_t min_cut = cur_cut;
		bool in_min_cut = false;

		int it = N+M;
		for (int z = 0; z < N+M; ) {
			if (coords[z+1][0] == coords[z][0]) {
				// we need to increment j
				while (coords[it][0] != coords[it-1][0]) it--;
				assert(coords[it][1] == M-coords[z][1] && coords[it-1][1] == M-coords[z+1][1]);
				cur_cut += N - coords[it][0];
				cur_cut -= coords[z][0];
				it--;
				z++;
			} else {
				// don't change cur_cut
				z++;
			}

			int cur_i = coords[z][0];
			int cur_j = M-coords[z][1];
			if (cur_cut < min_cut) {
				min_cut = cur_cut;
				in_min_cut = false;
			}
			if (cur_cut == min_cut) {
				// now, check this location
				// is A_init, B_init within this cut
				if ((A_init < cur_i) || (B_init < cur_j)) in_min_cut = true;
			}
		}
		return in_min_cut;
	};

	// Bob can force > mi
	int mi = -1;
	// Alice can force <= ma
	int ma = A_init_val + B_init_val;

	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (alice_wins(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}

	cout << ma << '\n';

	return 0;
}
