#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) { cin >> a; }
		struct stk_t {
			int64_t v;
			std::vector<int> min_idx;
		};
		std::vector<stk_t> stk; stk.reserve(N+1);
		stk.push_back({-1, std::vector<int>({-1})});
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			int64_t a = A[i];
			while (stk.size() > 1 && stk.back().v >= a) {
				stk.pop_back();
			}
			assert(!stk.empty());
			std::vector<int> min_idx; min_idx.reserve(1 + stk.back().min_idx.size());
			min_idx.push_back(i);
			min_idx.insert(min_idx.end(), stk.back().min_idx.begin(), stk.back().min_idx.end());
			// 3 is fine
			for (int l = 2; l < int(min_idx.size()) && l <= 3; l++) {
				// we need tgt * l >= a
				// tgt >= a / l
				int64_t tgt = (a + l - 1) / l;
				int mi = 0, ma = int(stk.size());
				while (ma - mi > 1) {
					int md = mi + (ma - mi) / 2;
					if (stk[md].v >= tgt) ma = md;
					else mi = md;
				}
				for (int n = 0; n < int(stk[mi].min_idx.size()); n++) {
					min_idx[l + n] = std::min(min_idx[l + n], stk[mi].min_idx[n]);
					if (min_idx[l + n] == -1) {
						min_idx.resize(l+n+1);
						break;
					}
				}
			}
			for (int l : min_idx) {
				// everything that starts <= min_idx[z] costs +1
				ans += l+1;
			}
			stk.push_back({a, std::move(min_idx)});
		}
		cout << ans << '\n';
	}

	return 0;
}
