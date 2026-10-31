#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> A(N);

	for (int i = 0; i < N; i++) cin >> A[i];
	cout << *max_element(A.begin(), A.end()) - *min_element(A.begin(), A.end()) << '\n';
	return 0;
}
