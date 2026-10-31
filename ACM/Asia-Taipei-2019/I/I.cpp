#include<bits/stdc++.h>
using namespace std;

const int MAXV = 1010;
int cnt[MAXV];
int N;

vector<vector<int>> answers;
vector<int> curAns;

int maxD;

void dfs(int i, int curMax) {
	assert(i <= N);

	if (i == N) {
		answers.push_back(curAns);
		sort(answers.back().begin(), answers.back().end());
		return;
	}

	assert(i < N);

	while (!cnt[curMax]) {
		curMax--;
	}

	assert(cnt[curMax] >= 1);

	if (cnt[curMax] >= 3) {
		return;
	}
	if (cnt[curMax] == 2) {
		// you need both
		bool isGood = true;
		curAns.push_back(curMax);
		curAns.push_back(maxD - curMax);
		for (int j = 0; j < i; j++) {
			if (--cnt[abs(curAns[j] - curAns[i])] < 0) isGood = false;
		}
		for (int j = 0; j < i+1; j++) {
			if (--cnt[abs(curAns[j] - curAns[i+1])] < 0) isGood = false;
		}
		if (isGood) {
			dfs(i+2, curMax);
		}
		for (int j = 0; j < i; j++) {
			++cnt[abs(curAns[j] - curAns[i])];
		}
		for (int j = 0; j < i+1; j++) {
			++cnt[abs(curAns[j] - curAns[i+1])];
		}
		curAns.pop_back();
		curAns.pop_back();

		return;
	}

	assert(cnt[curMax] == 1);
	for (int z = 0; z < 2; z++) {
		bool isGood = true;
		curAns.push_back(z ? maxD - curMax : curMax);
		for (int j = 0; j < i; j++) {
			if (--cnt[abs(curAns[j] - curAns[i])] < 0) isGood = false;
		}
		if (isGood) {
			dfs(i+1, curMax);
		}
		for (int j = 0; j < i; j++) {
			++cnt[abs(curAns[j] - curAns[i])];
		}
		curAns.pop_back();
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	maxD = 0;
	for (int i = 0; i < N * (N-1) / 2; i++) {
		long long v; cin >> v;
		if (v >= 1000) {
			cout << 0 << '\n';
			exit(0);
		}
		cnt[v]++;
		maxD = max(maxD, int(v));
	}

	assert(cnt[maxD] >= 1);

	if (cnt[maxD] >= 2) {
		cout << 0 << '\n';
		exit(0);
	}

	curAns.push_back(maxD);
	curAns.push_back(0);
	cnt[maxD] --;

	assert(cnt[maxD] == 0);

	dfs(2, maxD);

	sort(answers.begin(), answers.end());
	cout << answers.size() << '\n';
	for (const auto& a : answers) {
		for (int i = 0; i < N; i++) {
			cout << a[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
