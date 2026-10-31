#include <bits/stdc++.h>

void pssert(bool b) {
	if (!b) {
		std::cout << "GARBAGE" << '\n' << std::flush;
		exit(0);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	int num_ops = 0;
	auto do_empty = [&](int a) -> void {
		num_ops++;
		//pssert(num_ops <= int(1e5));
		pssert(num_ops <= 2 * N + 2 * M + 1);
		cout << "E " << a+1 << '\n' << flush;
	};
	auto do_fill = [&](int a) -> void {
		num_ops++;
		//pssert(num_ops <= int(1e5));
		pssert(num_ops <= 2 * N + 2 * M + 1);
		cout << "F " << a+1 << '\n' << flush;
	};
	auto do_move = [&](int i, int j) -> int {
		num_ops++;
		//pssert(num_ops <= int(1e5));
		pssert(num_ops <= 2 * N + 2 * M + 1);
		cout << "D " << i+1 << ' ' << j+1 << '\n' << flush;
		int resp; cin >> resp;
		if (resp == -1) exit(0);
		return resp;
	};

	vector<int> ans(N);
	int best_known = -1;

	vector<pair<int, int>> to_dump; to_dump.reserve(5*N);
	for (int i = 1; i < N; i++) to_dump.push_back({i, 0});
	int idx = 0;

	do_empty(0);
	for (int i = 0; i < N; i++) {
		if (ans[i]) continue;
		int last_unknown = -1;
		int last_unknown_val = 0;
		ans[i] = 0;
		while (true) {
			if (idx == int(to_dump.size())) {
				if (best_known == -1) {
					assert(i == 0 && N <= 3);
					do_fill(1);
					to_dump.push_back({1, ans[1]});
				} else {
					do_fill(best_known);
					to_dump.push_back({best_known, ans[best_known]});
				}
			}
			auto [j, v] = to_dump[idx++];
			int x = do_move(j, i);
			if (!x) break;
			ans[i] += x;
			if (last_unknown != -1) {
				ans[last_unknown] = last_unknown_val;
				if (best_known == -1 || ans[last_unknown] > ans[best_known]) best_known = last_unknown;
			}
			if (v) {
				if (x < v) break;
				last_unknown = -1;
			} else {
				if (x == M) {
					ans[j] = x;
					if (best_known == -1 || ans[j] > ans[best_known]) best_known = j;
				} else {
					last_unknown = j;
					last_unknown_val = x;
				}
			}
			if (ans[i] == M) break;
		}
		if (best_known == -1 || ans[i] > ans[best_known]) best_known = i;
		to_dump.push_back({i, ans[i]});
	}

	cout << "A "; for (int i = 0; i < N; i++) { cout << ans[i] << " \n"[i+1==N]; } cout << flush;

	return 0;
}
