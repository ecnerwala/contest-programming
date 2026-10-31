#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// R = (100+t) / 100 + epsilon
	// S = (100+t) / t - epsilon
	
	int64_t T; cin >> T;
	int64_t N; cin >> N;
	cout << ((100 + T) * N - 1) / T << '\n';

	return 0;
}
