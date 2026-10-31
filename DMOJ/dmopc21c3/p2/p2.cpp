#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t K; cin >> K;
	const int64_t N = int(1e6)+1;
	for (int64_t n0 = 1, n1 = N; n0 <= N && n1 >= 0; ) {
		int64_t c = (n0 * (n0 - 1) / 2) + (n1 * (n1 - 1) / 2);
		if (c == K) {
			if (n0 + n1 <= N) {
				// we're good
				assert(n0 >= 1);
				std::vector<int> A; A.reserve(n0 + n1 - 1);
				for (int i = 0; i < n0-1; i++) {
					A.push_back(0);
				}
				for (int i = 0; i < n1; i++) {
					A.push_back(i == 0 ? 1 : 0);
				}
				cout << A.size() << '\n';
				for (int z = 0; z < int(A.size()); z++) {
					cout << A[z] << " \n"[z+1==int(A.size())];
				}
				exit(0);
			} else {
				n1--;
			}
		} else if (c < K) {
			n0++;
		} else {
			n1--;
		}
	}
	cout << -1 << '\n';
	exit(0);

	return 0;
}
