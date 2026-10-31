#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<uint64_t>> val(N, std::vector<uint64_t>(N));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i) val[i][j] += val[i-1][j] + 1;
			if (j) val[i][j] += val[i][j-1] + 1;
		}
	}
	std::array<uint64_t, 2> ans{0, 0};
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int d = ((i+j == N-1) ? (i&1) : (i+j<N ? 0 : 1));
			ans[d] += val[i][j] + val[N-1-i][N-1-j];
			cout << d;
		}
		cout << '\n';
	}
	assert(ans[0] == ans[1]);

	return 0;
}

// 
// 0-0 == 1-1
// (0-1 + 0-0) + (0-0 + 1-0) == (0-1 + 1-1) + (1-1 + 1-0)
// start 0 + end 0 == start 1 + end 1
//
//
// start at (i, j) has (i+1 + j+1 choose i+1) - 2 ways
// N = 3:
// 0 1 2
// 1 4 8
// 2 8 18
// 18 9 4
// 9 8 9
// 4 9 18
