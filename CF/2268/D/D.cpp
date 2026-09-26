#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> P(N); for (auto& p : P) { std::cin >> p; p--; }

		std::vector<int> par(N, -1);
		std::vector<int> first_child(N, -1);
		std::vector<int> num_children(N, 0);
		{
			std::vector<std::pair<int, int>> stk; stk.reserve(N);
			for (int i = 0; i < N; i++) {
				int v = P[i];
				while (!stk.empty() && v > stk.back().first) {
					par[stk.back().second] = i;
					first_child[i] = stk.back().second;
					num_children[i]++;
					stk.pop_back();
				}
				stk.push_back({v, i});
			}
		}

		std::vector<bool> is_spine(N);
		for (int i = N-1; i >= 0; i--) {
			if (par[i] == -1) is_spine[i] = true;
			else if (i == first_child[par[i]] && is_spine[par[i]]) is_spine[i] = true;
		}

		int64_t ans = 0;

		for (int l = 0, r = 0; l < N; l = r+1) {
			r = l;
			while (par[r] != -1) r = par[r];
			// We have a spine from l..r inclusive
			for (int i = l; i != r; i = par[i]) {
				// use this spine edge
				ans += int64_t(i-l+1) * int64_t(r-i);

				// take a final step downwards
				ans += int64_t(i-l+1) * int64_t(par[i] - i - 1);

				// take initial 2 steps upwards, but sometimes 1
				ans += int64_t(r - par[i]) * int64_t(2 * (par[i] - i - 1) - (num_children[par[i]] - 1));

				// Now, we have just the within case
				// we have par[i] - i things in here
				int num_1 = par[i] - i - 1;
				int64_t num_2 = 0;
				for (int b = i+1; b <= par[i]; b++) {
					if (first_child[b] == -1) continue;
					num_2 += b - first_child[b] - num_children[b];
				}
				for (int a = i+1; a < par[i]; a++) {
					num_2 += par[a] - a - 1;
				}
				int64_t num_3 = int64_t(par[i] - i) * int64_t(par[i] - i - 1) / 2;
				num_3 -= num_1;
				num_3 -= num_2;
				ans += num_1 + 2 * num_2 + 3 * num_3;
			}

		}

		// Moving left
		ans += int64_t(N) * int64_t(N-1) / 2;
		std::cout << ans << '\n';
	}

	return 0;
}
