#include<bits/stdc++.h>
using namespace std;

using bs = bitset<64>;
unordered_map<bs, int> mem;
int N;
int K;
int L;

bs sortPts(bs a) {
	pair<int, int> cnts[8];
	for (int i = 0; i < N; i++) {
		cnts[i].first = 0;
		for (int j = 0; j < N; j++) {
			cnts[i].first += a[i*8+j];
		}
		cnts[i].second = i;
	}
	sort(cnts, cnts + N);
	reverse(cnts, cnts + N);
	bs b;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			b[i*8+j] = a[cnts[i].second*8+cnts[j].second];
			if (!(i <= j)) assert(!b[i*8+j]);
		}
	}
	return b;
}

int dp(bs state) {
	if (mem.count(state)) {
		return mem[state];
	}
	for (int j = 0; j < K; j++) {
		if (!state[j*8+K]) {
			goto notDone;
		}
	}
	for (int j = K+1; j < N; j++) {
		if (!state[K*8+j]) {
			goto notDone;
		}
	}
	// is done
	return mem[state] = 0;
notDone:
	int ans = 1e9;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (state[i*8+j]) continue;
			int cost = 0;
			{
				bs nstate = state;
				nstate[i*8+j] = true;
				for (int a = 0; a <= i; a++) {
					for (int b = j; b < N; b++) {
						if (state[a*8+i] && state[j*8+b]) {
							nstate[a*8+b] = true;
						}
					}
				}
				cost = max(cost, dp(sortPts(nstate)));
			}
			{
				bs nstate = state;
				nstate[j*8+i] = true;
				for (int a = 0; a <= j; a++) {
					for (int b = i; b < N; b++) {
						if (state[a*8+j] && state[i*8+b]) {
							nstate[a*8+b] = true;
						}
					}
				}
				cost = max(cost, dp(sortPts(nstate)));
			}
			ans = min(ans, cost + 1);
		}
	}
	return mem[state] = ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K >> L;
	K--;

	bs start;
	for (int i = 0; i < N; i++) {
		start[i*8+i] = true;
	}
	for (int e = 0; e < L; e++) {
		int x, y; cin >> x >> y;
		start[x*8+y] = true;
	}
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (start[i*8+k] && start[k*8+j]) {
					start[i*8+j] = true;
				}
			}
		}
	}
	start = sortPts(start);
	/*
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cerr << start[i*8+j] << ' ';
		}
		cerr << '\n';
	}
	*/
	cout << dp(start) << '\n';
	//cerr << int(mem.size()) << '\n';

	return 0;
}
