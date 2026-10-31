#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> V(N+1);
		for (int i = 1; i <= N; i++) cin >> V[i];
		std::vector<int> A(N+1);
		for (int i = 1; i <= N; i++) cin >> A[i];

		std::string B(N, '?');

		for (int L = N; L >= 1; L--) {
			int v = V[L];
			int b = 0;
			bool has_smaller = false;
			bool has_bigger = false;
			while ((L >> b) > 0 && A[L >> b] != v) {
				if (A[L >> b] > v) has_bigger = true;
				else has_smaller = true;
				b++;
			}
			if ((L >> b) == 0) {
				goto impossible;
			}
			if ((L >> b) > 1) {
				if (A[L >> (b+1)] > v) {
					has_smaller = true;
				} else if (A[L >> (b+1)] < v) {
					has_bigger = true;
				} else {
					// do nothing
				}
			}
			if (has_bigger && has_smaller) goto impossible;
			if (has_smaller) {
				// must be max heap
				B[L-1] = '1';
			} else {
				B[L-1] = '0';
			}

			// swap things back down
			for (int i = b; i > 0; i--) {
				std::swap(A[(L >> i)], A[(L >> (i-1))]);
			}
		}

		cout << B << '\n';
		continue;
impossible:
		cout << "Impossible" << '\n';
		continue;
	}

	return 0;
}
