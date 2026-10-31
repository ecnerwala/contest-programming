#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; p--; }

	int Q; cin >> Q;
	using query_t = std::tuple<int, int, int>;
	std::vector<query_t> queries(Q);
	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int l, r; cin >> l >> r; l--;
			queries[q] = {op, l, r};
		} else if (op == 2) {
			int v, x; cin >> v >> x; v--;
			queries[q] = {op, v, x};
		} else if (op == 3) {
			int i, j; cin >> i >> j; i--, j--;
			queries[q] = {op, i, j};
		} else assert(false);
	}

	const int QUERY_BLOCK = 500;

	std::vector<bool> is_swapped(N);
	std::vector<bool> val_is_swapped(N);
	std::vector<bool> is_updated(N);

	std::vector<bool> is_cut(N+1);
	std::vector<int> block_id(N+1);

	std::vector<int> cc_id(N);
	std::vector<int> cc_dest; cc_dest.reserve(N);

	const int MAX_BLOCKS = QUERY_BLOCK * 2 + 10;
	const int MAX_CHAINS = QUERY_BLOCK * 2 + 10;

	std::vector<int64_t> block_sum; block_sum.reserve(N);

	std::vector<int64_t> cc_weight;
	std::vector<std::vector<int64_t>> block_cc_weights;

	for (int qlo = 0, qhi; qlo < Q; qlo = qhi) {
		qhi = min(Q, qlo + QUERY_BLOCK);

		// GATHER QUERY DATA
		std::fill(is_swapped.begin(), is_swapped.end(), false);
		std::fill(val_is_swapped.begin(), val_is_swapped.end(), false);
		std::fill(is_updated.begin(), is_updated.end(), false);
		std::fill(is_cut.begin(), is_cut.end(), false);

		for (int q = qlo; q < qhi; q++) {
			auto [op, x, y] = queries[q];
			if (op == 3) {
				is_swapped[x] = is_swapped[y] = true;
				val_is_swapped[P[x]] = val_is_swapped[P[y]] = true;
			} else if (op == 1) {
				is_cut[x] = is_cut[y] = true;
			} else if (op == 2) {
				is_updated[x] = true;
			}
		}

		// BUILD THE CCS
		int NUM_CCS = 0;
		std::fill(cc_id.begin(), cc_id.end(), -1);
		cc_dest.clear();

		// First, build up the chains
		for (int i = 0; i < N; i++) {
			if (val_is_swapped[i]) {
				assert(cc_id[i] == -1);
				int cur_cc_id = NUM_CCS++;

				int cur;
				for (cur = i; true; cur = P[cur]) {
					cc_id[cur] = cur_cc_id;
					if (is_swapped[cur]) break;
				}
				cc_dest.push_back(cur);
			}
		}

		// Now we build the cycles
		for (int i = 0; i < N; i++) {
			if (cc_id[i] == -1 && is_updated[i]) {
				// build the cycle starting from i
				int cur_cc_id = NUM_CCS++;

				int cur;
				for (cur = i; true; cur = P[cur]) {
					cc_id[cur] = cur_cc_id;
					if (P[cur] == i) break;
				}
				cc_dest.push_back(cur);
			}
		}

		cc_weight.assign(NUM_CCS, 0);

		// BUILD THE BLOCKS
		is_cut[0] = false;
		is_cut[N] = true;
		block_id[0] = 0;
		for (int i = 1; i <= N; i++) {
			block_id[i] = block_id[i-1] + is_cut[i];
		}
		int NUM_BLOCKS = block_id[N];
		assert(NUM_BLOCKS <= MAX_BLOCKS);

		block_sum.clear();
		block_sum.reserve(NUM_BLOCKS + 1);
		block_sum.push_back(0);

		block_cc_weights.clear();
		block_cc_weights.reserve(NUM_BLOCKS + 1);
		block_cc_weights.emplace_back(NUM_CCS, 0);
		for (int i = 0; i < N; i++) {
			if (i == 0 || is_cut[i]) {
				block_sum.push_back(block_sum.back());
				block_cc_weights.push_back(block_cc_weights.back());
			}
			block_sum.back() += A[i];
			if (cc_id[i] != -1) block_cc_weights.back()[cc_id[i]]++;
		}

		for (int q = qlo; q < qhi; q++) {
			auto [op, x, y] = queries[q];
			if (op == 1) {
				y = block_id[y];
				x = block_id[x];
				int64_t ans = block_sum[y] - block_sum[x];
				const auto& bx = block_cc_weights[x];
				const auto& by = block_cc_weights[y];
				for (int c = 0; c < NUM_CCS; c++) {
					ans += cc_weight[c] * (by[c] - bx[c]);
				}
				cout << ans << '\n';
			} else if (op == 2) {
				x = cc_dest[cc_id[x]];
				for (int cur = x; true; ) {
					cc_weight[cc_id[cur]] += y;

					cur = cc_dest[cc_id[P[cur]]];
					if (cur == x) break;
				}
			} else if (op == 3) {
				std::swap(P[x], P[y]);
			} else assert(false);
		}

		for (int i = 0; i < N; i++) {
			if (cc_id[i] != -1) {
				A[i] += cc_weight[cc_id[i]];
			}
		}
	}

	return 0;
}
