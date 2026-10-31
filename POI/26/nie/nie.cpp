#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto char_to_int = [](char c) -> int {
		switch (c) {
			case 'A': return 0;
			case 'T': return 1;
			case 'C': return 2;
			case 'G': return 3;
			default: assert(false);
		}
	};
	auto int_to_char = [](int i) -> char { return "ATCG"[i]; };

	array<string, 2> S; cin >> S[0] >> S[1];
	array<vector<array<int, 4>>, 2> pref;
	array<vector<array<int, 4>>, 2> nxt;
	array<vector<array<int, 4>>, 2> prv;
	for (int z = 0; z < 2; z++) {
		int N = int(S[z].size());
		pref[z].resize(N+1);
		pref[z][0].fill(0);
		for (int i = 0; i < N; i++) {
			pref[z][i+1] = pref[z][i];
			pref[z][i+1][char_to_int(S[z][i])]++;
		}
		nxt[z].resize(N+1);
		nxt[z][N].fill(N+1);
		for (int i = N-1; i >= 0; i--) {
			nxt[z][i] = nxt[z][i+1];
			nxt[z][i][char_to_int(S[z][i])] = i+1;
		}
		prv[z].resize(N+1);
		prv[z][0].fill(-1);
		for (int i = 0; i < N; i++) {
			prv[z][i+1] = prv[z][i];
			prv[z][i+1][char_to_int(S[z][i])] = i;
		}
	}

	string resp; resp.reserve(min(S[0].size(), S[1].size()));
	array<int, 2> cur{0,0};
	vector<pair<int, array<int, 2>>> stk; stk.reserve(min(S[0].size(), S[1].size())+1);
	stk.push_back({-1, {int(S[0].size()), int(S[1].size())}});
	while (true) {
		assert(!stk.empty());
		assert(cur[0] <= stk.back().second[0]);
		assert(cur[1] <= stk.back().second[1]);
		auto& hi0 = pref[0][stk.back().second[0]];
		auto& hi1 = pref[1][stk.back().second[1]];
		auto& lo0 = pref[0][cur[0]];
		auto& lo1 = pref[1][cur[1]];
		int v = 0;
		while (v < 4 && !(hi0[v] - lo0[v] > 0 && hi1[v] - lo1[v] > 0)) v++;
		if (v < 4) {
			stk.push_back({v, {prv[0][stk.back().second[0]][v], prv[1][stk.back().second[1]][v]}});
		} else {
			int n = stk.back().first;
			stk.pop_back();
			if (stk.empty()) break;
			resp += int_to_char(n);
			for (int z : {0, 1}) cur[z] = nxt[z][cur[z]][n];
		}
	}

	cout << resp << '\n';

	return 0;
}
