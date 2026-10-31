#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class NewBanknote {

	ll euroCount(ll v) {
		ll res = (v / 100000) * 2;
		v %= 100000;
		for (int z = 0; z < 5; z++, v /= 10) {
			ll d = v % 10;
			if (d >= 5) {
				res++;
				d -= 5;
			}
			res += (d + 1) / 2;
		}
		return res;
	}

	ll solve(ll newBanknote, ll amountToPay) {
		ll res = euroCount(amountToPay);
		if (newBanknote >= 50000) {
			ll numNew = amountToPay / newBanknote;
			for (ll i = 0; i <= 100000 && i <= numNew; i++) {
				res = min(res, euroCount(amountToPay - newBanknote * (numNew - i)) + (numNew - i));
			}
		} else {
			ll numNew = amountToPay / newBanknote;
			for (ll i = 0; i <= 100000 && i <= numNew; i++) {
				res = min(res, euroCount(amountToPay - newBanknote * i) + i);
			}
		}
		return res;
	}

public:

	vector <int> fewestPieces(int newBanknote, vector <int> amountsToPay) {
		vector<int> res;
		for (int v : amountsToPay) {
			res.push_back(int(solve(newBanknote, v)));
		}
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::NewBanknote;

// vim:ft=cpp
