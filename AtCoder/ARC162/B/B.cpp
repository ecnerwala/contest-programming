#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }

	std::vector<std::pair<int, int>> ops; ops.reserve(2*N);

	auto do_op = [&](int i, int j) {
		assert(0 <= i && i <= N-2);
		assert(0 <= j && j <= N-2);
		assert(i > j);
		std::rotate(A.begin() + j, A.begin() + i, A.begin() + i + 2);
		ops.push_back({i, j});
	};

	for (int i = 0; i < N-2; i++) {
		if (A[N-1] == i) {
			// move it to the left
			do_op(N-2, N-3);
		}
		int loc = i;
		while (A[loc] != i) {
			loc++;
			assert(loc < N);
		}
		assert(loc <= N-2);
		assert(loc >= i);
		if (loc > i) {
			do_op(loc, i);
		}
	}

	if (A[N-1] != N-1) {
		cout << "No" << '\n';
	} else {
		cout << "Yes" << '\n';
		cout << ops.size() << '\n';
		for (auto [i, j] : ops) {
			cout << i+1 << ' ' << j << '\n';
		}
	}

	return 0;
}
