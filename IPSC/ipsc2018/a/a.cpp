#include<bits/stdc++.h>
using namespace std;

int main() {
	int T; cin >> T;
	while ( T-- > 0) {
		int N; cin >> N;
		for (int i = 0; i < N; i++) {
			string s; cin >> s;
			assert(!s.empty());
			assert(s[0] > '0');
			cout << '1';
			if (i == N-1) {
				cout << '\n';
			} else {
				for (int j = 1; j < int(s.size()); j++) {
					cout << '0';
				}
				cout << ' ';
			}
		}
	}

	return 0;
}
