#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, A1, AN; cin >> N >> A1 >> AN;

	// inclusive ranges
	auto query = [&](int l1, int r1, int l2, int r2) -> int {
		cout << 'Q' << ' ' << l1+1 << ' ' << r1+1 << ' ' << l2+1 << ' ' << r2+1 << '\n' << flush;
		int v; cin >> v;
		return v;
	};

	vector<int> diffs(N-1);
	for (int i = 0; i+1 < N; i++) {
		diffs[i] = query(i, i+1, i, i+1);
	}
	for (int i = 0; i < N-1; ) {
		if (diffs[i] == 0) {
			i++;
			continue;
		}

		int j = i+1;
		while (j < N-1 && diffs[j] == 0) j++;
		if (j == N-1) break;
		int v = query(i, j+1, i, j+1);
		if (v == max({abs(diffs[i]), abs(diffs[j]), abs(diffs[i] - diffs[j])})) diffs[j] *= -1;
		assert(v == max({abs(diffs[i]), abs(diffs[j]), abs(diffs[i] + diffs[j])}));
		i = j;
	}

	vector<int> res = [&]() -> vector<int> {
		array<vector<int>, 2> A{vector<int>(N), vector<int>(N)};
		A[0][0] = A[1][0] = A1;
		for (int i = 1; i < N; i++) {
			A[0][i] = A[0][i-1] + diffs[i-1];
			A[1][i] = A[1][i-1] - diffs[i-1];
		}
		assert(A[0][N-1] == AN || A[1][N-1] == AN);
		if (A[0][N-1] != AN) {
			return A[1];
		} else if (A[1][N-1] != AN) {
			return A[0];
		}
		assert(A[0][N-1] == A[1][N-1]);
		if (A[0] == A[1]) {
			return A[0];
		}
		int i = 0;
		int j = 1;
		while (A[0][i] + A[0][i+1] == A[0][j] + A[0][j+1]) j++;
		int v = query(i, i+1, j, j+1);
		if (v == max(A[0][i], A[0][i+1]) - min(A[0][j], A[0][j+1])) return A[0];
		else return A[1];
	}();

	cout << '!';
	for (int i : res) cout << ' ' << i;
	cout << '\n' << flush;

	return 0;
}
