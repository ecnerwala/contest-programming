#include<bits/stdc++.h>
using namespace std;

const int MAXN = 6.1e5;
int N;
string S;
int A[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> S;
	N = int(S.size()) + 1;
	for (int i = 1; i < N; i++) {
		if (S[i-1] == '<') {
			A[i] = max(A[i], A[i-1]+1);
		}
	}
	for (int i = N-2; i >= 0; i--) {
		if (S[i] == '>') {
			A[i] = max(A[i], A[i+1]+1);
		}
	}
	long long ans = 0;
	for (int i = 0; i < N; i++) {
		ans += A[i];
	}
	cout << ans << '\n';

	return 0;
}
