#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		string S; cin >> S;
		int cur = 0;
		string S2;
		for (char c : S) {
			int v = c - '0';
			while (cur < v) S2 += '(', cur++;
			while (cur > v) S2 += ')', cur--;
			S2 += c;
		}
		while (cur > 0) S2 += ')', cur--;

		cout << "Case #" << case_num << ": " << S2 << '\n';
	}

	return 0;
}
