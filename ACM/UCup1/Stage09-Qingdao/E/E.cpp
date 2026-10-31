#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N; int64_t M; cin >> N >> M;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;

		std::vector<int64_t> needs(N);

		std::vector<std::pair<int64_t, int64_t>> dp_right(N);
		std::vector<std::pair<int64_t, int64_t>> dp_left(N);
		auto is_good = [&](int64_t tgt) -> bool {
			assert(tgt > 0);
			for (int i = 0; i < N; i++) {
				needs[i] = (tgt + A[i] - 1) / A[i];
				assert(needs[i] > 0);
			}

			dp_left[0] = {1, 1};
			for (int i = 0; i+1 < N; i++) {
				auto [num_in, num_ops] = dp_left[i];
				auto num_out = std::max<int64_t>(0, needs[i] - num_in);
				dp_left[i+1] = {num_out + 1, std::min(M+1, num_ops + num_out + num_out + 1)};
			}

			dp_right[N-1] = {0, 0};
			for (int i = N-1; i > 0; i--) {
				auto [num_in, num_ops] = dp_right[i];
				auto num_out = std::max<int64_t>(1, needs[i] - num_in);
				dp_right[i-1] = {num_out, std::min(M+1, num_ops + num_out + num_out)};
			}

			for (int i = 0; i < N; i++) {
				// arrive at i
				int64_t cnd = std::min(M+1, dp_left[i].second + dp_right[i].second) + 2 * std::max<int64_t>(0, needs[i] - dp_left[i].first - dp_right[i].first);
				if (cnd <= M) return true;
			}
			return false;
		};

		int64_t mi = 0;
		int64_t ma = (*std::min_element(A.begin(), A.end())) * M + 1;

		while (ma - mi > 1) {
			int64_t md = (mi + ma) / 2;
			if (is_good(md)) {
				mi = md;
			} else {
				ma = md;
			}
		}
		cout << mi << '\n';
	}

	return 0;
}
