#include <bits/stdc++.h>

// If N is odd, then sum mod N is invariant

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::vector<int>> ops;
	if (std::accumulate(A.begin(), A.end(), 0) % N != 0) {
		ops.push_back(std::vector<int>(N));
		std::iota(ops.back().begin(), ops.back().end(), 1);
		for (int i = 0; i < N; i++) A[i] += ops.back()[i];
		if (std::accumulate(A.begin(), A.end(), 0) % N != 0) {
			cout << "No" << '\n';
			exit(0);
		}
	}
	assert(std::accumulate(A.begin(), A.end(), 0) % N == 0);
	{
		int mid = std::accumulate(A.begin(), A.end(), 0) / N;
		for (int i = 0; i < N; i++) A[i] -= mid;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; A[i] > 0 && j < N; j++) {
			while (A[i] > 0 && A[j] < 0) {
				int num_move = std::min(A[i], -A[j]);
				num_move = std::min(num_move, N-1);
				ops.push_back(std::vector<int>(N));

				int vi = 1, vj = vi + num_move;
				ops.back()[i] = vi;
				ops.back()[j] = vj;

				int v = 1;
				for (int k = 0; k < N; k++) {
					if (k == i || k == j) continue;
					while (v == vi || v == vj) v++;
					ops.back()[k] = v;
					v++;
				}

				ops.push_back(std::vector<int>(N));
				for (int k = 0; k < N; k++) {
					ops.end()[-1][k] = N+1-ops.end()[-2][k];
				}
				std::swap(ops.back()[i], ops.back()[j]);
				A[i] -= num_move;
				A[j] += num_move;
			}
		}
	}

	cout << "Yes" << '\n';
	cout << ops.size() << '\n';
	for (const auto& r : ops) {
		for (int i = 0; i < N; i++) {
			cout << r[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
