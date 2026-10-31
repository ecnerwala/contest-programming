#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class CrazyCrazy {

public:

	string possible(string song) {
		int N = int(song.size());
		assert(N % 2 == 0);
		unordered_set<string> prefs;
		for (int m = 0; m < (1 << (N/2)); m += 2) {
			string res[2];
			for (int i = 0; i < N/2; i++) {
				res[bool(m & (1 << i))] += song[i];
			}
			if (res[0].size() > res[1].size()) {
				swap(res[0], res[1]);
			}
			assert(res[0].size() <= res[1].size());
			if (res[1].substr(0, res[0].size()) != res[0]) {
				continue;
			}
			string cur = res[1].substr(res[0].size());
			prefs.insert(cur);
		}
		reverse(song.begin(), song.end());
		for (int m = 0; m < (1 << (N/2)); m += 2) {
			string res[2];
			for (int i = 0; i < N/2; i++) {
				res[bool(m & (1 << i))] += song[i];
			}
			if (res[0].size() > res[1].size()) {
				swap(res[0], res[1]);
			}
			assert(res[0].size() <= res[1].size());
			if (res[1].substr(0, res[0].size()) != res[0]) {
				continue;
			}
			string cur = res[1].substr(res[0].size());
			reverse(cur.begin(), cur.end());
			if (prefs.count(cur)) {
				return "possible";
			}
		}
		return "impossible";
	}
};

}  // namespace ecnerwala

using ecnerwala::CrazyCrazy;

// vim:ft=cpp
