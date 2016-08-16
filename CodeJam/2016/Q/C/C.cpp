#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

int N, J;

int main() {

	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	assert(T == 1);
	for(int case_num = 1; case_num <= T; case_num++) {
		cin >> N >> J;
		cout << "Case #" << case_num << ":\n";

		if(N <= 5) {
			for(int v = (1 << (N-1)) + 1; J; v += 2) {

				string s;

				for(int k = v; k; k /= 2) s += char('0' + k % 2);

				reverse(s.begin(), s.end());

				vector<ll> pf;

				for(int b = 2; b <= 10; b++) {
					ll num = 0;
					for(char c : s) num *= b, num += (c - '0');
					bool good = false;
					for(ll p = 2; p <= num / p; p++) {
						if(num % p == 0) {
							pf.push_back(p);
							good = true;
							break;
						}
					}
					if(!good) break;
				}

				if(pf.size() < 9) continue;

				cout << s;
				for(ll p : pf) cout << ' ' << p;
				cout << '\n';
				J--;
			}
		} else {
			assert(N % 2 == 0);
			for(int v = 0; J; v++) {

				string s;
				s += '1';

				for(int i = 0; i < (N - 2) / 2; i++) {
					s += char(((v >> i) & 1) + '0');
					s += char(((v >> i) & 1) + '0');
				}
				s += '1';

				if(N <= 18) {
					for(int b = 2; b <= 10; b++) {
						ll num = 0;
						for(char c : s) num *= b, num += (c - '0');
						assert(num > (b + 1) && (b + 1) > 1);
						assert(num % (b + 1) == 0);
					}
				}


				cout << s;
				for(ll b = 2; b <= 10; b++) cout << ' ' << b + 1;
				cout << '\n';
				J--;
			}
		}

	}

	return 0;
}
