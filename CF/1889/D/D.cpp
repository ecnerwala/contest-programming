#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<int>> stks(N);
	for (auto& r : stks) {
		int k; cin >> k;
		r.resize(k);
		for (auto& v : r) { cin >> v; v--; }
	}

	// mem is -2 if in stack
	std::vector<int> mem(N, -1);
	std::vector<int> cur_stack; cur_stack.reserve(N);
	for (int st = 0; st < N; st++) {
		int cur = st;
		while (true) {
			if (mem[cur] == -2) {
				while (true) {
					int v = cur_stack.back(); cur_stack.pop_back();
					mem[v] = -1;
					assert(!stks[v].empty());
					stks[v].pop_back();
					if (v == cur) break;
				}
				assert(mem[cur] == -1);
			}
			if (stks[cur].empty()) {
				assert(mem[cur] != -2);
				mem[cur] = cur;
			}
			if (mem[cur] != -1) {
				while (!cur_stack.empty()) {
					int v = cur_stack.back(); cur_stack.pop_back();
					mem[v] = mem[cur];
				}
				break;
			}
			mem[cur] = -2;
			cur_stack.push_back(cur);
			cur = stks[cur].back();
		}
	}
	for (int i = 0; i < N; i++) {
		cout << mem[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
