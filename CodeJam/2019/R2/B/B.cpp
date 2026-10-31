#include<bits/stdc++.h>
using namespace std;

void start_day(int d) {
	int day_num; cin >> day_num;
	if (day_num == -1) exit(0);
	assert(day_num == d);
}

vector<int> read_tokens() {
	int sz; cin >> sz;
	vector<int> res; res.reserve(sz);
	for (int i = 0; i < sz; i++) {
		int v; cin >> v; res.push_back(v);
	}
	return res;
}

void go() {
	mt19937 mt(48);

	for (int d = 1; d <= 20; d++) {
		start_day(d);
		cout << d << ' ' << 100 << endl << flush;
	}

	vector<pair<double, int>> sizes;
	vector<set<int>> tokens(21);

	vector<int> vases(20);
	iota(vases.begin(), vases.end(), 1);
	shuffle(vases.begin(), vases.end(), mt);

	for (int d = 21; d <= 40; d++) {
		start_day(d);
		//int vase = d - 20;
		int vase = vases[d - 21];
		cout << vase << ' ' << 0 << endl << flush;
		vector<int> toks = read_tokens();
		tokens[vase] = set<int>(toks.begin(), toks.end());
		sizes.emplace_back(int(toks.size()) - (d * 0.05), vase);
	}
	sort(sizes.begin(), sizes.end());

	for (int d = 41; d <= 60; d++) {
		start_day(d);
		int vase = sizes[1].second;
		int val = 1;
		while (val < 100 && tokens[vase].count(val)) val++;
		tokens[vase].insert(val);
		cout << vase << ' ' << val << endl << flush;
		sizes[1].first ++;
		sort(sizes.begin(), sizes.end());
	}

	auto oldSizes = sizes;
	sizes = {};
	for (int d = 61; d <= 80; d++) {
		start_day(d);
		//int vase = d - 60;
		//int vase = vases[d - 61];
		int vase = oldSizes[80 - d].second;
		cout << vase << ' ' << 0 << endl << flush;
		vector<int> toks = read_tokens();
		tokens[vase] = set<int>(toks.begin(), toks.end());
		sizes.emplace_back(int(toks.size()) - (d * 0.05), vase);
	}
	sort(sizes.begin(), sizes.end());

	for (int d = 81; d <= 100; d++) {
		start_day(d);
		int vase = sizes[1].second;
		int val = 1;
		while (val < 100 && tokens[vase].count(val)) val++;
		if (d == 100) val = 100;
		tokens[vase].insert(val);
		cout << vase << ' ' << val << endl << flush;
		sizes[1].first ++;
		sort(sizes.begin(), sizes.end());
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		go();
	}

	return 0;
}
