#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int NA, NB, Q; cin >> NA >> NB >> Q;
	int cA = 0, cB = 0;
	vector<bool> A(NA), B(NB);
	while (Q--) {
		char d; cin >> d;
		int v; cin >> v; v--;
		if (d == 'R') {
			cA += 2*(A[v]=!A[v])-1;
		} else if (d == 'C') {
			cB += 2*(B[v]=!B[v])-1;
		} else assert(false);
	}
	cout << int64_t(cA) * int64_t(NB - cB) + int64_t(NA - cA) * int64_t(cB) << '\n';

	return 0;
}
