#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
int A[MAXN];
int B[MAXN];

long long absDiff(long long a, long long b) {
	return abs(a-b);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		A[i] = x+y;
		B[i] = x-y;
	}
	sort(A, A+N);
	sort(B, B+N);
	int a = A[N/2];
	int b = B[N/2];
	long long tot = 0;
	for (int i = 0; i < N; i++) {
		tot += absDiff(A[i], a);
		tot += absDiff(B[i], b);
	}
	if ((a & 1) != (b & 1)) {
		int upCost = N - 2 * int(lower_bound(A, A + N, a) - A);
		int downCost = 2 * int(upper_bound(A, A + N, a) - A) - N;
		int leftCost = N - 2 * int(lower_bound(B, B + N, b) - B);
		int rightCost = 2 * int(upper_bound(B, B + N, b) - B) - N;
		tot += min(min(upCost, downCost), min(leftCost, rightCost));
	}
	assert((tot & 1) == 0);
	cout << (tot/2) << '\n';

	return 0;
}
