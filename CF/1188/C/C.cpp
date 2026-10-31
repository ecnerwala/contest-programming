#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N;
	cin >> N;
	int A; cin >> A;
	long long ans = 0;
	for (int i = 1; i < N; i++) {
		int B; cin >> B;
		ans += A ^ B;
		A = B;
	}
	cout << ans << '\n';

	return 0;
}
