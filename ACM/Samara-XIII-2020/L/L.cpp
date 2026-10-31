#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> A(N);
	for (int i = 0; i < N; i++) cin >> A[i];
	sort(A.begin(), A.end(), std::greater<>());
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		ans += max(0, A[i] - (i+1));
	}
	cout << ans << '\n';

	return 0;
}
