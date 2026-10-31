#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	if (N <= 3) {
		for (int i = 0; i < N; i++) {
			cout << i+1 << " \n"[i+1==N];
		}
		exit(0);
	}

	assert(N >= 4);

	for (int i = 0; i < N; i++) {
		if (A[i] == 1) {
			std::rotate(A.begin(), A.begin() + i, A.end());
			break;
		}
	}
	assert(A[0] == 1);

	std::vector<int> ans;

	{
		std::vector<int> B = A;
		for (int j = 0; j < N; j++) {
			if (B[j] == j+1) {
				continue;
			} else {
				for (int k = j+1; k < N; k++) {
					if (B[k] == j+1) {
						std::swap(B[j], B[k]);
						break;
					}
				}
				break;
			}
		}
		ans = std::move(B);
	}

	auto check = [&](std::vector<int> b) {
		for (int i = 0; i < N; i++) {
			if (b[i] == 1) {
				std::rotate(b.begin(), b.begin() + i, b.end());
				break;
			}
		}
		if (b < ans) ans = std::move(b);
	};

	// Swap 1 and then rotate
	for (int i = 1; i < N; i++) {
		if (A[i] == 2) {
			{
				auto B = A;
				std::swap(B[0], B[i-1]);
				check(B);
			}
			{
				auto B = A;
				std::swap(B[0], B[i]);
				check(B);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
