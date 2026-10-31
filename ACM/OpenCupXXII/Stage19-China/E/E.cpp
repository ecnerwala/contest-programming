#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		auto read_card = [&]() -> int {
			string s; cin >> s;
			char v = s[0];
			if ('2' <= v && v <= '9') {
				// 0, 1, 2, 3, 4, 5, 6, 7
				return v - '2';
			} else if (v == 'T') {
				return 8;
			} else if (v == 'J') {
				return 9;
			} else if (v == 'Q') {
				return 10;
			} else if (v == 'K') {
				return 11;
			} else if (v == 'A') {
				return 12;
			} else assert(false);
		};
		std::vector<int> A(N); for (auto& a : A) a = read_card();
		std::vector<int> B(M); for (auto& b : B) b = read_card();
		int P = read_card();

		cout << ([&]() -> bool {
			std::sort(A.begin(), A.end());
			std::sort(B.begin(), B.end());

			if (N == 1) return false;
			if (A[0] >= P) return false;
			if (B[0] >= P) return false;
			assert(B[0] < P);
			if (M == 1) {
				for (int a : A) {
					if (a >= B[0] && a < P) {
						return true;
					}
				}
				return false;
			}
			assert(M >= 2);
			if (B[1] < P) return true;

			assert(B[1] >= P);

			if (B.back() >= A.back()) {
				// Bob will win his last second to last trick, then lead his small
				return false;
			}
			assert(A.back() > B.back());
			// Alice must cover B.back(), Alice must lead something to give Bob control, and then Alice must thread the needle
			if (N <= 3) return false;
			int num_low = 0;
			int num_needle = 0;
			for (int a : A) {
				if (a < P) {
					num_low ++;
					if (a >= B[0]) num_needle ++;
				}
			}
			if (num_low < 2) return false;
			if (num_needle == 0) return false;
			return true;
		}() ? "Pang" : "Shou") << '\n';
	}

	return 0;
}
