#include<bits/stdc++.h>
using namespace std;

int main() {
	int T;
	cin >> T;
	for(int t = 1; t <= T; t++) {
		int D;
		cin >> D;
		vector<int> P(D);
		for(int i = 0; i < D; i++) {
			cin >> P[i];
		}
		int res = 10000000;
		for(int i = 1; i < 2000; i++) {
			int val = i;
			for(int p : P) {
				val += (p - 1) / i;
			}
			res = min(val, res);
		}
		cout << "Case #" << t << ": " << res << '\n';
	}
	return 0;
}
