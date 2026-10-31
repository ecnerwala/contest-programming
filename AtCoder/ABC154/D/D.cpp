#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K; cin >> N >> K;
	vector<int> A(N+1);
	for (int i = 0; i < N; i++) cin >> A[i+1], A[i+1] += A[i];
	int ans = 0;
	for (int i = 0; i<=N-K; i++) ans = max(ans, A[i+K]-A[i]);
	cout << (ans+K)/2 << '.' << (ans+K)%2*5 << "0000000000000\n";

	return 0;
}
