#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		string S; cin >> S;
		string V;
		for (char c : S) {
			V += (c ^ 'S' ^ 'E');
		}

		cout << "Case #" << case_num << ": " << V << '\n';
	}

	return 0;
}
