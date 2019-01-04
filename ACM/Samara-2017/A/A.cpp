#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;

using pii = pair<int, int>;
vector<pair<pii, int>> frags[2];

[[noreturn]] void BAD() {
	cout << "NO" << '\n';
	exit(0);
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	int tot = 0;
	for (int i = 1; i <= N; i++) {
		string S; cin >> S;
		int lo = 0;
		int net = 0;
		for (char c : S) {
			if (c == '(') {
				net++;
			} else {
				net--;
				lo = min(lo, net);
			}
		}
		tot += net;
		if (net >= 0) {
			assert(lo <= 0);
			frags[0].emplace_back(pii(0-lo, net), i);
		} else {
			assert(lo <= net);
			frags[1].emplace_back(pii(net-lo, -net), i);
		}
	}
	if (tot != 0) {
		BAD();
	}

	for (int z = 0; z < 2; z++) {
		int cur = 0;
		sort(frags[z].begin(), frags[z].end());
		for (auto it : frags[z]) {
			int need = it.first.first;
			int net = it.first.second;
			assert(need >= 0);
			assert(net >= 0);
			if (cur < need) {
				// can't satisfy
				BAD();
			}
			assert(cur >= need);
			cur += net;
		}
	}

	vector<int> res;
	for (auto it : frags[0]) {
		res.push_back(it.second);
	}
	reverse(frags[1].begin(), frags[1].end());
	for (auto it : frags[1]) {
		res.push_back(it.second);
	}
	cout << "YES\n";
	for (size_t i = 0; i < res.size(); i++) {
		cout << res[i] << " \n"[i+1 == res.size()];
	}

	return 0;
}
