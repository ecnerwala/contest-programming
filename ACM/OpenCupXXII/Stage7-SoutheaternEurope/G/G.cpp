#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; }
	std::vector<int> B(M);
	for (auto& b : B) { cin >> b; }
	std::sort(B.begin(), B.end());

	std::map<int, int> mp;
	auto query_val = [&](int i) -> int {
		auto it = mp.upper_bound(i);
		if (it == mp.begin()) {
			return -1;
		}
		return std::prev(it)->second;
	};

	auto insert_val = [&](int i, int v) -> void {
		assert(v >= 0);
		auto it = mp.upper_bound(i);
		if (it != mp.begin() && std::prev(it)->second >= v) {
			return;
		}
		it = mp.lower_bound(i);
		while (it != mp.end() && it->second <= v) {
			it = mp.erase(it);
		}
		mp[i] = v;
	};

	insert_val(0 - (-1) - 1, 0);
	std::optional<std::pair<int, int>> ins_queue;
	for (int i = 0; i < N; i++) {
		int c = int(std::lower_bound(B.begin(), B.end(), A[i]) - B.begin());
		if (i > 0 && A[i] > A[i-1]) {
			if (ins_queue) {
				insert_val(ins_queue->first, ins_queue->second);
				ins_queue = std::nullopt;
			}
		}
		int v = query_val(c - i);
		if (ins_queue) {
			insert_val(ins_queue->first, ins_queue->second);
			ins_queue = std::nullopt;
		}
		if (v != -1) {
			ins_queue = {c-i-1, v+1};
		}
	}

	if (ins_queue) {
		insert_val(ins_queue->first, ins_queue->second);
		ins_queue = std::nullopt;
	}

	int ans = query_val(M - N);
	if (ans == -1) {
		cout << -1 << '\n';
	} else {
		cout << N - ans << '\n';
	}

	return 0;
}
