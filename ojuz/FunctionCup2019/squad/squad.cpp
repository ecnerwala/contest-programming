#include "squad.h"

#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int S = 1 << 19;
vector<pair<int, int>> segs[2*S];

ll area(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
	return (ll(b.first) - ll(a.first)) * (ll(c.second) - ll(a.second)) - (ll(c.first) - ll(a.first)) * (ll(b.second) - ll(a.second));
}

void updateHull(vector<pair<int, int>>& hull, pair<int, int> p) {
	assert(hull.empty() || p >= hull.back());
	while (!hull.empty() && hull.back().second <= p.second) {
		hull.pop_back();
	}
	if (!hull.empty()) {
		assert(hull.back() <= p);
		assert(hull.back().second > p.second);
		assert(hull.back().first < p.first);
	}

	while (hull.size() >= 2 && area(hull[hull.size()-2], hull.back(), p) >= 0) {
		hull.pop_back();
	}

	hull.push_back(p);
}

bool compareAngle(pair<int, int> a, pair<int, int> b) {
	return 1ll * a.first * b.second < 1ll * a.second * b.first;
}

vector<pair<int, int>> hull;

void Init(vector<int> A, vector<int> D, vector<int> P){
	int N = int(A.size());

	vector<int> inds(N); iota(inds.begin(), inds.end(), 0);
	// defend on the left, attack on the right
	sort(inds.begin(), inds.end(), [&](int i, int j) { return A[i] - D[i] < A[j] - D[j]; });

	vector<pair<pair<int, int>, int>> evts[2];
	for (int z = 0; z < N; z++) {
		//cerr << z << ' ' << D[inds[z]] << ' ' << A[inds[z]] << ' ' << P[inds[z]] << '\n';
		evts[0].emplace_back(pair<int, int>(D[inds[z]], P[inds[z]]), z);
		evts[1].emplace_back(pair<int, int>(A[inds[z]], P[inds[z]]), z);
	}

	for (int z = 0; z < 2; z++) {
		sort(evts[z].begin(), evts[z].end());
		for (auto it : evts[z]) {
			pair<int, int> p = it.first;
			int i = it.second;
			for (int a = S+i; a > 1; a /= 2) {
				if ((a & 1) == z) {
					// insert p into segs[a]
					updateHull(segs[a], p);
				}
			}
		}
	}

	vector<pair<int, int>> pts;
	for (int a = S-1; a; segs[2*a] = {}, segs[2*a+1] = {}, a--) {
		auto& l = segs[2*a];
		auto& r = segs[2*a+1];
		if (l.empty() || r.empty()) continue;
		//cerr << "a " << a << '\n';
		//cerr << "L" << '\n';
		//for (auto p : l) cerr << p.first << ' ' << p.second << '\n';
		//cerr << "R" << '\n';
		//for (auto p : r) cerr << p.first << ' ' << p.second << '\n';

		for (int i = 0, j = 0; true; ) {
			pts.emplace_back(l[i].first + r[j].first, l[i].second + r[j].second);
			//cerr << l[i].first << ' ' << l[i].second << ' ' << r[j].first << ' ' << r[j].second << ' ' << pts.back().first << ' ' << pts.back().second << '\n';

			if (i+1 < int(l.size()) && j+1 < int(r.size())) {
				if (compareAngle(
							pair<int, int>(l[i].second - l[i+1].second, l[i+1].first - l[i].first),
							pair<int, int>(r[j].second - r[j+1].second, r[j+1].first - r[j].first)
						)){
					i++;
				} else {
					j++;
				}
			} else if (j+1 < int(r.size())) {
				j++;
			} else if (i+1 < int(l.size())) {
				i++;
			} else {
				break;
			}
		}
	}

	sort(pts.begin(), pts.end());
	for (auto it : pts) updateHull(hull, it);

	//for (auto it : hull) { cerr << it.first << ' ' << it.second << '\n'; }
}

long long BestSquad(int X, int Y){
	int mi = -1, ma = int(hull.size())-1;
	auto eval = [&](int i) -> long long {
		return 1ll * hull[i].first * X + 1ll * hull[i].second * Y;
	};
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (eval(md) <= eval(md+1)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return eval(ma);
}
