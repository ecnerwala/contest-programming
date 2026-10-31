#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<uint32_t> B(N);
	for (auto& b : B) cin >> b;

	// We have B[0] = A[0] + A[1], B[1] = A[0]
	
	// First, apply the transformation B[1] := B[0] - B[1], so B[0] = A[0] + A[1], B[1] = A[1]
	// Note that this causes B[N..infty] = A[N..infty] = 0
	for (int l = 0; (1 << l) < N; l++) {
		for (int i = 0; i < N; i++) {
			if (i & (1 << l)) B[i] ^= B[i-(1<<l)];
		}
	}

	// Now, apply B[0] := B[0] - B[1], so B[0] = A[0], B[1] = A[1]
	for (int l = 0; (1 << l) < N; l++) {
		for (int i = 0; i < N; i++) {
			if (i & (1 << l)) B[i - (1 << l)] ^= B[i];
		}
	}

	for (int i = 0; i < N; i++) {
		cout << B[N-1-i] << " \n"[i+1==N];
	}

	return 0;
}
