#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> state(N+M+1);
	for (int i = 0; i < N; i++) state[i] = 1;
	state[N] = 0;
	for (int i = N+1; i <= N+M; i++) state[i] = 2;

	std::vector<int> ops; ops.reserve(N*M+N+M);

	int cur = N;
	assert(state[cur] == 0);
	auto do_op = [&](int a) -> void {
		assert(state[cur] == 0);
		state[cur] = state[a];
		state[a] = 0;
		cur = a;
		ops.push_back(a);
	};

	for (int z = 0; int(ops.size()) < (N*M+N+M); z++) {
		assert(z < 2*(N+M));
		if ((z & 1) == 0) {
			bool did = false;
			if (cur-1 >= 0 && state[cur-1] == 1) {
				did = true;
				do_op(cur-1);
			}
			while (cur-2 >= 0 && state[cur-1] == 2 && state[cur-2] == 1) {
				did = false;
				do_op(cur-2);
			}
			if (!did && cur-1 >= 0 && state[cur-1] == 1) {
				do_op(cur-1);
			}
		} else {
			bool did = false;
			if (cur+1 <= N+M && state[cur+1] == 2) {
				did = true;
				do_op(cur+1);
			}
			while (cur+2 <= N+M && state[cur+1] == 1 && state[cur+2] == 2) {
				did = false;
				do_op(cur+2);
			}
			if (!did && cur+1 <= N+M && state[cur+1] == 2) {
				do_op(cur+1);
			}
		}
	}

	cout << ops.size() << '\n';
	for (int x : ops) cout << x << '\n';

	return 0;
}
