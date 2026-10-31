#include <bits/stdc++.h>

std::vector<int> compute_R(std::vector<int> A) {
	int N = int(A.size());
	int num_distinct = int(std::set<int>(A.begin(), A.end()).size());
	std::map<int, int> cur;
	std::vector<int> R(N);
	int j = 0;
	for (int i = 0; i < N; i++) {
		while (int(cur.size()) < num_distinct && j < N) {
			cur[A[j]]++;
			j++;
		}
		if (int(cur.size()) < num_distinct) {
			assert(j >= N);
			j = N+1;
		}
		R[i] = j-1;

		if ((--cur[A[i]]) == 0) cur.erase(A[i]);
	}
	return R;
}

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
	for (int i = 1; i <= N; i++) {
		if (R[i] == R[i-1]) {
			int first = int(std::upper_bound(targets.begin(), targets.end(), i-1) - targets.begin());
			// we must match with targets [first, last]
			// first <= num_sources + b <= last
			min_balls = std::max(min_balls, first - int(sources.size()));

			if (R[i] == N) {
				num_final++;
			} else {
				int last = int(std::lower_bound(targets.begin(), targets.end(), R[i]) - targets.begin()) - 1;
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
	std::iota(A.begin(), A.end(), 0);
	for (int i = 0; i < N; i++) {
		if (next_same[i] < N) A[next_same[i]] = A[i];
	}
	if (compute_R(A) != R_orig) return std::nullopt;
	return A;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 mt(48);
	for (int z = 0; z < 0; z++) {
		cerr << "test " << z << '\n';
		int N = std::uniform_int_distribution<int>(1, 10)(mt);
		int M = std::uniform_int_distribution<int>(1, 10)(mt);
		std::vector<int> A(N);
		for (auto& a : A) a = std::uniform_int_distribution<int>(1, M)(mt);
		std::vector<int> R = compute_R(A);
		auto ans = solve(R);
		if (!ans) {
			cerr << "failed, could not find" << '\n';
			for (int a : A) { cerr << a << ' '; }
			for (int r : R) { cerr << r << ' '; }
			assert(false);
		} else {
			auto ans_R = compute_R(*ans);
			if (ans_R != R) {
				cerr << "failed, mismatch not find" << '\n';
				for (int a : A) { cerr << a << ' '; }
				for (int r : R) { cerr << r << ' '; }
				for (int a : *ans) { cerr << a << ' '; }
				for (int r : ans_R) { cerr << r << ' '; }
				assert(false);
			}
		}
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> R(N);
		for (auto& r : R) { cin >> r; r--; }
		auto ans = solve(R);
		if (ans) {
			cout << "Yes" << '\n';
			for (int i = 0; i < N; i++) {
				cout << (*ans)[i]+1 << " \n"[i+1==N];
			}
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
