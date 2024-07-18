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
			A[i].second = i+1;
		}
		std::vector<std::array<int, 2>> res(N);
		std::vector<int> buckets(N);
		for (int i = N-1; i >= 1; i--) {
			assert(int(A.size()) == i+1);
			std::fill(buckets.begin(), buckets.begin() + i, -1);
			for (int j = 0; true; j++) {
				assert(j < int(A.size()));
				if (buckets[A[j].first % i] == -1) {
					buckets[A[j].first % i] = A[j].second;
				} else {
					res[i] = {buckets[A[j].first % i], A[j].second};
					A.erase(A.begin() + j);
					break;
				}
			}
		}
		cout << "YES" << '\n';
		for (int i = 1; i <= N-1; i++) {
			cout << res[i][0] << ' ' << res[i][1] << '\n';
		}
	}

	return 0;
}
