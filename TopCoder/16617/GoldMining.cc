#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class GoldMining {

public:

	long long maxProfit(long long goldInGround, long long miningTime, long long hiringCost) {
		using ll = long long;
		if (goldInGround <= miningTime) {
			return goldInGround;
		}
		ll ans = miningTime;
		ll cur_gold = 0;
		ll cur_workers = 1;
		for (int t = 0; t < miningTime; t++) {
			ll max_hire = min(cur_gold / hiringCost, goldInGround / (miningTime - t) - cur_workers);
			cur_workers += max_hire;
			cur_gold -= max_hire * hiringCost;
			// we cound stop hiring here...
			// these guys can't clear everything, by definition
			ans = max(ans, cur_gold + cur_workers * (miningTime - t));

			if (cur_gold >= hiringCost) {
				// add one guy to finish everything
				cur_workers++;
				cur_gold -= hiringCost;
				assert(cur_workers * (miningTime - t) >= goldInGround);
				ans = max(ans, cur_gold + goldInGround);
				break;
			}

			// spend this unit mining
			cur_gold += min(cur_workers, goldInGround);
			goldInGround -= min(cur_workers, goldInGround);
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::GoldMining;

// vim:ft=cpp
