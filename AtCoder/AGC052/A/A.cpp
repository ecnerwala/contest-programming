#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S1, S2, S3; cin >> S1 >> S2 >> S3;
		cout << string(N, '0') << string(N, '1') << '0' << '\n';
	}

	return 0;
}

// 00100 is bad if S starts and ends with 0
// 11011 is bad if S starts and ends with 1
// 01111100000 is bad if S starts with 11
//
// 0000011111
// 1111100000
//
// 01111100000111110000
// 01010101100101010110
// 110101010011
// 1010101001
//
// 11110000011111000001
