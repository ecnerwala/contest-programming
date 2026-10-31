#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;
	const int M_final = *std::max_element(B.begin(), B.end()) + 1;
	std::vector<int> B_vals(N+1);
	for (int i = 0; i < N; i++) {
		B_vals[i+1] = M_final * i + B[i];
	}
	cout << 2*N+1 << '\n';
	int cur_max = N;
	for (int i = N; i > 0; i--) {
		cout << 2 << ' ' << cur_max << '\n';
		cur_max--;
		int d = B_vals[i] - B_vals[i-1];
		cout << 1 << ' ' << d << '\n';
		cur_max += d;
	}
	cout << 2 << ' ' << M_final << '\n';

	return 0;
}
