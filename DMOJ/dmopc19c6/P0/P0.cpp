#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int A, B, C; cin >> A >> B >> C;
	cout << (2 * max({A,B,C}) < A + B + C ? "yes" : "no") << '\n';

	return 0;
}
