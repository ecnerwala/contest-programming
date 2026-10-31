#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;

		int64_t tot = 0;
		for (auto a : A) tot += a;
		if (tot < 0) {
			tot = -tot;
			for (auto& a : A) a = -a;
		}
		assert(tot >= 0);

		// start with a -tot, end with a -tot

		int64_t cur_min = 0;
		int64_t cur_max = tot;
		int ans = N;
		auto process = [&](int64_t v) {
			cur_min -= v;
			cur_max -= v;
			if (cur_min > 0) cur_min = 0;
			if (cur_max < 0) cur_max = 0;
			if (cur_max - cur_min > tot) {
				ans++;
				cur_min = -v;
				cur_max = -v;
				if (cur_min > 0) cur_min = 0;
				if (cur_max < 0) cur_max = 0;
				assert(cur_max - cur_min <= tot);
			}
		};
		for (int i = 0; i < N; i++) {
			process(A[i]);
		}
		process(-tot);
		cout << ans << '\n';
	}

	return 0;
}
