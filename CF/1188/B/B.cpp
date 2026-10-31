#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> A(N);
	for (int i = 0; i < N; i++) cin >> A[i];
	sort(A.begin(), A.end());
	if (A[N-2] + A[N-3] > A[N-1]) {
		cout << "YES" << '\n';
		int last = A[N-2];
		A.erase(A.begin() + (N-2));
		for (int i : A) cout << i << " ";
		cout << last << '\n';
	} else {
		cout << "NO" << '\n';
	}

	return 0;
}
