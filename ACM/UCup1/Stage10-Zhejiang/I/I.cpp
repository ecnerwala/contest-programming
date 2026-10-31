#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> ranges(N);
	for (auto& [l, r] : ranges) { cin >> l >> r; l--, r--; }

	std::vector<int> ans(N);
	for (int dir = 0; dir < 2; dir++) {
		// reverse everything
		for (auto& [l, r] : ranges) {
			std::swap(l, r);
			l = 2*N-1-l;
			r = 2*N-1-r;
		}

		std::vector<int> idx(2*N);
		std::vector<int> is_start(2*N);
		for (int i = 0; i < N; i++) {
			idx[ranges[i][0]] = i;
			is_start[ranges[i][0]] = true;
			idx[ranges[i][1]] = i;
			is_start[ranges[i][1]] = false;
		}

		std::vector<int> val(N, -1);
		std::vector<int> jmp(N, -1);

		for (int x_left = 0, x_right; x_left < 2*N; x_left = x_right) {
			{
				assert(is_start[x_left]);
				x_right = x_left + 1;
				int cnt = 1;
				while (x_right == x_left || cnt != 0) {
					cnt += (is_start[x_right] ? +1 : -1);
					x_right++;
				}
			}

			if (dir == 0) {
				// add the step 0 cost
				int cnt = (x_right - x_left) / 2 - 1;
				for (int x = x_left; x < x_right; x++) {
					if (is_start[x]) ans[idx[x]] += cnt;
				}
			}

			int best_left = -1;
			for (int x = x_right - 1; x >= x_left; x--) {
				if (!is_start[x]) {
					if (best_left == -1 || ranges[idx[x]][0] < ranges[best_left][0]) {
						best_left = idx[x];
					}
				} else {
					assert(best_left != -1);
					jmp[idx[x]] = best_left;
				}
			}

			{
				int num_ends = 0;
				for (int x = x_left; x < x_right; x++) {
					if (is_start[x]) {
						int i = idx[x];
						if (jmp[i] == i) {
							assert(x == x_left);
							assert(num_ends == 0);
							val[i] = num_ends;
							ans[i] += val[i];
						} else {
							val[i] = val[jmp[i]] + num_ends;
							ans[i] += val[i];
						}
					} else {
						num_ends++;
					}
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
