#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	if ((K > 1 || N == 1) && N * N % K == 0) {
		int a = std::gcd(N, K);
		int b = K / a;
		cout << "Yes" << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << 1 + (i+j)%a + (j % b) * a << " \n"[j+1==N];
			}
		}
	} else {
		cout << "No" << '\n';
	}

	return 0;
}
