#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int, int>> A(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i].first;
			A[i].second = i;
		}
		std::sort(A.begin(), A.end(), std::greater<>());
		auto is_symmetric = [&]() -> bool {
			std::vector<int> freq(N+1);
			for (int i = 0; i < N; i++) {
				freq[A[i].first]++;
			}
			for (int i = N; i > 0; i--) {
				freq[i-1] += freq[i];
			}
			for (int i = 0; i < N; i++) {
				if (freq[i+1] != A[i].first) {
					return false;
				}
			}
			return true;
		};
		if (!is_symmetric()) {
			cout << "NO" << '\n';
			continue;
		}
		std::vector<int> B(N);
		int cur = N;
		for (int i = 0, j = N; i < j; ) {
			if (A[i].first < j) {
				B[A[j-1].second] = -cur;
				cur--;
				j--;
			} else {
				B[A[i].second] = +cur;
				cur--;
				i++;
			}
		}
		cout << "YES" << '\n';
		for (int i = 0; i < N; i++) {
			cout << B[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
