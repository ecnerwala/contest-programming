#include <bits/stdc++.h>

using std::cerr;

std::vector<int> op_slow(std::vector<int> A) {
	int N = int(A.size());
	for (int l = 1; l <= N; l++) {
		std::vector<int> best(A.end() - l, A.end());
		for (int j = 0; j+l < N; j++) {
			std::vector<int> cnd(A.begin()+j, A.begin()+j+l);
			if (cnd < best) best = cnd;
		}
		std::copy(best.begin(), best.end(), A.end() - l);
	}
	return A;
}

std::vector<int> op(const std::vector<int>& A) {
	int N = int(A.size());
	std::vector<int> B; B.reserve(N);
	std::vector<int> border(N);
	std::vector<std::pair<int, int>> lyndon_tail(N); // length and count of final run of identical lyndon words
	for (int st = 0, en = 0, k = -1; en < N; en++) {
		while (k != -1 && A[en] < A[st+k]) {
			int extra = (en - st) % (en - st - k);
			if (extra == 0) {
				k = -1;
			} else {
				k = border[extra - 1];
			}
			st = en - extra;
		}
		if (k == -1 || A[en] == A[st+k]) {
			k++;
		} else if (A[en] > A[st+k]) {
			k = 0;
		} else assert(false);
		cerr << st << ".." << en+1 << ' ' << k << '\n';
		border[en-st] = k;

		{
			int extra = (en+1 - st) % (en+1 - st - k);
			if (extra == 0) {
				lyndon_tail[en] = {en+1-st-k, (en+1-st) / (en+1-st-k)};
			} else {
				lyndon_tail[en] = lyndon_tail[st + extra - 1];
			}
		}
	}

	for (int en = N-1; en >= 0; en--) {
		if (en+1==N || lyndon_tail[en+1] == std::pair<int, int>{1, 1}) {
			lyndon_tail[en].second = 1;
		}
	}
	for (int en = 0; en < N; en++) {
		auto [len, cnt] = lyndon_tail[en];
		assert(len >= 1);
		assert(cnt >= 1);
		for (int z = 0; z < cnt; z++) {
			for (int i = en - len + 1; i <= en; i++) {
				B.push_back(A[i]);
				if (int(B.size()) == N) goto done;
			}
		}
	}

	assert(false);
done:
	return B;
}

void test() {
	int N = 10;
	for (int msk = 0; msk < 1 << N; msk++) {
		std::vector<int> A(N);
		for (int i = 0; i < N; i++) {
			A[i] = (msk >> i) & 1;
		}
		cerr << N << '\n';
		for (int i = 0; i < N; i++) {
			cerr << A[i] << " \n"[i+1==N];
		}
		auto slo = op_slow(A);
		auto mine = op(A);
		if (slo != mine) {
			for (int i = 0; i < N; i++) {
				cerr << slo[i] << " \n"[i+1==N];
			}
			for (int i = 0; i < N; i++) {
				cerr << mine[i] << " \n"[i+1==N];
			}
			assert(false);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();
	exit(0);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::vector<int>> As; As.reserve(30);
	As.emplace_back(A);
	do {
		As.emplace_back(op(As.back()));
	} while (As.end()[-2] != As.end()[-1]);

	int Q; cin >> Q;
	while (Q--) {
		int i, j; cin >> i >> j; j--;
		i = std::min(i, int(As.size())-1);
		cout << As[i][j] << '\n';
	}

	return 0;
}
