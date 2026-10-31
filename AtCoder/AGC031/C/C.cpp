#include<bits/stdc++.h>
using namespace std;

vector<int> ans;

void solve(int N, int A, int B, int msk) {
	assert(__builtin_popcount(unsigned(A^B)) % 2 == 1);
	assert(((A^B) & msk) == (A^B));
	assert(__builtin_popcount(unsigned(msk)) == N);
	if (N == 1) {
		ans.push_back(A);
		ans.push_back(B);
		return;
	}
	int d = A ^ B;
	int flip = d & (-d);
	msk ^= flip;
	d ^= flip;
	assert(__builtin_popcount(unsigned(d)) % 2 == 0);
	assert(msk);
	d ^= msk & (-msk);
	solve(N-1, A, A ^ d, msk);
	solve(N-1, A^d^flip, B, msk);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, A, B; cin >> N >> A >> B;
	assert(A != B);
	if (__builtin_popcount(unsigned(A^B)) % 2 == 0) {
		cout << "NO" << '\n';
		exit(0);
	}

	solve(N, A, B, (1<<N)-1);
	cout << "YES" << '\n';
	for (int i = 0; i < (1<<N); i++) {
		cout << ans[i] << " \n"[i+1==(1<<N)];
	}

	return 0;
}
