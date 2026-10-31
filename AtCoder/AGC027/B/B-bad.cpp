#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using num = __int128_t;

const int MAXN = 3e5;
int N;
ll V;
ll X[MAXN];
num pref[MAXN];

int main() {
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	cin >> N >> V;
	for (int i = 0; i < N; i++) {
		cin >> X[i];
	}

	pref[0] = X[0];
	for (int i = 1; i < N; i++) {
		pref[i] = pref[i-1] + X[i];
	}

	num worst = V + X[N-1] + X[N-1];
	for (int i = N-1; i >= 0; i--) {
		worst += V + num(X[i]) * ((N-1-i) * 2 + 3);
	}
	//cerr << N-1 << ' ' << ll(worst) << '\n';

	deque<pair<ll, num>> vals;
	vals.emplace_back(N-1, worst);

	for (ll i = N-2; i >= 0; i--) {
		num cnd = V + 2 * X[i] + pref[i] * i * 2;
		num coeff = -2 * pref[i];
		while (vals.size() >= 2) {
			auto it = vals.back(); vals.pop_back();
			if (coeff * vals.back().first + vals.back().second <= coeff * it.first + it.second) {
				// leave popped and continue
				continue;
			} else {
				vals.push_back(it);
				break;
			}
		}

		cnd += coeff * vals.back().first + vals.back().second;
		//cerr << i << ' ' << ll(cnd) << '\n';

		if (cnd >= vals.front().second) continue;

		while (vals.size() >= 2) {
			// insert the new guy
			auto it = vals.front(); vals.pop_front();
			if ((vals.front().first - i) * (it.second - cnd) >= (it.first - i) * (vals.front().second - cnd)) {
				// leave it popped
				continue;
			} else {
				vals.push_front(it);
				break;
			}
		}
		vals.emplace_front(i, cnd);
	}

	cout << ll(vals.front().second) << '\n';

	return 0;
}
