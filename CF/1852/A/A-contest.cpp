#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) cin >> a;

		auto survives = [&](int64_t cnt) -> bool {
			int64_t cur_days = K;
			for (int i = N; i > 0 && cur_days > 0; ) {
				assert(cnt > 0);
				if (A[i-1] > cnt) {
					i--;
					continue;
				}
				int64_t num_days = (cnt - A[i-1] + i) / i;
				num_days = std::min(num_days, cur_days);
				cnt -= num_days * i;
				cur_days -= num_days;
				if (cnt <= 0) return false;
			}
			assert(cnt > 0);
			return true;
		};

		int64_t mi = 0;
		int64_t ma = int64_t(N) * K + 1;

		while (ma - mi > 1) {
			int64_t md = (mi + ma) / 2;
			if (survives(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		cout << ma << '\n';
	}

	return 0;
}
