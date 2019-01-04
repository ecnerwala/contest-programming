#include<bits/stdc++.h>
using namespace std;

int main() {
	long long A, B, C; cin >> A >> B >> C;
	cout << B + min(C, B + A + 1) << '\n';

	return 0;
}
