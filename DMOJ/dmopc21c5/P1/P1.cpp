#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	if (N >= 5) {
		std::vector<int> A; A.reserve(N);
		for (int i = 1; i <= N; i += 2) {
			if (i == 5) continue;
			A.push_back(i);
		}
		A.push_back(5);
		A.push_back(4);
		for (int i = N; i >= 1; i--) {
			if (i & 1) continue;
			if (i == 4) continue;
			A.push_back(i);
		}
		assert(int(A.size()) == N);
		for (int i = 0; i < N; i++) cout << A[i] << " \n"[i+1==N];
	} else if (N == 1) {
		cout << 1 << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
