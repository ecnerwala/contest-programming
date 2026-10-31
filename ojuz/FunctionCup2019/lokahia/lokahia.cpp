#include "lokahia.h"

#include <bits/stdc++.h>
using namespace std;

struct f {
	int first;
	int maxVal = -1;
	int cnt = 1;
};

int FindBase(int N){
	vector<f> ranges;
	for (int i = 0; i < N; ) {
		f cur{i};
		int cnt = 1;
		int j;
		for (j = i+1; cnt > 0 && j < N; j++) {
			int v = CollectRelics(i, j);
			if (v == -1) {
				cnt--;
				ranges.push_back(f{j});
			} else {
				cnt++;
				cur.cnt ++;
				cur.maxVal = max(cur.maxVal, v);
			}
		}

		if (cnt == 0) {
			ranges.push_back(cur);
			i = j;
		} else {
			assert(cnt > 0);
			assert(j == N);
			for (const auto& o : ranges) {
				if (o.first > i) {
					// skip it for speed
					continue;
				}
				int v = CollectRelics(i, o.first);
				if (v == -1) {
					// skip it
				} else {
					cur.cnt += o.cnt;
					cur.maxVal = max(cur.maxVal, o.maxVal);
					cur.maxVal = max(cur.maxVal, v);
				}
			}
			if (cur.cnt > N / 2) {
				if (cur.maxVal == -1) {
					return cur.first;
				} else {
					return cur.maxVal;
				}
			} else {
				return -1;
			}
		}
	}
	return -1;
}
