#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class OneHandSort {

public:

	vector <int> sortShelf(vector <int> target) {
		int N = int(target.size());
		vector<int> curLocs(N);
		for (int i = 0; i < N; i++) {
			curLocs[target[i]] = i;
		}
		vector<int> ops;
		for (int st = 0; st < N; st++) {
			if (target[st] == st) {
				continue;
			}
			int inExtra = target[st];
			ops.push_back(st);
			curLocs[inExtra] = N;
			for (int cur = st; true; ) {
				ops.push_back(curLocs[cur]);
				target[cur] = cur;
				if (curLocs[cur] == N) {
					break;
				} else {
					int nxt = curLocs[cur];
					curLocs[cur] = cur;
					cur = nxt;
				}
			}
		}
		return ops;
	}
};

}  // namespace ecnerwala

using ecnerwala::OneHandSort;

// vim:ft=cpp
