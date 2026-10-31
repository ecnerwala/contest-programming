#include <bits/stdc++.h>
using namespace std;

int main() {
	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	int a0 = 0;
	int a1 = 0;
	for (int i = 0; i < N; i++) {
		a0 += (i & 1) != (A[i] & 1);
		a1 += (i & 1) == (A[i] & 1);
	}
	cout << min(a0, a1) << '\n';
}
