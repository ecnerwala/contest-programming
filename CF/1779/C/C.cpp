#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		{
			int v; cin >> v;
		}
		int ans = 0;
		for (int z = 0; z < 2; z++) {
			int L = (z ? N-M : M-1);
			std::vector<int> A(L);
			for (auto& a : A) cin >> a;
			if (z == 0) {
				std::reverse(A.begin(), A.end());
				for (auto& a : A) a = -a;
			}

			int64_t cur_sum = 0;
			std::priority_queue<int> vals;
			for (auto a : A) {
				cur_sum += a;
				vals.push(-a);
				if (cur_sum < 0) {
					cur_sum += 2 * vals.top();
					vals.pop();
					ans ++;
				}
				assert(cur_sum >= 0);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
