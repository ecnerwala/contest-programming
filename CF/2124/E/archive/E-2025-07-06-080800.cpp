#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
		int64_t tot = std::accumulate(A.begin(), A.end(), int64_t(0));
		if (tot & 1) {
			cout << -1 << '\n';
			continue;
		}
		int64_t cur_sum = 0;
		int mid = -1;
		while (cur_sum < tot / 2) {
			mid++;
			cur_sum += A[mid];
		}
		assert(cur_sum >= tot / 2);
		if (cur_sum == tot / 2) {
			cout << 1 << '\n';
			for (int i = 0; i < N; i++) {
				cout << A[i] << " \n"[i+1==N];
			}
			continue;
		}
		assert(cur_sum > tot / 2);
		assert(mid != -1);
		if (A[mid] * 2 > cur_sum) {
			cout << -1 << '\n';
			continue;
		}
		std::vector<int64_t> B(N);
		int64_t cnt = cur_sum - tot / 2;
		for (int i = 0; i < mid; i++) {
			int64_t v = std::min(A[i], cnt);
			B[i] += v;
			B[mid] += v;
			A[i] -= v;
			A[mid] -= v;
			cnt -= v;
		}
		assert(cnt == 0);
		cout << 2 << '\n';
		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
		for (int i = 0; i < N; i++) {
			cout << B[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
