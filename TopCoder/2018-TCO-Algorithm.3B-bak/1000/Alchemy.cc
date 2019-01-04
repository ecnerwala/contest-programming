#include <bits/stdc++.h>
using namespace std;

class Alchemy {
	typedef pair<int, int> pii;
	typedef long long ll;

	bool isGood(vector<pii> leadGolds, vector<int> golds, ll goal) {
		if (golds.empty()) return false;

		sort(golds.begin(), golds.end());
	}

public:

	string leadToGold(vector <int> leadOutput, vector <int> goldOutput, long long goal) {
		assert(leadOutput.size() == goldOutput.size());
		vector<int> golds;
		vector<pii> leadGolds;
		for (int i = 0; i < int(leadOutput.size()); i++) {
			if (leadOutput[i] == 0) golds.push_back(goldOutput[i]);
			else leadGolds.emplace_back(leadOutput[i] - 1, goldOutput[i]);
		}
		return isGood(leadGolds, golds, goal) ? "Possible" : "Impossible";
	}
};

// vim:ft=cpp
