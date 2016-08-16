#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

int B(int n) {
	return n ? B(n / 2) + (n & 1) : 0;
}

ll u(int n) {
	return pow(2.0, B(3 * n)) + pow(3.0, B(n)) + B(n + 1);
}

ld area(ld a, ld b, ld c, ld d) {
	ld s = (a + b + c + d) / 2;
	return (s - a) * (s - b) * (s - c) * (s - d);
}

int main() {
	vector<ll> s;
	s.push_back(u(1));
	s.push_back(u(2));
	s.push_back(u(3));
	ll res = 0;
	for(int i = 4; i <= 3000000; i++) {
		s.push_back(u(i));
		if(s.size() > 7) {
			sort(s.begin(), s.end());
			reverse(s.begin(), s.end());
			s.pop_back();
		}
		ld val = 0;
		ll f = 0;
		for(int a = 0; a < int(s.size()); a++) {
			for(int b = a + 1; b < int(s.size()); b++) {
				for(int c = b + 1; c < int(s.size()); c++) {
					for(int d = c + 1; d < int(s.size()); d++) {
						ld v = area(s[a], s[b], s[c], s[d]);
						if(v > val) {
							val = v;
							f = s[a] + s[b] + s[c] + s[d];
						}
					}
				}
			}
		}
		assert(f > 0);
		res += f;
	}
	cout << res << '\n';
	return 0;
}
