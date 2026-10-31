#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<array<int, 2>> A(N);
	A[0] = {0,0};
	string S; cin >> S;
	for (int i = 0; i < N-1; i++) {
		A[i+1] = A[i];
		switch(S[i]) {
			case 'L':
				A[i+1][0]--;
				break;
			case 'R':
				A[i+1][0]++;
				break;
			case 'D':
				A[i+1][1]--;
				break;
			case 'U':
				A[i+1][1]++;
				break;
			default: assert(false);
		}
	}
	int L; cin >> L;
	int ans = 0;
	for (int i = 0; i+L < N; i++) {
		ans += (max(abs(A[i][0] - A[i+L][0]), abs(A[i][1] - A[i+L][1])) <= 1);
	}
	cout << ans << '\n';

	return 0;
}
