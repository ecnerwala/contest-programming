#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N);
	for (auto& a : A) {
		cin >> a;
	}

	int has_1 = -1;
	for (int i = 0; i+1 < N; i++) {
		if (A[i+1] / A[i] == 1) {
			if (has_1 == -1) {
				has_1 = i;
			} else if (i >= has_1 + 2) {
				cout << "YES" << '\n';
				cout << has_1+1 << ' ' << has_1+2 << ' ' << i+1 << ' ' << i+2 << '\n';
				exit(0);
			} else {
				// do nothing
			}
		}
	}

	// otherwise, there are at most log(n) things

	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			for (int p = j+1; p < N; p++) {
				for (int q = p+1; q < N; q++) {
					if (A[q] / A[p] == A[j] / A[i]) {
						cout << "YES" << '\n';
						cout << i+1 << ' ' << j+1 << ' ' << p+1 << ' ' << q+1 << '\n';
						exit(0);
					}
				}
			}
		}
	}
	cout << "NO" << '\n';
	return 0;
}
