#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i]; A[i] ^= (i & 1);
	}
	int num_begin = 0;
	while (num_begin < N && A[num_begin] == 0) num_begin++;

	if (num_begin == 0) {
		cout << "No" << '\n';
		exit(0);
	}
	num_begin--;
	int num_needed = 0;
	for (int i = num_begin; i+1 < N; i++) {
		num_needed += A[i] == A[i+1];
	}
	if (num_needed > num_begin) {
		cout << "No" << '\n';
	} else {
		cout << "Yes" << '\n';
	}


	return 0;
}
