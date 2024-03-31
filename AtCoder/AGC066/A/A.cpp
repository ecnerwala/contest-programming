#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, D; cin >> N >> D;
	std::vector<int> A(N*N);
	std::vector<int> B(N*N);
	std::vector<int> C(N*N);

	int64_t c0 = 0;
	int64_t c1 = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> A[i*N+j];
			int offset = A[i*N+j] % D;
			if (offset < 0) offset += D;
			int v = (A[i*N+j] - offset) / D;
			if ((i^j^v) & 1) {
				B[i*N+j] = v*D;
				C[i*N+j] = (v+1)*D;
			} else {
				C[i*N+j] = v*D;
				B[i*N+j] = (v+1)*D;
			}
			c0 += std::abs(B[i*N+j] - A[i*N+j]);
			c1 += std::abs(C[i*N+j] - A[i*N+j]);
		}
	}

	if (c1 < c0) {
		std::swap(B, C);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << B[i*N+j] << " \n"[j+1==N];
		}
	}

	return 0;
}
