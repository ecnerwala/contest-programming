#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	std::vector<std::pair<int, int>> locs(2*N);
	for (int i = 0; i < 2*N; i++) {
		cin >> locs[i].first;
		locs[i].second = i;
	}

	std::sort(locs.begin(), locs.end());

	N *= 2;

	std::vector<int> inv_loc(N);
	for (int i = 0; i < N; i++) {
		inv_loc[locs[i].second] = i;
	}

	const int B = 500;
	assert(B % 2 == 0);

	int K = int(N+B-1) / B;
	std::vector<int> block_sum(N);
	std::vector<std::vector<int64_t>> block_vals(K);
	auto recompute_block = [&](int k) -> void {
		assert(0 <= k && k < K);
		int lo = k * B;
		int hi = std::min(N, (k+1) * B);
		int len = std::min(lo, hi);
		assert(len % 2 == 0);
		// from -len to len
		block_vals[k].assign(2 * len + 1, int64_t(0));

		int cur_sum = 0;
		for (int i = lo; i < hi; i++) {
			// if we moved towards 0, then we're adding locs[i].first
			// otherwise we're subtracting locs[i].first
			if (locs[i].second >= N) {
				cur_sum ++;
				// if (initial_sum + cur_sum <= 0) ans += locs[i].first
				// if (initial_sum <= -cur_sum) ans += locs[i].first;
				block_vals[k][0] += 2 * locs[i].first;
				block_vals[k][len - cur_sum + 1] -= 2 * locs[i].first;
			} else {
				cur_sum --;
				// if (initial_sum + cur_sum >= 0) ans += locs[i].first
				// if (initial_sum >= -cur_sum) ans += locs[i].first
				block_vals[k][0] -= locs[i].first;
				block_vals[k][len - cur_sum] += 2 * locs[i].first;
			}
		}
		for (int z = 0; z < 2*len; z++) block_vals[k][z+1] += block_vals[z][k];
		block_sum[k] = cur_sum;
	};
	for (int k = 0; k < K; k++) {
		recompute_block(k);
	}

	auto get_ans = [&]() -> int64_t {
		int cur_sum = 0;
		int64_t ans = 0;
		for (int k = 0; k < K; k++) {
			int lo = k * B;
			int hi = std::min(N, (k+1) * B);
			int len = std::min(lo, hi);
			ans += block_vals[k][std::min(std::max(cur_sum, -len), len) + len];
			cur_sum += block_sum[k];
		}
		return ans;
	};

	cout << get_ans() << '\n';
	for (int q = 0; q < Q; q++) {
		int u, v; cin >> u >> v; u--, v--; v += N;
		int a = inv_loc[u], b = inv_loc[v];
		std::swap(locs[a].second, locs[b].second);
		std::swap(inv_loc[u], inv_loc[v]);
		recompute_block(a / B);
		if (a / B != b / B) recompute_block(b / B);
		cout << get_ans() << '\n';
	}

	return 0;
}
