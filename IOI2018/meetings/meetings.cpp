#include "meetings.h"

#include <bits/stdc++.h>

namespace ecnerwala {
using namespace std;
using ll = long long;


std::vector<long long> minimum_costs(std::vector<long long> H, std::vector<int> L, std::vector<int> R) {
	int N = int(H.size());
	vector<int> nextRight(N);
	vector<int> nextLeft(N);
	vector<int> chLeft(N, -1);
	vector<int> chRight(N, -1);
	vector<int> rangeLen(N);
	vector<ll> rangeVal(N);

	auto computeRangeVal = [&](int a) {
		int l = chLeft[a], r = chRight[a];
		assert(nextLeft[a] == -1 || H[nextLeft[a]] > H[a]);
		assert(nextRight[a] == N || H[nextRight[a]] >= H[a]);
		if (l == -1) {
			assert(nextLeft[a] == a-1);
		} else {
			assert(nextLeft[a] == nextLeft[l]);
			assert(nextRight[l] == a);
		}
		if (r == -1) {
			assert(nextRight[a] == a+1);
		} else {
			assert(nextRight[a] == nextRight[r]);
			assert(nextLeft[r] == a);
		}
		rangeLen[a] = (l == -1 ? 0 : rangeLen[l]) + 1 + (r == -1 ? 0 : rangeLen[r]);
		assert(rangeLen[a] == nextRight[a] - nextLeft[a] - 1);
		rangeVal[a] = ll(H[a]) * rangeLen[a];
		if (l != -1) {
			assert(rangeVal[l]);
			rangeVal[a] = min(rangeVal[a], rangeVal[l] + ll(H[a]) * (rangeLen[a] - rangeLen[l]));
		}
		if (r != -1) {
			assert(rangeVal[r]);
			rangeVal[a] = min(rangeVal[a], rangeVal[r] + ll(H[a]) * (rangeLen[a] - rangeLen[r]));
		}
	};

	using height_t = pair<ll, int>;
	stack<height_t> q;
	const ll INF_HEIGHT = int(1.1e18);
	q.emplace(INF_HEIGHT+1, -1);

	for (int i = 0; i <= N; i++) {
		height_t cur(i == N ? INF_HEIGHT : H[i], i);

		while (q.top() < cur) {
			nextRight[q.top().second] = i;
			computeRangeVal(q.top().second);
			if (i < N) chLeft[i] = q.top().second;
			q.pop();
		}

		if (i == N) {
			break;
		}

		nextLeft[i] = q.top().second;
		if (nextLeft[i] != -1) chRight[nextLeft[i]] = i;
		q.push(cur);
	}

	struct range_data {
		int dest;
		int len;
		ll leftVal;
		ll insideCost;
		ll leftCost;
	};
	auto merge = [](const range_data& l, const range_data& r) -> range_data {
		range_data res;
		res.dest = r.dest;
		res.len = l.len + r.len;
		res.leftVal = l.leftVal;
		res.insideCost = min(l.insideCost + r.leftCost, l.len * r.leftVal + r.insideCost);
		res.leftCost = l.leftCost + r.leftCost;
		return res;
	};

	vector<vector<range_data>> rightRanges(N);
	for (int i = N-1; i >= 0; i--) {
		rightRanges[i].push_back(range_data{
				nextRight[i],
				nextRight[i] - i,
				H[i],
				H[i] + (chRight[i] == -1 ? 0 : rangeVal[chRight[i]]),
				H[i] * (nextRight[i] - i),
		});
		for (int z = 0; rightRanges[i][z].dest < N && z < int(rightRanges[rightRanges[i][z].dest].size()); z++) {
			rightRanges[i].push_back(merge(rightRanges[i][z], rightRanges[rightRanges[i][z].dest][z]));
		}
	}

	vector<vector<range_data>> leftRanges(N);
	for (int i = 0; i < N; i++) {
		leftRanges[i].push_back(range_data{
				nextLeft[i],
				i - nextLeft[i],
				H[i],
				H[i] + (chLeft[i] == -1 ? 0 : rangeVal[chLeft[i]]),
				H[i] * (i - nextLeft[i]),
		});
		for (int z = 0; leftRanges[i][z].dest > -1 && z < int(leftRanges[leftRanges[i][z].dest].size()); z++) {
			leftRanges[i].push_back(merge(leftRanges[i][z], leftRanges[leftRanges[i][z].dest][z]));
		}
	}

	auto query = [&](int l, int r) -> ll {
		range_data lhs{
			l, 0, -INF_HEIGHT, 0, 0,
		};
		for (int z = int(rightRanges[l].size()); z >= 0; z--) {
			int curL = lhs.dest;
			if (z < int(rightRanges[curL].size()) && rightRanges[curL][z].dest <= r) {
				lhs = merge(lhs, rightRanges[curL][z]);
			}
			assert(lhs.dest == l + lhs.len);
		}

		range_data rhs{
			r, 0, -INF_HEIGHT, 0, 0,
		};

		for (int z = int(leftRanges[r].size()); z >= 0; z--) {
			int curR = rhs.dest;
			if (z < int(leftRanges[curR].size()) && leftRanges[curR][z].dest >= l) {
				rhs = merge(rhs, leftRanges[curR][z]);
			}
			assert(rhs.dest == r - rhs.len);
		}

		assert(lhs.len + rhs.len + 1 == r - l + 1);

		assert(lhs.dest == rhs.dest);
		int mid = lhs.dest;
		return min(lhs.insideCost + H[mid] * (1 + rhs.len), rhs.insideCost + H[mid] * (1 + lhs.len));
	};

	int Q = int(L.size());
	vector<ll> ans(Q);
	for (int i = 0; i < Q; i++) {
		ans[i] = query(L[i], R[i]);
	}
	return ans;
}
std::vector<long long> minimum_costs_slow(std::vector<long long> H, std::vector<int> L, std::vector<int> R) {
	int Q = int(L.size());
	vector<ll> ans(Q);
	for (int i = 0; i < Q; i++) {
		ans[i] = 1e18;
		for (int a = L[i]; a <= R[i]; a++) {
			ll tot = H[a];
			{
				ll cost = H[a];
				for (int v = a-1; v >= L[i]; v--) {
					cost = max(cost, H[v]);
					tot += cost;
				}
			}
			{
				ll cost = H[a];
				for (int v = a+1; v <= R[i]; v++) {
					cost = max(cost, H[v]);
					tot += cost;
				}
			}
			ans[i] = min(ans[i], tot);
		}
	}
	return ans;
}

} // namespace ecnerwala

std::vector<long long> minimum_costs(std::vector<int> H, std::vector<int> L, std::vector<int> R) {
	return ecnerwala::minimum_costs_slow({H.begin(), H.end()}, L, R);
	return ecnerwala::minimum_costs({H.begin(), H.end()}, L, R);
}
