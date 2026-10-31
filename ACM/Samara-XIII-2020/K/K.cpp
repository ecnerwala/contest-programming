#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int A[4];
	for (int& i : A) cin >> i;
	sort(A, A + 4);
	cout << (A[0]+A[3] == A[1]+A[2] ? "YES" : "NO") << '\n';

	return 0;
}
