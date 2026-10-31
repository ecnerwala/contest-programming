#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> A(2*N-1);
		for (auto& a : A) cin >> a;

		auto check_cnd = [&](int cnd) -> std::optional<int> {
			int val = 0;
			bool used = false;
			for (int i = 0, j = 2*N-2; i <= j; ) {
				assert(used == ((i & 1) != (j & 1)));
				if (i == j) {
					// parity
					assert(!used);
					val = cnd - A[i];
					used = true;
					i++;
				} else {
					assert(i < j);
					if (A[i] + A[j] < cnd) {
						if (used) return std::nullopt;
						val = cnd - A[i];
						used = true;
						i++;
					} else if (A[i] + A[j] > cnd) {
						if (used) return std::nullopt;
						val = cnd - A[j];
						used = true;
						j--;
					} else {
						i++, j--;
					}
				}
			}
			assert(used);
			if (val <= 0) return std::nullopt;
			return val;
		};

		auto solve = [&]() -> int {
			std::sort(A.begin(), A.end());

			if (N == 1) {
				return 1;
			}

			assert(N >= 2);
			for (int cnd : {A[0] + A[2*N-3], A[0] + A[2*N-2], A[1] + A[2*N-2]}) {
				auto r = check_cnd(cnd);
				if (r) return *r;
			}
			return -1;
		};

		int ans = solve();
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
