#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		string N; cin >> N;
		string A;
		string B;
		for (char c : N) {
			if (c == '4') {
				A += '1';
				B += '3';
			} else {
				if (!A.empty()) A += '0';
				B += c;
			}
		}

		cout << "Case #" << case_num << ": " << A << ' ' << B << '\n';
	}

	return 0;
}
