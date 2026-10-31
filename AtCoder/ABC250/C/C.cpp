#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int> A(N);
	std::iota(A.begin(), A.end(), 0);
	std::vector<int> invA = A;
	while (Q--){
		int x; cin >> x; x--;
		int i = invA[x];
		if (i == N-1) i--;
		std::swap(A[i], A[i+1]);
		invA[A[i]] = i;
		invA[A[i+1]] = i+1;
	}
	for (int i = 0; i < N; i++) {
		cout << A[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
