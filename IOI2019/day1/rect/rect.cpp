#include "rect.h"

#include<bits/stdc++.h>
using namespace std;

using ll = long long;

ll count_rectangles(vector<vector<int>> G) {
	int N = int(G.size()) - 1;
	int M = int(G[0].size()) - 1;

	if (N <= 1 || M <= 1) return 0;

	vector<stack<int>> stacks(M);
	for (int j = 1; j < M; j++) {
		if (G[0][j] > G[1][j]) {
			stacks[j].push(0);
		}
		stacks[j].push(1);
	}

	vector<vector<pair<int, int>>> history(M, vector<pair<int, int>>(M, pair<int, int>(-1, -1)));

	ll ans = 0;

	for (int i = 1; i < N; i++) {
		stack<pair<int, vector<int>>> s;
		s.emplace(0, vector<int>());
		for (int j = 1; j <= M; j++) {
			vector<int> curSet;
			bool initialized = false;

			auto merge = [&](const vector<int>& v) {
				if (!initialized) {
					curSet = v;
					initialized = true;
					return;
				}
				vector<int> nCurSet;
				set_intersection(curSet.begin(), curSet.end(), v.begin(), v.end(), back_inserter(nCurSet));
				curSet = std::move(nCurSet);
			};

			while (!s.empty() && G[i][j] >= G[i][s.top().first]) {
				merge(s.top().second);
				if (G[i][j] > G[i][s.top().first]) {
					s.pop();
					if (!s.empty()) {
						int l = s.top().first + 1;
						int r = j-1;
						if (history[l][r].second != i-1) {
							history[l][r].first = i;
						}
						history[l][r].second = i;
						//cerr << i << ' ' << l << ' ' << r << ' ' << history[l][r].first << '\n';
						//for (auto it : curSet) { cerr << it << ' '; } cerr << '\n';

						assert(initialized);
						int numGood = int(curSet.end() - lower_bound(curSet.begin(), curSet.end(), history[l][r].first));
						//cerr << numGood << '\n';
						ans += numGood;
					}
				} else {
					s.pop();
				}
			}
			if (j == M) break;
			vector<int> heights;
			{ // build heights
				while (!stacks[j].empty() && G[i+1][j] >= G[stacks[j].top()][j]){
					if (G[i+1][j] > G[stacks[j].top()][j]) {
						stacks[j].pop();
						if (!stacks[j].empty()) {
							heights.push_back(stacks[j].top()+1);
						}
					} else {
						stacks[j].pop();
					}
				}
				stacks[j].push(i+1);
			}
			reverse(heights.begin(), heights.end());
			merge(heights);
			s.emplace(j, std::move(curSet));
		}
	}
	return ans;
}
