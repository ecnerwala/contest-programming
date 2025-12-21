#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << ((i+j == N-1) ? (i&1) : (i+j<N ? 0 : 1));
		}
		cout << '\n';
	}

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
