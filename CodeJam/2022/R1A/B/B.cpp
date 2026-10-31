#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		assert(N >= 30);
		std::vector<int> A(2*N);
		for (int i = 0; i < 30; i++) {
			A[i] = (1 << i);
		}
		for (int i = 30; i < N; i++) {
			A[i] = int(1e9) - (N - 1 - i);
		}
		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
		cout << flush;
		for (int i = 0; i < N; i++) {
			cin >> A[N+i];
		}
		int64_t tot = std::accumulate(A.begin(), A.end(), int64_t(0));
		assert((tot % 2) == 0);
		int64_t tgt = tot / 2;
		std::vector<int> res; res.reserve(2*N);
		for (int i = 2*N-1; tgt >= (1 << 30); i--) {
			assert(tgt >= A[i]);
			res.push_back(A[i]);
			tgt -= A[i];
		}
		for (int i = 0; i < 30; i++) {
			if (tgt & (1 << i)) res.push_back(1 << i);
		}
		for (int i = 0; i < int(res.size()); i++) {
			cout << res[i] << " \n"[i+1==int(res.size())];
		}
		cout << flush;
	}

	return 0;
}
