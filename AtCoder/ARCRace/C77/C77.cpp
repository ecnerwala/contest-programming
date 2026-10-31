#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int>A(N);
	for (int i = 0; i < N; i++) cin >> A[i];
	vector<int> R;
	int v;
	for (v = N-1; v >= 0; v -= 2) R.push_back(A[v]);
	v += 2;
	for (v ^= 1; v < N; v += 2) R.push_back(A[v]);
	for (int i = 0; i < N; i++) {
		cout << R[i] << " \n"[i+1==N];
	}

	return 0;
}
