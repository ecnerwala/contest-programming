#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N); for (auto& a : A) { cin >> a; }

	for (int i = 0; i < N; i++) {
		bool take = false;
		if ((i == 0 || A[i] > A[i-1]) == (i != N-1 && A[i] >= A[i+1])) {
			take = true;
		}
		cout << int(take) << " \n"[i+1==N];
	}

	return 0;
}
