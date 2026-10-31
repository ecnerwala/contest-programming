#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> S(N);
	for (int i = 0; i < N; i++) cin >> S[i];
	vector<int> T(N);
	for (int i = 0; i < N; i++) cin >> T[i];

	vector<pair<int, int>> ans;
	vector<pair<int, int>> ansSuff;

	auto go = [&]() -> bool {
		unordered_set<int> startSet(S.begin(), S.end());
		for (int i = 0; i < N; i++) {
			if (!startSet.count(T[i])) return false;
		}
		
		unordered_set<int> valSet(T.begin(), T.end());

		if (int(valSet.size()) == N) {
			if (S == T) {
				return true;
			} else {
				return false;
			}
			assert(false);
		}
		assert(int(valSet.size()) < N);

		int extra = 1;
		while (startSet.count(extra)) extra++;
		assert(extra <= 100);

		N--; // the end is special

		auto findFirstSame = [&](const vector<int>& a) -> int {
			for (int i = 0; i < N; i++) {
				if (!valSet.count(a[i])) return i;
				for (int j = i+1; j < N; j++) {
					if (a[i] == a[j]) return i;
				}
			}
			assert(false);
		};

		{
			int end = S.back(); S.pop_back();
			auto it = find(S.begin(), S.end(), end);
			if (it != S.end()) {
				// don't have to do anything
			} else {
				int i = findFirstSame(S);
				ans.emplace_back(S[i], end);
				S[i] = end;
			}
		}

		{
			int end = T.back(); T.pop_back();
			auto it = find(T.begin(), T.end(), end);
			if (it != T.end()) {
				ansSuff.emplace_back(extra, end);
			} else {
				int i = findFirstSame(T);
				int v = T[i];
				T[i] = end;
				ansSuff.emplace_back(extra, end);
				ansSuff.emplace_back(end, v); // set T[i] = v
			}
		}

		for (int z = N-1; z >= 0; z--) {
			if (S[z] == T[z]) continue;
			int old = S[z];
			ans.emplace_back(extra, old);
			for (int i = 0; i <= z; i++) {
				if (S[i] == old) {
					ans.emplace_back(S[i], T[z]); // set to whatever
					S[i] = T[z];
				}
			}
			if (valSet.count(old) && find(S.begin() + z + 1, S.end(), old) == S.end()) {
				// gotta write old down somewhere before we clobber it
				assert(z >= 1);
				int i = findFirstSame(S);
				assert(i < z);
				ans.emplace_back(S[i], old);
				S[i] = old;
			}
		}

		return true;
	};

	if (go()) {
		ans.insert(ans.end(), ansSuff.begin(), ansSuff.end());
		printf("%d\n", int(ans.size())*2);
		for (auto it : ans) {
			printf("foreach ($a as &$x) if ($x == %d) break;\n", it.first);
			printf("foreach ($a as  $x) if ($x == %d) break;\n", it.second);
		}
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
