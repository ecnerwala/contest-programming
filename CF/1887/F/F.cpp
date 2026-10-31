#include <bits/stdc++.h>

std::optional<std::vector<int>> solve(std::vector<int> R) {
	std::vector<int> R_orig = R;
	using std::cerr;
	int N = int(R.size());
	R.push_back(N);
	for (int i = 0; i < N; i++) {
		if (R[i] > R[i+1]) {
			return std::nullopt;
		}
	}
	if (R[0] == N) return std::nullopt;
	std::vector<bool> appears(N+1);
	for (auto r : R) {
		appears[r] = true;
	}
	std::vector<int> targets; targets.reserve(N);
	for (int i = 0; i <= N; i++) {
		if (!appears[i]) {
			targets.push_back(i);
		}
	}
	std::vector<int> next_same(N, -1);
	std::vector<int> sources; sources.reserve(N);
	int min_balls = 0;
	int max_balls = R[0];
	int num_final = 0;
	auto lo = targets.begin();
	auto hi = targets.begin();
	for (int i = 1; i <= N; i++) {
		if (R[i] == R[i-1]) {
			while (lo != targets.end() && *lo <= i-1) ++lo;
			int first = int(lo - targets.begin());
			// we must match with targets [first, last]
			// first <= num_sources + b <= last
			min_balls = std::max(min_balls, first - int(sources.size()));

			while (hi != targets.end() && *hi <= R[i]) ++hi;
			if (R[i] == N) {
				num_final++;
			} else {
				int last = int(hi - targets.begin()) - 1;
				max_balls = std::min(max_balls, last - int(sources.size()));
			}
			sources.push_back(i-1);
		} else {
			next_same[i-1] = R[i];
		}
	}
	assert(int(sources.size()) == int(targets.size()));
	max_balls = std::min(max_balls, num_final);
	if (min_balls > max_balls) return std::nullopt;

	sources.insert(sources.begin(), min_balls, -1);
	targets.insert(targets.end(), min_balls, N);
	for (int z = 0; z < int(sources.size()); z++) {
		int s = sources[z], t = targets[z];
		if (s != -1) {
			next_same[s] = t;
		}
	}
	std::vector<int> A(N);
	std::iota(A.begin(), A.end(), 1);
	for (int i = 0; i < N; i++) {
		if (next_same[i] < N) A[next_same[i]] = A[i];
	}
	return A;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> R(N);
		for (auto& r : R) { cin >> r; r--; }
		auto ans = solve(R);
		if (ans) {
			cout << "Yes" << '\n';
			for (int i = 0; i < N; i++) {
				cout << (*ans)[i] << " \n"[i+1==N];
			}
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
