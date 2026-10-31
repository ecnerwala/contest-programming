#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<string> A(N); for (auto& a : A) cin >> a;

		int ans = 0;

		string cur = A[0];
		for (int i = 1; i < N; i++) {
			// increment cur, because strictly increasing
			for (int x = int(cur.size())-1; true; x--) {
				if (x < 0) {
					cur.insert(cur.begin(), '1');
					break;
				} else if (cur[x] == '9') {
					cur[x] = '0';
					continue;
				} else {
					cur[x]++;
					break;
				}
			}

			string cnd = A[i];
			if (cnd.size() > cur.size()) {
				// do nothing
				cur = cnd;
				continue;
			} else {
				// try to match it in size
				assert(cur.size() >= cnd.size());
				ans += int(cur.size() - cnd.size());
				for (int a = 0; true; a++) {
					if (a == int(cnd.size())) {
						// the prefixes match
						cnd = cur;
						break;
					} else if (cnd[a] < cur[a]) {
						// it's bad
						ans++;
						cnd.insert(cnd.end(), cur.size() - cnd.size() + 1, '0');
						break;
					} else if (cnd[a] > cur[a]) {
						// it's fine, we can just insert 0's
						cnd.insert(cnd.end(), cur.size() - cnd.size(), '0');
						break;
					} else {
						continue;
					}
				}
				cur = cnd;
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
