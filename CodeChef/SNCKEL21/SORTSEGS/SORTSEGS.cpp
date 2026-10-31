#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		int lo = 0, hi = N;
		while (lo < hi && A[lo] == lo) lo++;
		while (lo < hi && A[hi-1] == hi-1) hi--;
		if (lo == hi) {
			cout << 0 << '\n';
		} else if (hi - lo <= K) {
			cout << 1 << '\n';
		} else {
			std::vector<int> B = A;
			std::sort(B.begin()+lo, B.begin()+lo+K);
			std::sort(B.begin()+hi-K, B.begin()+hi);
			if (std::is_sorted(B.begin(), B.end())) {
				cout << 2 << '\n';
			} else {
				B = A;
				std::sort(B.begin()+hi-K, B.begin()+hi);
				std::sort(B.begin()+lo, B.begin()+lo+K);
				if (std::is_sorted(B.begin(), B.end())) {
					cout << 2 << '\n';
				} else {
					cout << 3 << '\n';
				}
			}
		}
	}

	return 0;
}
