#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
using i128 = __int128_t;

clock_t start_time;

ll W;
ll A[3][3];

ll bestVal;
ll bestX[3];

ll testXYZ(ll x, ll y, ll z) {
	assert(x > 0 && y > 0 && z > 0);
	ll v = 0;
	for (int i = 0; i < 3; i++) {
		v += max(max(A[i][0] / x, A[i][1] / y), A[i][2] / z);
	}

	if (v < bestVal) {
		bestVal = v;
		bestX[0] = x, bestX[1] = y, bestX[2] = z;
	}

	return v;
}

ll solveYZ(ll X) {
	ll YZ = W - X;
	ll ownBest = ll(1e18);
	if (YZ < 2) {
		return ownBest;
	}

	vector<pair<pair<ll, ll>, pair<int, ll>>> ranges;
	for (int i = 0; i < 3; i++) {
		ll xval = A[i][0] / X;

		ll cutoff = ll(i128(YZ) * i128(A[i][1]) / (A[i][1] + A[i][2]));
		assert(0 <= cutoff && cutoff <= YZ);
		// <= cutoff, Y is bigger
		// > cutoff, Z is bigger
		if (cutoff > 0) {
			// [1,cutoff]
			if (A[i][1] / cutoff >= xval) {
				// insert [1, cutoff], A[i][1] / y
				ranges.emplace_back(pair<ll, ll>(1, cutoff), pair<int, ll>(1, A[i][1]));
			} else if (A[i][1] > xval) {
				// find max y where A[i][1] / y >= xval <=> A[i][1] / xval >= y
				ll ycut = A[i][1] / xval;
				// insert [1, ycut], A[i][1] / y
				ranges.emplace_back(pair<ll, ll>(1, ycut), pair<int, ll>(1, A[i][1]));
				// insert [ycut + 1, cutoff], xval
				ranges.emplace_back(pair<ll, ll>(ycut + 1, cutoff), pair<int, ll>(0, xval));
			} else {
				// insert [1, cutoff], xval
				ranges.emplace_back(pair<ll, ll>(1, cutoff), pair<int, ll>(0, xval));
			}
		}
		if (cutoff + 1 < YZ) {
			if (A[i][2] / (YZ - cutoff - 1) >= xval) {
				// insert [cutoff + 1, YZ-1], A[i][2] / z
				ranges.emplace_back(pair<ll, ll>(cutoff + 1, YZ - 1), pair<int, ll>(2, A[i][2]));
			} else if (A[i][2] > xval) {
				ll zcut = YZ - A[i][2] / xval;
				// insert [cutoff + 1, zcut - 1], xval
				ranges.emplace_back(pair<ll, ll>(cutoff + 1, zcut - 1), pair<int, ll>(0, xval));
				// insert [zcut, YZ - 1], A[i][2] / z
				ranges.emplace_back(pair<ll, ll>(zcut, YZ - 1), pair<int, ll>(2, A[i][2]));
			} else {
				// insert [cutoff + 1, YZ - 1], xval
				ranges.emplace_back(pair<ll, ll>(cutoff + 1, YZ - 1), pair<int, ll>(0, xval));
			}
		}
	}

	set<ll> cutoffs;
	for (auto it : ranges) {
		assert(it.first.first <= it.first.second);
		cutoffs.insert(it.first.first);
	}
	for (ll mid : cutoffs) {
		ll lo = 1, hi = YZ - 1;
		ll xcoeff = 0;
		vector<ll> ycoeffs;
		vector<ll> zcoeffs;
		for (auto it : ranges) {
			if (it.first.first <= mid && mid <= it.first.second) {
				lo = max(lo, it.first.first);
				hi = min(hi, it.first.second);
				auto term = it.second;
				if (term.first == 0) xcoeff += term.second;
				else if (term.first == 1) ycoeffs.push_back(term.second);
				else if (term.first == 2) zcoeffs.push_back(term.second);
				else assert(false);
			}
		}

		ownBest = max(ownBest, testXYZ(X, lo, YZ - lo));
		ownBest = max(ownBest, testXYZ(X, hi, YZ - hi));

		ll ysum = 0;
		ll zsum = 0;
		for (ll a : ycoeffs) ysum += a;
		for (ll a : zcoeffs) zsum += a;
		if (ysum == 0 || zsum == 0) { continue; }

		ll ymid = ll(YZ * sqrt(ld(ysum)) / (sqrt(ld(ysum)) + sqrt(ld(zsum))));
		ymid = min(max(ymid, 1ll), YZ - 1);

		//ll midVal = testXYZ(X, ymid, YZ - ymid);
		//ownBest = max(ownBest, midVal)
		//if (midVal - 5 > bestVal) continue;

		const int NUM_ADJ = 4;

		for (ll a : ycoeffs) {
			ll cur = a / (a / ymid + 1) + 1;
			for (int z = 0; cur >= 1 && cur <= YZ - 1 && z < NUM_ADJ; z++) {
				ownBest = max(ownBest, testXYZ(X, cur, YZ - cur));
				if (a / cur == 0) {
					break;
				} else {
					cur = a / (a / cur) + 1;
				}
			}

			cur = a / (a / ymid + 1) + 1;
			for (int z = 0; cur >= 1 && cur <= YZ - 1 && z < NUM_ADJ; z++) {
				ownBest = max(ownBest, testXYZ(X, cur, YZ - cur));
				if (cur == 1) break;
				cur = a / (a / (cur-1) + 1) + 1;
			}
		}

		/*
		ll zmid = YZ - ymid;

		for (ll a : zcoeffs) {
			ll cur = a / (a / zmid + 1) + 1;
			for (int z = 0; cur >= 1 && cur <= YZ - 1 && z < NUM_ADJ; z++) {
				ownBest = max(ownBest, testXYZ(X, YZ - cur, cur));
				if (a / cur == 0) {
					break;
				} else {
					cur = a / (a / cur) + 1;
				}
			}

			cur = a / (a / zmid + 1) + 1;
			for (int z = 0; cur >= 1 && cur <= YZ - 1 && z < NUM_ADJ; z++) {
				ownBest = max(ownBest, testXYZ(X, YZ - cur, cur));
				if (cur == 1) break;
				cur = a / (a / (cur-1) + 1) + 1;
			}
		}
		*/
	}
	return ownBest;
}

int main() {
	start_time = clock();
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> W;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> A[i][j];
			A[i][j] --;
		}
	}

	int origInd[3] = {0, 1, 2};
	ll s[3] = {0, 0, 0};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			s[j] += A[i][j];
		}
	}
	if (s[1] > s[0]) {
		for (int i = 0; i < 3; i++) {
			swap(A[i][0], A[i][1]);
		}
		swap(s[0], s[1]);
		swap(origInd[0], origInd[1]);
	}
	if (s[2] > s[0]) {
		for (int i = 0; i < 3; i++) {
			swap(A[i][0], A[i][2]);
		}
		swap(s[0], s[2]);
		swap(origInd[0], origInd[2]);
	}

	int toOrig[3] = {0, 0, 0};
	for (int i = 0; i < 3; i++) {
		toOrig[origInd[i]] = i;
	}

	bestVal = 1e18;

	vector<int> xs;
	for (int i = 0; i < 3; i++) {
		for (ll x = 1; x + 2 <= W; ) {
			xs.push_back(int(x));
			if (A[i][0] / x == 0) {
				break;
			} else {
				x = A[i][0] / (A[i][0] / x) + 1;
			}
		}
	}
	sort(xs.begin(), xs.end());
	xs.resize(unique(xs.begin(), xs.end()) - xs.begin());
	const int B = 10;
	vector<pair<ll, int>> buckets;
	for (int i = 0; i < int(xs.size()); i += B) {
		buckets.emplace_back(solveYZ(xs[i]), i);
	}
	sort(buckets.begin(), buckets.end());
	for (auto it : buckets) {
		for (int i = it.second + 1; i < it.second + B && i < int(xs.size()); i++) {
			solveYZ(xs[i]);
		}
		if (clock() - start_time >= 4.95 * CLOCKS_PER_SEC) {
			break;
		}
	}

	bestVal += 3; // ceiling

	cout << bestVal << '\n';
	cout << bestX[toOrig[0]] << ' ' << bestX[toOrig[1]] << ' ' << bestX[toOrig[2]] << '\n';

	return 0;
}
