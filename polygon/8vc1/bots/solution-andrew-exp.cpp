#include<bits/stdc++.h>
using namespace std;

int main() {
	int n; cin >> n;
	string s; cin >> s;

	n = min(n, 30);
	int res = 0;
	for(int m = 1; m < (1 << n); m++) {
		int tot_x = 0, tot_y = 0;
		for(int j = 0; j < n; j++) {
			if(!(m & (1 << j))) continue;
			switch(s[j]) {
				case 'L': tot_x --; break;
				case 'R': tot_x ++; break;
				case 'D': tot_y --; break;
				case 'U': tot_y ++; break;
				default: assert(false);
			}
		}
		res += (!tot_x && !tot_y);
	}
	cout << res << '\n';
}
