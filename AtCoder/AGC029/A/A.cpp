#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string S; cin >> S;
	int numB = 0;
	long long res = 0;
	for (char c : S) {
		if (c == 'B') {
			numB ++;
		} else if (c == 'W'){
			res += numB;
		} else assert(false);
	}
	cout << res << '\n';

	return 0;
}
