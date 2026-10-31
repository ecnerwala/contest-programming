#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N; cin >> N;
	vector<int> A(N);
	iota(A.begin(), A.end(), 1);
	shuffle(A.begin(), A.end(), mt19937(48));

	vector<pair<int, int>> B(N);
	for (int i = 0; i < N; i++) {
		B[i] = {i+1, 0};
	}

	for (int cur : A) {
		vector<int> fixed;
		fixed.push_back(-1);
		for (int j = 0; j < N; j++) {
			if (B[j].second) fixed.push_back(j);
		}
		fixed.push_back(N);
		int mi = 0, ma = int(fixed.size())-1;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			cout << '?' << ' ' << cur << ' ' << B[fixed[md]].first << '\n' << flush;
			char c; cin >> c;
			if (c == '<') {
				ma = md;
			} else if (c == '>') {
				mi = md;
			} else assert(false);
		}
		int lo = fixed[mi]+1, hi = fixed[ma]-1;
		assert(lo <= hi);
		vector<pair<int, int>> los;
		vector<pair<int, int>> his;
		pair<int, int> me;
		for (int j = lo; j <= hi; j++) {
			cout << '?' << ' ' << cur << ' ' << B[j].first << '\n' << flush;
			char c; cin >> c;
			if (c == '<') {
				his.push_back(B[j]);
			} else if (c == '>') {
				los.push_back(B[j]);
			} else if (c == '=') {
				me = {B[j].first, cur};
			} else assert(false);
		}
		auto it = B.begin() + lo;
		it = copy(los.begin(), los.end(), it);
		*it = me; ++it;
		it = copy(his.begin(), his.end(), it);
		assert(it == B.begin()+hi + 1);
	}

	vector<int> realAns(N);
	for (auto it : B) realAns[it.second-1] = it.first;

	cout << '!';
	for (int i = 0; i < N; i++) {
		cout << ' ' << realAns[i];
	}
	cout << '\n' << flush;

	return 0;
}
