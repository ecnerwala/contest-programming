#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2e5;
int N;
int F[MAXN+2];

ll cross(pair<int, int> a, pair<int, int> b) {
	return ll(a.first) * ll(b.second) - ll(b.first) * ll(a.second);
}

ll cross(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
	return cross({b.first - a.first, b.second - a.second}, {c.first - a.first, c.second - a.second});
}

int main() {
	if(fopen("balance.in", "r")) {
		FILE* a = freopen("balance.in", "r", stdin);
		FILE* b = freopen("balance.out", "w", stdout);
		assert(a);
		assert(b);
	}
	cin >> N;
	F[0] = F[N+1] = 0;
	for (int i = 1; i <= N; i++) {
		cin >> F[i];
	}

	vector<pair<int, int>> hull;
	hull.emplace_back(0, F[0]);
	for (int i = 1; i <= N+1; i++) {
		pair<int, int> nxt(i, F[i]);
		while (hull.size() >= 2) {
			pair<int, int> cur = hull.back();
			hull.pop_back();
			const pair<int, int>& prv = hull.back();
			if (cross(prv, cur, nxt) < 0) {
				// save it
				hull.push_back(cur);
				break;
			}
		}
		hull.push_back(nxt);
	}

	for (int i = 1, ind = 1; i <= N; i++) {
		assert(hull[ind-1].first < i);
		assert(hull[ind].first >= i);
		if (hull[ind].first == i) {
			cout << ll(hull[ind].second) * 100000LL << '\n';
			ind++;
		} else {
			if (hull[ind-1].second < hull[ind].second) {
				__int128_t a = 100000LL;
				a *= i - hull[ind-1].first;
				a *= hull[ind].second - hull[ind-1].second;
				a /= hull[ind].first - hull[ind-1].first;
				a += ll(hull[ind-1].second) * 100000LL;
				cout << ll(a) << '\n';
			} else {
				__int128_t a = 100000LL;
				a *= hull[ind].first - i;
				a *= hull[ind-1].second - hull[ind].second;
				a /= hull[ind].first - hull[ind-1].first;
				a += ll(hull[ind].second) * 100000LL;
				cout << ll(a) << '\n';
			}
		}
	}

	return 0;
}
