#include <bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cout << "Bad usage, use ./judge infile outfile" << '\n';
		exit(1);
	}
	ifstream infile(argv[1]);
	ifstream outfile(argv[2]);
	int N; infile >> N;
	std::vector<int> B_target(N);
	for (int i = 0; i < N; i++) {
		infile >> B_target[i];
	}

	std::vector<int> A(N);
	std::iota(A.begin(), A.end(), 1);

	int ops; outfile >> ops;
	for (int j = 0; j < ops; j++) {
		int type, v;
		bool is_good = bool(outfile >> type >> v);
		assert(is_good);
		if (type == 1) {
			for (auto& a : A) a += v;
		} else if (type == 2) {
			for (auto& a : A) a %= v;
		} else assert(false);
	}
	assert(A == B_target);

	return 0;
}
