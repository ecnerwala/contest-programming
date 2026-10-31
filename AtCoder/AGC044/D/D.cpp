#include<bits/stdc++.h>

const int MAXL = 128;
const int Q = 850;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789";
	assert(chars.size() == 62);
	const bool TEST = false;

	string TEST_ANS;
	if (TEST) {
		std::random_device rd;
		mt19937 mt(rd());
		for (int i = 0; i < MAXL - 1; i++) {
			TEST_ANS += chars[mt() % 2];
		}
	}

	int QUERY_CNT = 0;

	auto query = [&](string q) -> int {
		QUERY_CNT++;
		cout << '?' << ' ' << q << '\n' << flush;
		int v; cin >> v;
		return v;
	};

	auto get_cnt = [&](char c) -> int {
		if (TEST) {
			QUERY_CNT++;
			int res = 0;
			for (char v : TEST_ANS) {
				res += v == c;
			}
			return res;
		}
		return MAXL - query(string(MAXL, c));
	};
	vector<int> cnts(62);
	for (int i = 0; i < 62; i++) {
		cnts[i] = get_cnt(chars[i]);
	}
	int L = accumulate(cnts.begin(), cnts.end(), 0);

	auto is_subseq = [&](string q) -> bool {
		assert(int(q.size()) <= L);
		if (TEST) {
			QUERY_CNT++;
			int i = 0;
			for (char c : TEST_ANS) {
				if (c == q[i]) {
					i++;
					if (i == int(q.size())) return true;
				}
			}
			return false;
		}
		return query(q) == L - int(q.size());
	};

	string ans; ans.reserve(L);
	for (int z = 0; z < 62; z++) {
		char c = chars[z];
		int cnt = cnts[z];
		int last = -1;
		for (int i = 0; i < cnt; i++) {
			int mi = last, ma = int(ans.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (is_subseq(string(i+1, c) + ans.substr(md))) {
					ma = md;
				} else {
					mi = md;
				}
			}
			ans.insert(ans.begin()+ma, c);
			last = ma;
		}
	}

	//cerr << TEST_ANS << '\n';
	//cerr << ans << '\n';
	if (TEST) {
		assert(ans == TEST_ANS);
	}
	cerr << QUERY_CNT << '\n';
	assert(QUERY_CNT <= Q);

	cout << '!' << ' ' << ans << '\n';
	return 0;
}
