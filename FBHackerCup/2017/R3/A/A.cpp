#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e3;
int N;
int P[MAXN];
int Q[MAXN];

string go() {
	for (int i = 0; i <= N; i++) {
		Q[P[i]] = i;
	}
	char cur = 'A';
	string res("@", size_t(N));
	res[Q[1]] = cur;
	for (int i = 2; i <= N; i++) {
		int a = Q[i-1], b = Q[i];
		assert(0 <= a && a < N);
		assert(0 <= b && b < N);
		assert(P[a] == i-1);
		assert(P[b] == i);
		assert(a != b);
		assert(P[b+1] != P[a+1]);
		if (P[b+1] < P[a+1]) {
			if (cur == 'Z') {
				return "-1";
			} else {
				cur ++;
			}
		}
		res[b] = cur;
	}
	return res;
}

bool isSalient(const string &s) {
	vector<pair<string, int>> suffixes;
	for (int i = 0; i < N; i++) {
		suffixes.emplace_back(s.substr(size_t(i)), i);
	}
	sort(suffixes.begin(), suffixes.end());
	for (int i = 0; i < N; i++) {
		if (P[suffixes[size_t(i)].second] != i + 1) {
			return false;
		}
	}
	return true;
}

int main() {
	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cin >> N;
		assert(N + 1 < MAXN);
		for(int i = 0; i < N; i++) {
			cin >> P[i];
			assert(1 <= P[i] && P[i] <= N);
		}
		P[N] = 0;

		string res = go();
		cerr << res << '\n';
		assert(res == "-1" || isSalient(res));
		cout << "Case #" << T << ": " << res << '\n';
	}

	return 0;
}
