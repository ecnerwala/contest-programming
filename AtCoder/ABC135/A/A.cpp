#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	long long A, B; cin >> A >> B;
	if (A % 2 != B % 2) {
		cout << "IMPOSSIBLE\n";
	} else {
		cout << (A+B)/2 << '\n';
	}

	return 0;
}
