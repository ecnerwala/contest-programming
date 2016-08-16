#include<bits/stdc++.h>
using namespace std;

int main() {
	int n; cin >> n;
	string s; cin >> s;
	int res = 0;
	for(int i = 0; i < n; i++) {
		int tot_x = 0;
		int tot_y = 0;
		for(int j = i; j < n; j++) {
			switch(s[j]) {
				case 'L': tot_x --; break;
				case 'R': tot_x ++; break;
				case 'D': tot_y --; break;
				case 'U': tot_y ++; break;
				default: assert(false);
			}
			res += (!tot_x && !tot_y);
		}
	}
	cout << res << '\n';
}
