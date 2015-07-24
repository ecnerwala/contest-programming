#include<bits/stdc++.h>
using namespace std;

struct quaternion {
	int val;
	bool neg;
};

quaternion operator * (quaternion a, quaternion b) {
	quaternion res;
	res.neg = a.neg ^ b.neg;
	if(a.val == 0 || b.val == 0) {
		res.val = a.val + b.val;
	} else if (a.val == b.val) {
		res.val = 0;
		res.neg = !res.neg;
	} else {
		res.val = 6 - (a.val + b.val);
		if((b.val + 1) % 3 == (a.val) % 3) {
			res.neg = !res.neg;
		}
	}

	return res;
}

int main() {
	quaternion i, j, k;
	i.val = 1, j.val = 2, k.val = 3;
	i.neg = j.neg = k.neg = false;

	quaternion m = i, n = k; // mark 1 and 2

	int T; cin >> T;
	for(int t = 1; t <= T; t++) {

		quaternion pref;
		pref.val = 0;
		pref.neg = false;

		int L; long long X;
		string s;
		cin >> L >> X >> s;

		X = min(X % 4 + 8, X);

		int val = 0;
		for(int x = 0; x < X; x++) {
			for(char c : s) {
				switch(c) {
					case 'i': pref = pref * i;
							  break;
					case 'j': pref = pref * j;
							  break;
					case 'k': pref = pref * k;
							  break;
				}
				//cerr << val << ' ' << pref.val << ' ' << pref.neg << '\n';
				if(val == 1 && pref.val == 3 && pref.neg == false) val = 2;
				if(val == 0 && pref.val == 1 && pref.neg == false) val = 1;
			}
		}
		bool res = false;
		if(val == 2 && pref.val == 0 && pref.neg == true) res = true;

		cout << "Case #" << t << ": " << (res ? "YES" : "NO") << '\n';

	}

	return 0;
}
