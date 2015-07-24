#include<bits/stdc++.h>
using namespace std;

// can't take from a
int recurse(int a, int b, int c) {
	assert(a >= 0 && b >= 0 && c >= 0);
	if(a == 0 && b == 0 && c == 0) return 1;
	int res = 0;
	if(b) res += recurse(b - 1, a, c);
	if(c) res += recurse(c - 1, a, b);
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);

	int n; cin >> n;
	cout << 3 * recurse(n - 1, n, n) << '\n';

	return 0;
}
