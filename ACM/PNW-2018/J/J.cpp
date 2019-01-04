#include<bits/stdc++.h>
using namespace std;

int main() {
	int N, S;
	cin >> N >> S;
	int res = 0;
	for (int i = 0; i < N; i++) {
		int T; cin >> T;
		res = max(res, (T * S + 999) / 1000);
	}
	cout << res << '\n';

	return 0;
}
