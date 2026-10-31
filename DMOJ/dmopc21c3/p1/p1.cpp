#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N);

	auto query = [&](int i, int j) {
		cout << '?' << ' ' << i+1 << ' ' << j+1 << '\n' << flush;
		int64_t r; cin >> r;
		return r;
	};

	A[0] = query(0, 1);
	A[1] = query(0, 1) / 2;
	A[0] += A[1];

	int64_t x = 2 * A[0];
	for (int i = 2; i < N; i++) {
		A[i] = x - query(0, i);
		x += A[i];
	}

	cout << '!'; for (int64_t a : A) { cout << ' ' << a; } cout << '\n' << flush;

	return 0;
}
