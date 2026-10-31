#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t K; cin >> N >> K;
		if (N <= 4) {
			cout << -1 << '\n';
			continue;
		}
		bool is_zero = K == 0;
		if (is_zero) K = 1;
		vector<int64_t> res; res.reserve(N);
		if (N % 2 == 1) {
			if (is_zero) {
				res.push_back(3);
				res.push_back(3);
				res.push_back(4);
				res.push_back(5);
				res.push_back(5);
			} else {
				res.push_back(3);
				res.push_back(3);
				res.push_back(3);
				res.push_back(5);
				res.push_back(5);
			}
		} else {
			if (is_zero) {
				res.push_back(1);
				res.push_back(3);
				res.push_back(3);
				res.push_back(3);
				res.push_back(5);
				res.push_back(5);
			} else {
				res.push_back(1);
				res.push_back(4);
				res.push_back(4);
				res.push_back(4);
				res.push_back(6);
				res.push_back(6);
			}
		}
		res.resize(N, 10);

		for (int i = 0; i < N; i++) cout << res[i] * K << " \n"[i+1==N];
	}

	return 0;
}

// N = 1: always optimal (a / 0)
// N = 2: always optimal (a / b)
// N = 3: always optimal (a / b + c)
// N = 4: always optimal (a / b + c + d or a + d / b + c)
//
// N = 5:
// K = 0:
// 5 5 4 3 3
// 5 4
// 5 3 3
//
// K = 1:
// 5 5 4 3 3
// 5 3 3
// 5 3
//
// N = 6:
// K = 0:
// 5 5 3 3 3 1
// 5 3 3
// 5 3 1
//
// K = 1:
// 6 6 4 4 4 1
// 6 4 1
// 6 4 4
