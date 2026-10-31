#include <bits/stdc++.h>

std::vector<int> compute_B(int N, std::vector<int> A) {
	assert(N >= 1);
	assert(int(A.size()) == N+2);

	int M = int(std::set(A.begin()+1, A.end()-1).size());

	int num_good = 0;
	std::map<int, int> cnt;
	std::vector<int> my_B(N+2);
	my_B[0] = 0;
	my_B[N+1] = N+1;
	for (int i = 1, j = 0; i <= N; i++) {
		while (num_good < M && j <= N) {
			j++;
			if (j == N+1) continue;
			num_good += !(cnt[A[j]]++);
		}
		assert(j >= i);
		my_B[i] = j;
		num_good -= !(--cnt[A[i]]);
	}

	return my_B;
}

std::optional<std::vector<int>> solve(int N, std::vector<int> B) {
	assert(int(B.size()) == N+2);
	B[0] = 0;
	B[N+1] = N+1;

	std::vector<int> pref(N+2);
	for (int i = 0; i <= N; i++) {
		if (B[i+1] > B[i]) {
			pref[i+1] ++;
			pref[B[i+1]] --;
		}
	}
	for (int i = 0; i <= N; i++) {
		pref[i+1] += pref[i];
	}

	int M = 1;
	for (int i = 1; i <= N; i++) {
		M = std::max(M, pref[i] + 1);
	}

	bool is_good = true;
	for (int i = 1; i <= N; i++) {
		if (B[i] <= N && M > (B[i] - i + 1)) {
			is_good = false;
			break;
		}
	}
	if (!is_good) {
		return std::nullopt;
	}

	std::vector<int> A(N+2, -1);

	std::queue<int> q;
	A[B[1]] = 0;
	for (int v = 1; v < M; v++) q.push(v);
	for (int i = 1; i <= N; i++) {
		if (A[i] == -1) {
			A[i] = q.front(); q.pop();
		}
		if (B[i+1] > B[i]) {
			A[B[i+1]] = A[i];
		} else {
			q.push(A[i]);
		}
	}

	return A;
}

void test() {
	int seed = 0;
	while (seed < 100000) {
		std::cerr << seed << '\n';
		std::mt19937 mt(seed++);
		int N = seed % 30 + 1;
		int M = mt() % N + 1;

		std::cerr << N << ' ' << M << '\n';

		std::vector<int> orig_A(N+2);
		for (int i = 1; i <= N; i++) {
			orig_A[i] = mt() % M;
		}
		std::vector<int> B = compute_B(N, orig_A);

		auto A = solve(N, B);

		assert(A);
		std::vector<int> my_B = compute_B(N, *A);
		assert(my_B == B);
		for (int i = 1; i <= N; i++) {
			assert((*A)[i] >= 0);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> B(N+2);
		B[0] = 0;
		for (int i = 1; i <= N; i++) {
			cin >> B[i];
		}
		B[N+1] = N+1;

		auto A = solve(N, B);

		if (A) {
			std::vector<int> my_B = compute_B(N, *A);
			assert(my_B == B);
			if (my_B != B) A = std::nullopt;
		}

		if (A) {
			for (int i = 1; i <= N; i++) {
				assert((*A)[i] != -1);
				cout << (*A)[i] << " \n"[i==N];
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
