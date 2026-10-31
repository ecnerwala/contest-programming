#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	int L = N+M;
	string S, T; cin >> S >> T;
	assert(int(S.size()) == L);
	assert(int(T.size()) == L);
	int ans = 0;
	for (int z = 0; z < 2; z++) {
		std::vector<std::array<int, 2>> A(z ? M : N);
		{
			int it = 0;
			for (int i = 0; i < L; i++) {
				if (S[i] - '0' == z) {
					A[it++][0] = i;
				}
			}
			assert(it == int(A.size()));
		}
		{
			int it = 0;
			for (int i = 0; i < L; i++) {
				if (T[i] - '0' == z) {
					A[it++][1] = i;
				}
			}
			assert(it == int(A.size()));
		}

		int K = int(A.size());
		for (auto& v : A) {
			if (v[0] > v[1]) std::swap(v[0], v[1]);
		}
		for (int i = 0; i+1 < K; i++) {
			assert(A[i][0] < A[i+1][0]);
			assert(A[i][1] < A[i+1][1]);
		}
		std::vector<int> B(L, 0);
		for (int i = 0, j; i < K; i = j) {
			j = i;
			int pos = A[i][1];
			while (j < K && A[j][0] <= pos && pos <= A[j][1]) {
				B[pos] = 1;
				pos++;
				j++;
			}
		}
		int cur = 0;
		for (int i = 0; i+1 < L; i++) {
			cur += B[i] == B[i+1];
		}
		ans = std::max(ans, cur);
	}
	cout << ans << '\n';

	return 0;
}
