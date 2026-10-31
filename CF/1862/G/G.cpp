#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) {
			cin >> a;
		}

		std::set<std::pair<int, int>> vals;
		std::set<std::pair<int, int>> diffs;
		auto modify_its = [&](auto ait, auto bit, bool insert) -> void {
			std::pair<int, int> key = {bit->first - ait->first, ait->second};
			if (insert) {
				auto [_, did_insert] = diffs.insert(key);
				assert(did_insert);
			} else {
				int num_erased = int(diffs.erase(key));
				assert(num_erased == 1);
			}
		};
		auto insert_a = [&](int i) -> void {
			auto [it, did_insert] = vals.insert({A[i], i});
			assert(did_insert);
			if (it != vals.begin() && std::next(it) != vals.end()) {
				modify_its(std::prev(it), std::next(it), false);
			}
			if (it != vals.begin()) {
				modify_its(std::prev(it), it, true);
			}
			if (std::next(it) != vals.end()) {
				modify_its(it, std::next(it), true);
			}
		};
		auto erase_a = [&](int i) -> void {
			auto it = vals.find({A[i], i});
			assert(it != vals.end());
			if (it != vals.begin()) {
				modify_its(std::prev(it), it, false);
			}
			if (std::next(it) != vals.end()) {
				modify_its(it, std::next(it), false);
			}
			if (it != vals.begin() && std::next(it) != vals.end()) {
				modify_its(std::prev(it), std::next(it), true);
			}
			vals.erase(it);
		};
		auto get_ans = [&]() -> int {
			int res = std::prev(vals.end())->first;
			if (!diffs.empty()) {
				res += std::prev(diffs.end())->first;
			}
			return res;
		};
		for (int i = 0; i < N; i++) {
			insert_a(i);
		}
		int Q; cin >> Q;
		for (int q = 0; q < Q; q++) {
			int i, x; cin >> i >> x; i--;
			erase_a(i);
			A[i] = x;
			insert_a(i);
			cout << get_ans() << " \n"[q+1==Q];
		}
	}

	return 0;
}
