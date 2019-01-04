#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 3e5;
int N;
ll L[MAXN];
ll R[MAXN];

ll floordiv(ll a, ll b) {
	if (b < 0) a = -a, b = -b;
	if (a >= 0) {
		return a / b;
	} else {
		return (a-b+1) / b;
	}
}
ll ceildiv(ll a, ll b) {
	if (b < 0) a = -a, b = -b;
	if (a >= 0) {
		return (a+b-1) / b;
	} else {
		return a / b;
	}
}

// take the convex hull of the points i, p

struct pt {
	ll x, y;
	friend pt operator - (const pt& a, const pt& b) { return pt{a.x - b.x, a.y - b.y}; }
	friend pt operator + (const pt& a, const pt& b) { return pt{a.x + b.x, a.y + b.y}; }
};
ll cross(pt a, pt b) {
	return a.x * b.y - a.y * b.x;
}
ll cross(pt a, pt b, pt c) {
	return cross(b-a, c-a);
}
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> L[i] >> R[i];
	}
	assert(N >= 2);
	vector<pt> lhull;
	vector<pt> rhull;
	for (int i = 0; i < N; i++) {
		{
			pt nxt{i, L[i]};
			while (lhull.size() >= 2) {
				pt cur = lhull.back(); lhull.pop_back();
				pt& prv = lhull.back();
				if (cross(prv, cur, nxt) < 0) {
					lhull.push_back(cur);
					break;
				}
			}
			lhull.push_back(nxt);
		}

		{
			pt nxt{i, R[i]};
			while (rhull.size() >= 2) {
				pt cur = rhull.back(); rhull.pop_back();
				pt& prv = rhull.back();
				if (cross(prv, cur, nxt) > 0) {
					rhull.push_back(cur);
					break;
				}
			}
			rhull.push_back(nxt);
		}
	}

	//for (auto it : lhull) { cerr << it.x << ',' << it.y << ' '; } cerr << '\n';
	//for (auto it : rhull) { cerr << it.x << ',' << it.y << ' '; } cerr << '\n';

	assert(N >= 2);
	assert(lhull.size() >= 2);
	assert(rhull.size() >= 2);

	ll res = 0;
	// convert the hull to bounds based on slope
	int lind = 0, rind = int(rhull.size()) - 1;
	ll curSlope = 1.1e9; // inclusive
	while (true) {
		ll nxtSlope = -1.1e9;
		if (lind < int(lhull.size()) - 1) {
			pt p = lhull[lind], q = lhull[lind+1];
			ll trans = floordiv(q.y - p.y, q.x - p.x);
			nxtSlope = max(nxtSlope, trans);
		}
		if (rind > 0) {
			pt p = rhull[rind], q = rhull[rind-1];
			ll trans = floordiv(q.y - p.y, q.x - p.x);
			nxtSlope = max(nxtSlope, trans);
		}

		assert(nxtSlope <= curSlope);
		// nxtSlope is exclusive
		if (curSlope > nxtSlope) {
			// the current bound is lhull[ind].y - lhull[ind].x * slope <= a <= rhull[ind].y - rhull[ind].x * slope
			// number of things is max(rhull[ind].y - lhull[ind].y - (rhull[ind].x - lhull[ind].y) * slope + 1, 0)
			pt bound = rhull[rind] - lhull[lind];
			bound.x *= -1;
			// max(bound.y + bound.x * slope + 1, 0)

			// inclusive range
			ll lo = nxtSlope + 1;
			ll hi = curSlope;
			//cerr << lo << ' ' << hi << '\n';
			if (bound.x < 0) {
				bound.x *= -1;
				lo *= -1;
				hi *= -1;
				swap(lo, hi);
			}
			assert(bound.x >= 0);

			ll loVal = bound.y + lo * bound.x + 1;
			ll hiVal = bound.y + hi * bound.x + 1;
			//cerr << lo << ' ' << hi << ' ' << bound.y << ' ' << bound.x << '\n';
			// we want y - slope * x >= 0
			if (hiVal <= 0) {
				// sucks
			} else {
				assert(hiVal >= 1);
				if (loVal <= 0) {
					// lo * bound.x >= -bound.y
					// lo >= (-bound.y) / bound.x
					lo = ceildiv(-bound.y, bound.x);
					assert(lo <= hi);
					loVal = bound.y + lo * bound.x + 1;
					assert(loVal >= 1);
				}
				//cerr << lo << ' ' << hi << ' ' << loVal << ' ' << hiVal << '\n';
				res += (loVal + hiVal) * (hi - lo + 1) / 2;
			}
		}

		curSlope = nxtSlope;
		if (lind == int(lhull.size()) - 1 && rind == 0) break;
		if (lind < int(lhull.size()) - 1) {
			pt p = lhull[lind], q = lhull[lind+1];
			ll trans = floordiv(q.y - p.y, q.x - p.x);
			if (trans == curSlope) lind ++;
		}
		if (rind > 0) {
			pt p = rhull[rind], q = rhull[rind-1];
			ll trans = floordiv(q.y - p.y, q.x - p.x);
			if (trans == curSlope) rind --;
		}
	}
	cout << res << '\n';
	return 0;
}
