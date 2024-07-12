#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
	int64_t wl = 0;
	int64_t xl = 0;
	int64_t pl = 0;
	int64_t wr = K;
	int64_t xr = std::accumulate(A.end() - K, A.end(), int64_t(0));
	int64_t pr = 0;
	for (int i = 0; i < K; i++) {
		pr += -(K-1)+2*i;
	}
	assert(pr == 0);

	for (int i = 0; i < K-1; i++) {
		wl++;
		xl += A[i];
		pl += (K-1) - 2*i;
		wr--;
		xr -= A[N-K+i];
		pr -= (K-1) - 2*i;
	}

	return 0;
}
