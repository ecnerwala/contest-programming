#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> S(N);
	for (auto& a : S) cin >> a;

	int64_t cur_val = 0;
	for (auto a : S) cur_val += a;

	for (int i = 0, j = 0; i < N; i++) {
		while (cur_val > 0) {
			cur_val -= 2 * S[j % N];
			j++;
		}
		assert(cur_val <= 0);
		assert(j > i);

		int64_t ans = min(-cur_val, cur_val + 2 * S[(j-1)%N]);

		cout << ans << " \n"[i+1==N];

		cur_val += 2 * S[i];
	}

	return 0;
}
