#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;

int sum_all(const vi &G) {
	int res = 0;
	for (int i : G) res += i;
	return res;
}

ll fac[20];

ll go(const vi &G) {
	int L = int(G.size());
	int sum = sum_all(G);
	if (sum > L) return 1;

	ll res = 1; // self

	vi nxt;
	for (int i = 0; i < (L - sum); i++) {
		nxt.push_back(0);
	}
	for (int i = 1; i <= L; i++) {
		for (int j = 0; j < G[size_t(i-1)]; j++) {
			nxt.push_back(i);
		}
	}
	assert(int(nxt.size()) == L);

	int nxtSum = sum_all(nxt);
	if (nxtSum > L) {
		// each next is a 1, so we just need to count them
		ll v = fac[L];
		for (int i : G) v /= fac[i];
		v /= fac[L - sum];
		res += v;
		return res;
	} else {
		bool may_cycle = (G[0] == 1 && sum == 1);
		sort(nxt.begin(), nxt.end());
		do {
			if (may_cycle && G == nxt) continue;
			res += go(nxt);
		} while (next_permutation(nxt.begin(), nxt.end()));
		return res;
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	fac[0] = 1;
	for (int i = 1; i < 20; i++) fac[i] = fac[i-1] * i;

	for(int case_num = 1; case_num <= T; case_num ++) {
		string S;
		cin >> S;
		vi G;
		for (char it : S) {
			G.push_back(it - '0');
		}
		assert(int(G.size()) >= 1);
		ll res = go(G);
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}

// only cycle is 1000, as either sum decreases or all 1's -> x0000 -> 00010 -> 10000
