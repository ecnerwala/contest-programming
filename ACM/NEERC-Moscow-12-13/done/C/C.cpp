#include<bits/stdc++.h>
using namespace std;

int A[2000];

int main() {
	ios_base::sync_with_stdio(0);
	int N; cin >> N;
	for(int i = 0; i < N; i++) cin >> A[i];
	int sum = 0;
	for(int i = 0; i < N; i++) {
		sum += A[i];
	}
	int res = 0;
	for(int i = 0; i < N; i++) res += (A[i] * N > sum);
	cout << res << '\n';

	return 0;
}
