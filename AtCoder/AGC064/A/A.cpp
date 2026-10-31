#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int L = N * (N+1) / 2;
	std::vector<int> prv(L, -2);
	std::vector<int> nxt(L, -2);
	std::vector<int> val(L);
	int st = 0;
	int en = N;
	for (int i = 0; i < N; i++) {
		prv[i] = (i+N-1)%N;
		nxt[i] = (i+1)%N;
		val[i] = N;
	}
	for (int i = N-1; i >= 1; i--) {
		assert(en - st == i+1);
		for (int j = 0; j < i; j++) {
			int cur = en + j;
			val[cur] = i;
			int p, n;
			if (i == N-1) {
				int nei = st + j;
				p = nei;
				n = nxt[nei];
			} else if (i == N-2 && j == i-1) {
				int nei = N-1;
				p = nei;
				n = nxt[nei];
			} else {
				int nei = st + j;
				assert(val[nei] == i+1);
				if (prv[nei] != -1 && val[prv[nei]] == i+2) {
					p = prv[nei], n = nei;
				} else if (nxt[nei] != -1 && val[nxt[nei]] == i+2) {
					p = nei, n = nxt[nei];
				} else assert(false);
			}
			prv[cur] = p;
			nxt[cur] = n;
			prv[n] = cur;
			nxt[p] = cur;
		}
		st = en;
		en += i;
	}

	assert(en == L);

	int cur = 0;
	std::vector<int> ans; ans.reserve(L);
	do {
		ans.push_back(val[cur]);
		cur = nxt[cur];
	} while (cur != 0);
	assert(int(ans.size()) == L);
	for (int i = 0; i < L; i++) {
		cout << ans[i] << " \n"[i+1==L];
	}

	return 0;
}

// 2431342434
// 4342431243
// 5453542431243
// 532421354545453
