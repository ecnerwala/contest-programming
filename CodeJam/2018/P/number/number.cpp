#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		int A, B;
		cin >> A >> B;
		int N;
		cin >> N;

		string resp;

		while (true) {
			assert(A < B);
			int guess = (A+B+1) / 2;
			cout << guess << endl;
			cin >> resp;
			if (resp == "WRONG_ANSWER") { return 0; }
			else if (resp == "CORRECT") break;
			else if (resp == "TOO_BIG") {
				B = guess - 1;
			} else if (resp == "TOO_SMALL") {
				A = guess;
			} else assert(false);
		}
	}

	return 0;
}
