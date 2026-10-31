#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;

	int ma = *max_element(A.begin(), A.end());
	int mi = *min_element(A.begin(), A.end());

	if (N <= 2 || ma == mi) {
		cout << -1 << '\n';
		exit(0);
	}

	for (int i = 0; i < N; i++) {
		if (A[i] != mi && A[i] != ma) {
			if (i) {
				// swap it to the beginning
				assert(A[0] != A[i]);
				cout << 0+1 << ' ' << i+1 << '\n';
				exit(0);
			} else {
				// try to swap it to the end
				if (A[N-1] != A[i]) {
					cout << 1 << ' ' << N << '\n';
					exit(0);
				} else {
					int j = int(min_element(A.begin(), A.end()) - A.begin());
					cout << i+1 << ' ' << j+1 << '\n';
					exit(0);
				}
			}
		}
	}

	// all elements are equal to the min or the max
	assert(N >= 3);

	int eq_ind = -1;
	int neq_ind = -1;
	for (int i = 1; i < N-1; i++) {
		if (A[i] == A[0]) {
			eq_ind = i;
		} else {
			neq_ind = i;
		}
	}

	if (A[0] == A[N-1]) {
		assert(neq_ind != -1);
		if (eq_ind == -1) {
			if (N == 3) {
				cout << -1 << '\n';
				exit(0);
			} else {
				cout << 1 << ' ' << 2 << '\n';
				exit(0);
			}
		} else {
			cout << eq_ind+1 << ' ' << neq_ind+1 << '\n';
			exit(0);
		}
	} else {
		if (eq_ind != -1) {
			cout << N << ' ' << eq_ind+1 << '\n';
		} else {
			cout << neq_ind+1 << ' ' << 1 << '\n';
		}
	}

	return 0;
}
