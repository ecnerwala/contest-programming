#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	mt19937 mt(48);

	int N, M; cin >> N >> M;

	int last_val = -1;
	// returns the difference
	auto query = [&](int x, int d) -> int {
		assert(0 <= x && x < N);
		assert(d == -1 || d == +1);

		cout << '?' << ' ' << x << ' ' << d << '\n' << flush;

		int v; cin >> v;
		int diff = v - last_val;
		last_val = v;
		return diff;
	};
	query(0, +1); // initialize last_val

	int L = N * M;

	while (true) {
		if (query(0, +1) == +1) {
			// we've found a starting point
			break;
		}
	}

	// the predecessor of 0 is laser; we'll let this predecessor be index 0

	vector<int> pos(N);

	vector<int> alive(N-1); iota(alive.begin(), alive.end(), N-1);
	shuffle(alive.begin(), alive.end(), mt);

	pos[0] = 1;
	for (int x = 1; x < L-M; ) {
		assert(pos[0] == x);
		// position x-1 is guaranteed to be empty and position x+M-1 is guaranteed to be full

		pos[0]++;
		if (query(0, +1) == +1) {
			// take another step
			x++;
			continue;
		}

		assert(pos[0] == x+1);
		pos[0]--;
		query(0, -1);

		assert(pos[0] == x);

		// there's someone starting at x
		for (int i : alive) {
		}
	}

	for (int i : alive) {
		pos[i] = L-M;
	}

	cout << '!';
	for (int i = 1; i < N; i++) {
		cout << ' ' << pos[i];
	}
	cout << '\n' << flush;

	return 0;
}
