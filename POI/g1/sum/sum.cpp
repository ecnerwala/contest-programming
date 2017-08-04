#include<bits/stdc++.h>
using namespace std;

struct num {
	static const int Z = 100;
	int digs[300];
	bool neg;
	
	int cmp_abs(const num& n) {
		for (int i = 299; i >= 0; i--) {
			if (digs[i] > n.digs[i]) {
				return 1;
			} else if (digs[i] < n.digs[i]) {
				return -1;
			}
		}
		return 0;
	}

	num& operator+= (num& n) {
		if (n.neg == neg) {
			// do the normal thing
			int carry = 0;
			for (int i = 0; i < 300; i++) {
				digs[i] += n.digs[i] + carry;
				carry = digs[i] / 10;
				digs[i] %= 10;
			}
			assert(carry == 0);
		} else {
			int d = cmp_abs(n);
			if (d == 0) {
				neg = false;
				memset(digs, 0, sizeof(digs));
				return *this;
			} else {
				if (d == -1) {
					neg = !neg;
					for (int i = 0; i < 300; i++) {
						swap(digs[i], n.digs[i]);
					}
				}
				int carry = 0;
				for (int i = 0; i < 300; i++) {
					digs[i] += - n.digs[i] + carry;
					if (digs[i] < 0) {
						carry = -1;
						digs[i] += 10;
					} else {
						carry = 0;
					}
					assert(0 <= digs[i] && digs[i] < 10);
				}
				assert(carry == 0);
			}
		}
		return *this;
	}

	num(string s) {
		if (s[0] == '-') {
			neg = true;
		} else if (s[0] == '+') {
			neg = false;
		} else {
			neg = false;
			s = "+" + s;
		}
		memset(digs, 0, sizeof(digs));
		int c = 0;
		while (c < int(s.size()) && s[c] != ',') c++;
		if (c == int(s.size())) s.push_back(',');
		int p = Z + c - 2;
		for (int i = 1; i < int(s.size()); i++) {
			if (i == c) {
				assert(p == Z - 1);
				continue;
			}
			int v = s[i] - '0';
			assert(0 <= v && v < 10);
			digs[p] = v;
			p--;
		}
	}

	num() {
		neg = false;
		memset(digs, 0, sizeof(digs));
	}

};

ostream& operator << (ostream& o, num& n) {
	if (n.neg) {
		o << '-';
	}
	bool printed = false;
	for (int i = 299; i >= n.Z; i--) {
		if (printed || n.digs[i]) {
			o << n.digs[i];
			printed = true;
		}
	}
	if (!printed) {
		o << '0';
	}
	for (int i = 0; i < n.Z; i++) {
		if(n.digs[i]) {
			o << ',';
			for (int j = n.Z-1; j >= i; j--) {
				o << n.digs[j];
			}
			break;
		}
	}
	return o;
}

int main() {
	ios_base::sync_with_stdio(0);
	int N; cin >> N;
	num res;
	for (int i = 0; i < N; i++) {
		string s;
		cin >> s;
		num v(s);
		res += v;
	}
	cout << res << '\n';
	return 0;
}
