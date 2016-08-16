#include<bits/stdc++.h>
using namespace std;

map<string, int> mp;


int get_ind(const string &s) {
	if(mp.count(s)) return mp[s];
	int v = int(mp.size());
	return mp[s] = v;
}

int main() {
	ios_base::sync_with_stdio(0);
	string s;
	int N; cin >> N;
	getline(cin, s);

	vector<int> diary;
	for(int i = 0; i < N; i++) {
		getline(cin, s);
		s += '\n';
		string cur;
		for(char c : s) {
			if('a' <= c && c <= 'z') {
				cur += c;
			} else if('A' <= c && c <= 'Z') {
				cur += char(c - 'A' + 'a');
			} else {
				if(!cur.empty()) {
					diary.push_back(get_ind(cur));
					cur.clear();
				}
			}
		}
		assert(cur.empty());
	}

	vector<int> blacklist;
	int K; cin >> K;
	for(int i = 0; i < K; i++) {
		cin >> s;
		blacklist.push_back(get_ind(s));
	}

	sort(blacklist.begin(), blacklist.end());

	N = 0;
	for(int i = 0; i < int(diary.size()); i++) {
		if(!binary_search(blacklist.begin(), blacklist.end(), diary[i])) {
			diary[N] = diary[i];
			N++;
		}
	}
	diary.resize(N);

	assert(N >= 2);

	unordered_multiset<int> cnt;
	unordered_multiset<long long> cnt2;

	long long S = mp.size();

	for(int i = 0; i < N; i++) {
		cnt.insert(diary[i]);
	}

	for(int i = 0; i + 1 < N; i++) {
		int a = diary[i], b = diary[i + 1];
		if(a > b) swap(a, b);
		cnt2.insert(a * S + b);
	}

	int Q; cin >> Q;
	for(int i = 0; i < Q; i++) {
		int a, b;
		cin >> s; a = get_ind(s);
		cin >> s; b = get_ind(s);
		if(a > b) swap(a, b);

		double res = N;
		res /= double(cnt.count(a));
		res *= N;
		res /= double(cnt.count(b));
		res /= N - 1;
		res *= double(cnt2.count(a * S + b));
		cout << fixed << setprecision(20) << res << '\n';
	}
	return 0;
}
