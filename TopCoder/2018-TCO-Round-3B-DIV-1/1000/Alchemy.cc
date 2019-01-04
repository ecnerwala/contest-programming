#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

const int MAXN = 60;
const int MAXV = 10500;
const int V = 10200;

bool dpv[MAXV];

bool anyv[MAXV];

int modulo;
const ll INF = 2e18;
ll best[MAXV];

class Alchemy {

	void do_dp(const vector<int>& golds, const vector<pair<int, int>>& leadGolds) {
		memset(dpv, false, sizeof(dpv));
		memset(anyv, false, sizeof(anyv));
		dpv[0] = true;
		anyv[0] = true;
		int maxl = 0;
		for (const pair<int, int>& p : leadGolds) {
			maxl = max(p.first, maxl);
		}

		for (int t = 0; t <= maxl; t++) {
			for (const pair<int, int>& p : leadGolds) {
				if (p.first == t) {
					for (int i = 0; i <= V; i++) {
						if (dpv[i]) {
							assert(i+p.second <= V);
							anyv[i+p.second] = true;
						}
					}
				}
			}
			for (int i = V; i >= 0; i--) {
				if (dpv[i]) {
					dpv[i] = false;
					for (int g : golds) {
						assert(i+g <= V);
						dpv[i+g] = true;
					}
				}
			}
		}
	}

	void dijk() {
		assert(anyv[0]);
		modulo = 0;
		for (int i = 1; i <= V; i++) {
			if (anyv[i]) {
				modulo = i;
				break;
			}
		}
		if (modulo == 0) {
			return;
		}
		assert(modulo > 0);
		for (int i = 0; i < modulo; i++) {
			best[i] = INF;
		}
		ll last = -1;
		best[0] = 0;
		while (true) {
			ll next = INF;
			for (int i = 0; i < modulo; i++) {
				if (best[i] < INF && best[i] > last) {
					assert(best[i] != next);
					if (best[i] < next) {
						next = best[i];
					}
				}
			}
			if (next == INF) break;
			last = next;
			for (int v = modulo + 1; v <= V; v++) {
				if (anyv[v]) {
					best[(next + v) % modulo] = min(best[(next+v) % modulo], next+v);
				}
			}
		}
	}

	bool can_make(ll g) {
		if (modulo == 0) {
			return (g == 0);
		}
		return best[g % modulo] != INF && g >= best[g % modulo];
	}

	bool go(vector<int> golds, vector<pair<int, int>> leadGolds, ll goal) {
		do_dp(golds, leadGolds);
		dijk();
		for (int g : golds) {
			if (goal >= g && can_make(goal - g)) {
				return true;
			}
		}
		return false;
	}

public:

	string leadToGold(vector <int> leadOutput, vector <int> goldOutput, ll goal) {
		vector<int> golds;
		vector<pair<int, int>> leadGolds;
		assert(leadOutput.size() == goldOutput.size());
		for (size_t i = 0; i < leadOutput.size(); i++) {
			int l = leadOutput[i];
			int g = goldOutput[i];
			if (l == 0) {
				golds.emplace_back(g);
			} else {
				leadGolds.emplace_back(l-1, g);
			}
		}

		bool good = go(golds, leadGolds, goal);
		return good ? "Possible" : "Impossible";
	}
};

}  // namespace ecnerwala

using ecnerwala::Alchemy;

// vim:ft=cpp
