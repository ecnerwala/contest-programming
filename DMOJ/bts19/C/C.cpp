#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	// (2^k)^m >= n
	long long N, M;
	cin >> N >> M;
	int b = (N == 1 ? 0 : 64 - __builtin_clzll(N-1));
	cout << (b + M-1) / M << '\n';

	return 0;
}
