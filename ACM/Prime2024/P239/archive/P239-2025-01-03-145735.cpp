#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;
		cout << ([&]() -> bool {
			std::pair<int, int> m1{0, 0};
			std::pair<int, int> m2{0, 0};
			for (int i = 0; i < N; i++) {
				std::pair<int, int> cnd(std::min(A[i], B[i]), B[i] - A[i]);
				if (cnd > m1) {
					m2 = m1;
					m1 = cnd;
				} else if (cnd > m2) {
					m2 = cnd;
				}
			}
			return m1.second > 0 || m2.second > 0 || (m1.second >= 0 && m1.first != m2.first);
		}() ? "Alice" : "Bob") << '\n';
	}

	return 0;
}
