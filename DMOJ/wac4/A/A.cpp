#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N, Q; cin >> N >> Q;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	while (Q--) {
		int op, l, r; cin >> op >> l >> r;
		l--, r--;
		sort(A.begin()+l, A.begin()+r+1);
		if (op == 2) {
			reverse(A.begin()+l, A.begin()+r+1);
		}
	}
	for (int i = 0; i < N; i++) {
		cout << A[i] << " \n"[i+1==N];
	}

	return 0;
}
