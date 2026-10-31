#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	string line; getline(cin, line);
	vector<int> bestTime(10, 301);
	int firstSolve = 301;
	int lastSolve = -1;

	vector<vector<int>> contestants;
	for (int i = 0; i < N-1; i++) {
		getline(cin, line);
		stringstream str(line);
		int score = 0;
		int penalty = 0;
		vector<int> solveTimes;
		for (int z = 0; z < 10; z++) {
			string cur; getline(str, cur, ',');
			//cerr << cur << '\n';
			if (cur == "-") continue;
			stringstream tokenV(cur);
			int t, w; tokenV >> t >> w;
			score ++;
			penalty += t + w * 20;
			solveTimes.push_back(t);
			bestTime[z] = min(bestTime[z], t);
			firstSolve = min(firstSolve, t);
			lastSolve = max(lastSolve, t);
		}
		sort(solveTimes.begin(), solveTimes.end());
		solveTimes.push_back(penalty);
		solveTimes.push_back(-score);
		reverse(solveTimes.begin(), solveTimes.end());
		contestants.push_back(solveTimes);
	}

	sort(contestants.begin(), contestants.end());
	//for (const auto& v : contestants) { for (auto it : v) { cerr << it << ' '; } cerr << '\n'; }

	vector<pair<int, int>> pangTime(10);
	{
		getline(cin, line);
		stringstream str(line);
		for (int z = 0; z < 10; z++) {
			string cur; getline(str, cur, ',');
			//cerr << cur << '\n';
			if (cur == "-") {
				pangTime[z] = {301, 0};
				continue;
			}
			stringstream tokenV(cur);
			int t, w; tokenV >> t >> w;
			pangTime[z] = {t,w};
		}
	}


	int ans = 0;

	vector<int> ordering(10);
	iota(ordering.begin(), ordering.end(), 0);
	do {
		int curTime = 0;
		int score = 0;
		int penalty = 0;
		vector<pair<int, int>> solveTimes;
		for (int x = 0; x < 10; x++) {
			int z = ordering[x];
			curTime += pangTime[z].first;
			if (curTime > 300) break;
			score++;
			penalty += curTime + 20 * pangTime[z].second;
			solveTimes.push_back({z, curTime});
		}
		vector<int> rankTuple;
		for (auto it : solveTimes) rankTuple.push_back(it.second);
		rankTuple.push_back(penalty);
		rankTuple.push_back(-score);
		reverse(rankTuple.begin(), rankTuple.end());
		//if (score >= 2) { for (auto it : rankTuple) { cerr << it << ' '; } cerr << '\n'; }
		int r = 1 + int(lower_bound(contestants.begin(), contestants.end(), rankTuple) - contestants.begin());
		int happiness = 5000 / r;
		if (r <= N / 10) {
			happiness += 1200;
		} else if (r <= N / 10 * 3) {
			happiness += 800;
		} else if (r <= N / 10 * 6) {
			happiness += 400;
		}

		for (auto it : solveTimes) {
			if (it.second <= bestTime[it.first]) happiness += 800;
		}

		if (!solveTimes.empty() && solveTimes.front().second <= firstSolve) {
			happiness += 700;
		}
		if (!solveTimes.empty() && solveTimes.back().second >= lastSolve) {
			happiness += 500;
		}

		ans = max(ans, happiness);
	} while (next_permutation(ordering.begin(), ordering.end()));

	cout << ans << '\n';

	return 0;
}
