#include<bits/stdc++.h>

struct query_t {
	int t, id, val;
};
int solve(int N, int M, std::vector<query_t> Q) {
	std::vector<std::array<int, 2>> person_bounds(N);
	std::vector<int> observations(M);
	std::vector<int> start_cnt(M);
	std::vector<int> end_cnt(M);

	auto is_good = [&](int K) -> bool {
		assert(K <= int(Q.size()));
		fill(observations.begin(), observations.end(), -1);
		fill(person_bounds.begin(), person_bounds.end(), std::array<int, 2>{M, -1});
		for (int k = 0; k < K; k++) {
			auto [t, id, val] = Q[k];
			if (observations[t] == -1) {
				observations[t] = val;
			} else if (observations[t] != val) {
				return false;
			}

			person_bounds[id][0] = std::min(person_bounds[id][0], t);
			person_bounds[id][1] = std::max(person_bounds[id][1], t);
		}

		fill(start_cnt.begin(), start_cnt.end(), 0);
		fill(end_cnt.begin(), end_cnt.end(), 0);
		for (auto [st, en] : person_bounds) {
			if (st > en) continue;
			start_cnt[st]++;
			end_cnt[en]++;
		}

		int num_ops = 0;

		int cur_val = 0;
		int cur_normal = 0;
		int cur_special = 0;
		for (int t = 0; t < M; t++) {
			if (observations[t] != -1) {
				int v = observations[t];
				if (v > cur_val) {
					int num_open = v - cur_val;
					num_ops += num_open;
					if (num_ops > N) return false;
					cur_normal += num_open;
				} else {
					int num_close = cur_val - v;
					if (num_close <= cur_special) {
						cur_special -= num_close;
					} else {
						num_close -= cur_special;
						cur_special = 0;
						if (num_close <= cur_normal) {
							cur_normal -= num_close;
						} else {
							return false;
						}
					}
				}
				cur_val = v;
			}

			{
				int v = start_cnt[t];
				if (v <= cur_normal) {
					cur_normal -= v;
				} else {
					v -= cur_normal;
					cur_normal = 0;
					if (v <= cur_special) {
						num_ops += v;
						if (num_ops > N) return false;
						cur_special -= v;
					} else {
						return false;
					}
				}
			}

			{
				int v = end_cnt[t];
				cur_special += v;
			}
		}

		return true;
	};

	int mi = 0;
	int ma = int(Q.size()+1);
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return mi;
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::vector<query_t> Q(M);
		for (auto& q : Q) {
			std::cin >> q.t >> q.id >> q.val;
			q.t--, q.id--, q.val++;
		}
		std::cout << solve(N, M, std::move(Q)) << '\n';
	}

	return 0;
}
