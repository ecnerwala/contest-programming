#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class OneHandRoadPainting {

public:

	ll fastest(vector <int> dStart, vector <int> dEnd, int paintPerBrush) {
		ll ans = 0;
		while (!dEnd.empty()) {
			if (dEnd.back() - dStart.back() > paintPerBrush) {
				ll numFull = (ll(dEnd.back()) - ll(dStart.back()) - 1) / ll(paintPerBrush);
				ans += (ll(dEnd.back()) + ll(dEnd.back()) - (numFull-1) * ll(paintPerBrush)) * numFull / 2;
				dEnd.back() -= numFull * ll(paintPerBrush);
			}
			ans += dEnd.back();
			ll amountLeft = paintPerBrush;
			while (amountLeft > 0 && !dEnd.empty()) {
				ll v = min(amountLeft, ll(dEnd.back()) - ll(dStart.back()));
				assert(v > 0);
				amountLeft -= v;
				dEnd.back() -= v;
				if (dEnd.back() == dStart.back()) {
					dEnd.pop_back();
					dStart.pop_back();
				}
			}
		}
		return ans * 2;
	}
};

}  // namespace ecnerwala

using ecnerwala::OneHandRoadPainting;

// vim:ft=cpp
