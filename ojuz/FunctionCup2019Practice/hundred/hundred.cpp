#include "hundred.h"

#include <bits/stdc++.h>
using namespace std;

string GetHundredPoints(int A, int B, int C) {
	int N = A + B + C;
	string ans(N, '_');

	vector<int> locs(N); iota(locs.begin(), locs.end(), 0);
	vector<pair<int, char>> cnts({{A, 'A'}, {B, 'B'}, {C, 'C'}});

	while (!locs.empty()) {
		sort(cnts.begin(), cnts.end());
		reverse(cnts.begin(), cnts.end());
		while (!cnts.empty() && cnts.back().first == 0) {
			cnts.pop_back();
		}
		assert(!cnts.empty());
		if (cnts.size() == 1) {
			assert(cnts[0].first == int(locs.size()));
			for (int i = 0; i < int(locs.size()); i++) {
				ans[locs[i]] = cnts[0].second;
			}
			locs = {};
			continue;
		}

		assert(cnts.size() >= 2);

		string cur = ans;
		for (int i = 0; i < int(locs.size()); i++) {
			if (i < cnts[0].first) {
				cur[locs[i]] = cnts[0].second;
			} else if (i < cnts[0].first + cnts[1].first) {
				cur[locs[i]] = cnts[1].second;
			} else {
				assert(cnts.size() >= 3);
				cur[locs[i]] = cnts[2].second;
			}
		}
		int base = Mark(cur);
		for (int j = cnts[0].first; true; j ++) {
			if (j == cnts[0].first + cnts[1].first) {
				// all of cnts[0] and cnts[1] are equal, so they must be equal to cnts[0].second
				vector<int> setlocs;
				vector<int> nlocs;

				setlocs.push_back(locs[0]);
				for (int z = 1; z < int(cnts[0].first); z++) {
					nlocs.push_back(locs[z]);
				}
				for (int z = int(cnts[0].first); z < j; z++) {
					setlocs.push_back(locs[z]);
				}
				for (int z = j; z < int(locs.size()); z++) {
					nlocs.push_back(locs[z]);
				}

				locs = nlocs;
				for (int l : setlocs) {
					ans[l] = cnts[0].second;
					cnts[0].first --;
				}

				break;
			} else {
				swap(cur[locs[0]], cur[locs[j]]);
				int nxt = Mark(cur);
				swap(cur[locs[0]], cur[locs[j]]);
				if (nxt == base) {
					continue;
				}
				if (nxt > base) {
					swap(cur[locs[0]], cur[locs[j]]);
				}
			}
		}
	}

	return ans;
}
