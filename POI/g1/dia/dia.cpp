#include<bits/stdc++.h>
using namespace std;

void print(const vector<int> &a) {
	cout << a.size();
	for (int i = 0; i < int(a.size()); i++) cout << ' ' << a[i];
	cout << '\n';
}

void go(vector<int> d) {
	vector<int> p, m;
	if (d.size() == 1 && d[0] == 0) {
		print(p);
		print(m);
		return;
	}
	for (int i = 0; i < 200; i++) {
		// first add one
		int carry = 1;
		for (int j = int(d.size()) - 1; carry && j >= 0; j--) {
			d[j] += carry;
			if (d[j] == 10) {
				d[j] = 0;
				carry = 1;
			} else {
				carry = 0;
			}
		}
		assert(carry <= 1);
		// now divide by 3
		for (int j = 0; j < int(d.size()); j++) {
			carry *= 10;
			carry += d[j];
			d[j] = carry / 3;
			carry = carry % 3;
		}
		assert(carry < 3);
		if (carry == 0) {
			m.push_back(i);
		} else if (carry == 1) {
		} else if (carry == 2) {
			p.push_back(i);
		} else assert(false);
	}
	print(p);
	print(m);
}

int main() {
	ios_base::sync_with_stdio(0);
	int N; cin >> N;
	for (int t = 0; t < N; t++) {
		string s; cin >> s;
		vector<int> digs;
		for (int i = 0; i < int(s.size()); i++) {
			digs.push_back(s[i] - '0');
		}
		go(digs);
	}

	return 0;
}
