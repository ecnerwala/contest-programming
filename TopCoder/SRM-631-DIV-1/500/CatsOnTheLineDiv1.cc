#include <bits/stdc++.h>
using namespace std;

class CatsOnTheLineDiv1 {

public:

	int getNumber(vector <int> position, vector <int> count, int time) {
		int N = int(position.size());
		assert(N);
		assert(N == int(count.size()));
		vector<pair<int, int>> cats(N);
		for (size_t i = 0; int(i) < N; i++) {
			cats[i] = {position[i], count[i]};
		}

		sort(cats.begin(), cats.end());

		int res = 0;
		int last = cats[0].first - time - 1;
		for (int i = 0; i < N; i++) {
			int p = cats[i].first;
			int c = cats[i].second;

			last = max(last, p - time - 1);
			assert(last < p + time);
			if (last + c <= p + time) {
				// lay them out this way, and keep going
				last = last + c;
			} else {
				res ++;
				// clump them all together at p+time
				// kill everything else that can be killed
				while (i+1 < N && cats[i+1].first - time <= p + time) {
					i++;
				}
				last = p + time;
			}
		}
		return res;
	}
};

// vim:ft=cpp
