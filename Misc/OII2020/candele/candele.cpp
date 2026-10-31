#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

void brucia(int N, std::vector<int> &M, std::vector<int> &B, std::vector<long long> &T) {
	assert(int(M.size()) == N);
	assert(int(B.size()) == N);
	assert(int(T.size()) == N);
	fill(T.begin(), T.end(), -1);

	std::vector<std::pair<int, int>> los; los.reserve(N);
	std::vector<std::pair<int, int>> his; his.reserve(N);
	for (int i = 0; i < N; i++) {
		(M[i] <= M[0] ? los : his).push_back({M[i], i});
	}
	sort(los.begin(), los.end());
	sort(his.begin(), his.end(), std::greater<>());
	std::queue<std::pair<long long, int>> left_q;
	std::queue<std::pair<long long, int>> right_q;

	left_q.push({M[0], M[0]});
	right_q.push({M[0], M[0]});

	while (!left_q.empty() || !right_q.empty()) {
		if (!left_q.empty() && (los.empty() || los.back().first < left_q.front().second)) {
			left_q.pop();
			continue;
		}
		if (!right_q.empty() && (his.empty() || his.back().first > right_q.front().second)) {
			right_q.pop();
			continue;
		}
		int i;
		if (left_q.empty() || (!right_q.empty() && his.back().first - right_q.front().first < left_q.front().first - los.back().first)) {
			assert(!right_q.empty());
			assert(!his.empty());
			i = his.back().second;
			T[i] = his.back().first - right_q.front().first;
			his.pop_back();
			if (B[i] < M[i]) {
				left_q.push({M[i] + T[i], B[i]});
			} else {
				setmax(right_q.front().second, B[i]);
			}
		} else {
			assert(!left_q.empty());
			assert(!los.empty());
			i = los.back().second;
			T[i] = left_q.front().first - los.back().first;
			los.pop_back();
			if (B[i] > M[i]) {
				right_q.push({M[i] - T[i], B[i]});
			} else {
				setmin(left_q.front().second, B[i]);
			}
		}
	}
}
