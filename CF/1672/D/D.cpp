#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> B(N);
		for (auto& a : B) { cin >> a; a--; }

		bool good = true;
		std::vector<int> cnt(N);
		int i, j;
		for (i = N-1, j = N-1; i >= 0; ) {
			int v = A[i];
			while (j >= 0 && B[j] == v) j--, cnt[v]++;
			while (i >= 0 && A[i] == v) i--, cnt[v]--;
			if (cnt[v] < 0) {
				good = false;
				break;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
