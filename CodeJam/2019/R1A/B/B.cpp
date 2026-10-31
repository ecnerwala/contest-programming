#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	int N, M;
	cin >> N >> M;

	for (int case_num = 1; case_num <= T; case_num ++) {

		int ans = 0, mod = 1;

		for (int d : {7, 11, 13, 15, 17, 16}) {
			for (int z = 0; z < 18; z++) {
				cout << d << " \n"[z+1 == 18];
			}
			cout << flush;
			int tot = 0;
			for (int z = 0; z < 18; z++) {
				int v; cin >> v; tot += v;
			}
			tot %= d;
			while (ans % d != tot) {
				ans += mod;
			}
			assert(ans % d == tot);
			assert(__gcd(mod, d) == 1);
			mod *= d;
		}
		//cerr << mod << '\n';
		assert(mod > M);
		cout << ans << endl << flush;
		int verdict; cin >> verdict;
		if (verdict == -1) break;
		//assert(verdict == 1);
	}

	return 0;
}
