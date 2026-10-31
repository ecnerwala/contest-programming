#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::array<std::vector<int>, 2> inds;
		inds[0].reserve(N);
		inds[1].reserve(N);
		std::array<std::vector<int>, 2> vals;
		vals[0].reserve(N);
		vals[1].reserve(N);
		for (int i = 0; i < N; i++) {
			int z = A[i] & 1;
			inds[z].push_back(i);
			vals[z].push_back(A[i]);
		}
		std::vector<int> B(N);
		for (int z = 0; z < 2; z++) {
			std::sort(vals[z].begin(), vals[z].end());
			for (int i = 0; i < int(inds[z].size()); i++) {
				B[inds[z][i]] = vals[z][i];
			}
		}
		bool good = std::is_sorted(B.begin(), B.end());
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
