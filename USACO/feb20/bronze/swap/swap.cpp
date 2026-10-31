#include<bits/stdc++.h>
using namespace std;

vector<int> apply(const vector<int>& a, const vector<int>& b) {
	vector<int> res(a.size());
	for (int i = 0; i < int(a.size()); i++) {
		res[i] = a[b[i]];
	}
	return res;
}

int main() {
	if (fopen("swap.in", "r")) {
		freopen("swap.in", "r", stdin);
		freopen("swap.out", "w", stdout);
	}
	int N, K; cin >> N >> K;
	vector<int> A(N);
	iota(A.begin(), A.end(), 0);
	for (int z = 0; z < 2; z++) {
		int l, r; cin >> l >> r; l--;
		reverse(A.begin() + l, A.begin() + r);
	}
	vector<int> ans(N);
	iota(ans.begin(), ans.end(), 0);
	while (K) {
		if (K & 1) ans = apply(ans, A);
		A = apply(A, A);
		K /= 2;
	}
	for (int i : ans) {
		cout << i+1 << '\n';
	}

	return 0;
}
