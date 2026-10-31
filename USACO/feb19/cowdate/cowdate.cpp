#include<bits/stdc++.h>
using namespace std;

using ld = long double;

const int MAXN = 1.1e6;
int N;
ld P[MAXN];

int main() {
	if(fopen("cowdate.in", "r")) {
		freopen("cowdate.in", "r", stdin);
		freopen("cowdate.out", "w", stdout);
	}

	cin >> N;
	for (int i = 0; i < N; i++) {
		int p_; cin >> p_;
		P[i] = 1e-6 * ld(p_);
	}

	ld prod = 1, sum = 1;
	deque<pair<ld, ld>> hull;
	ld res = 0;
	for (int i = 0; i < N; i++) {
		{ // insert the current
			pair<ld, ld> nxt(-sum / prod, 1 / prod);
			while (hull.size() >= 2) {
				// second increases over time
				auto cur = hull.back(); hull.pop_back();
				auto& prv = hull.back();
				ld x = (prv.second - nxt.second) / (nxt.first - prv.first);
				if (cur.first + cur.second * x < nxt.first + nxt.second * x) {
					continue;
				} else {
					hull.push_back(cur);
					break;
				}
			}
			hull.push_back(nxt);
		}
		prod *= (1-P[i]);
		sum += P[i] / (1 - P[i]);
		{ // query the current
			while (hull.size() >= 2) {
				auto cur = hull.front(); hull.pop_front();
				auto& nxt = hull.front();
				if (cur.first + cur.second * sum < nxt.first + nxt.second * sum) {
					continue;
				} else {
					hull.push_front(cur);
					break;
				}
			}
			res = max(res, prod * (hull.front().first + hull.front().second * sum));
		}
	}

	cout << int(res * 1e6 + 1e-9) << '\n';

	return 0;
}
