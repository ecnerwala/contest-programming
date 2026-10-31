#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }

		auto do_op = [&](int l, int r) -> std::vector<int> {
			assert(0 <= l && l < r && r <= N);
			std::vector<int> Q(N);
			auto it = Q.begin();
			it = std::copy(P.begin() + r, P.end(), it);
			it = std::copy(P.rend() - r, P.rend() - l, it);
			it = std::copy(P.begin(), P.begin() + l, it);
			return Q;
		};

		int idx_max = int(std::find(P.begin(), P.end(), N-1) - P.begin());
		std::vector<int> opt = do_op(0, idx_max + 1);
		for (int i = 0; i <= N; i++) {
			if (i < idx_max) opt = std::max(opt, do_op(i, idx_max));
			if (i < idx_max+1) opt = std::max(opt, do_op(i, idx_max+1));
			if (idx_max < i) opt = std::max(opt, do_op(idx_max, i));
			if (idx_max+1 < i) opt = std::max(opt, do_op(idx_max+1, i));
			if (i+1 <= N) {
			opt = std::max(opt, do_op(i, i+1));
			}
		}
		for (int i = 0; i < N; i++) {
			cout << opt[i]+1 << " \n"[i+1==N];
		}
	}

	return 0;
}
