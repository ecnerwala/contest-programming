#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;

	int64_t K; cin >> K;

	std::vector<int> loc; loc.reserve(S.size());
	for (int i = 0; i < int(S.size()); i++) {
		if (S[i] == 'Y') loc.push_back(i);
	}
	int N = int(loc.size());
	for (int i = 0; i < N; i++) {
		loc[i] -= i;
	}

	int ans = 0;

	set<std::pair<int, int>> los;
	set<std::pair<int, int>> his;
	std::optional<std::pair<int, int>> mid;
	int64_t cur_val = 0; // sum(his) - sum(los);

	auto rebalance = [&]() -> void {
		while (los.size() > his.size()) {
			if (mid) {
				cur_val += mid->first;
				his.insert(*mid);
				mid = std::nullopt;
			} else {
				auto it = std::prev(los.end());
				cur_val += it->first;
				mid = *it;
				los.erase(it);
			}
		}
		while (los.size() < his.size()) {
			if (mid) {
				cur_val -= mid->first;
				los.insert(*mid);
				mid = std::nullopt;
			} else {
				auto it = his.begin();
				cur_val -= it->first;
				mid = *it;
				his.erase(it);
			}
		}
	};
	auto remove = [&](std::pair<int, int> v) -> void {
		if (mid == v) {
			mid = std::nullopt;
		} else if (los.count(v)) {
			cur_val += v.first;
			los.erase(v);
		} else if (his.count(v)) {
			cur_val -= v.first;
			his.erase(v);
		} else assert(false);
		rebalance();
	};
	auto insert = [&](std::pair<int, int> v) -> void {
		assert(los.size() == his.size());
		if (mid) {
			if (v < *mid) {
				cur_val -= v.first;
				los.insert(v);
			} else {
				cur_val += v.first;
				his.insert(v);
			}
			rebalance();
		} else if (los.empty() && his.empty()) {
			mid = v;
		} else {
			if (v < *his.begin()) {
				cur_val -= v.first;
				los.insert(v);
			} else {
				cur_val += v.first;
				his.insert(v);
			}
			rebalance();
		}
	};

	for (int i = 0, j = 0; true; ) {
		if (cur_val <= K) {
			ans = std::max(ans, j-i);
			// increment j
			if (j == N) {
				break;
			}
			insert({loc[j], j});
			j++;
		} else {
			// increment i
			assert(i < j);
			remove({loc[i], i});
			i++;
		}
	}

	cout << ans << '\n';

	return 0;
}
