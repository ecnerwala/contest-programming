#include <bits/stdc++.h>

struct test_case {
	std::vector<int> A;
	int D;
	friend std::ostream& operator << (std::ostream& o, const test_case& a) {
		o << a.A.size() << '\n';
		for (int v : a.A) {
			o << "A" << ' ' << std::fixed << std::setprecision(2) << v*0.01 << '\n';
		}
		o << a.D << '\n';
		return o;
	}
};

int main() {
	using std::cin;
	using std::cout;
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);

	auto make_case = [&](std::vector<double> diffs) -> test_case {
		double tot_diff = std::accumulate(diffs.begin(), diffs.end(), double(0));
		int D = int(72 * 60 / tot_diff + 0.5);
		int N = int(diffs.size());
		std::vector<int> res; res.reserve(N);
		double cur = 0;
		for (double d : diffs) {
			res.push_back(int(cur / tot_diff * 36000));
			cur += d;
		}
		return {res, D};
	};
	int case_num = 0;
	auto write_case = [&](std::string tag, const test_case& t) {
		std::ofstream fout("case-" + std::to_string(case_num++) + "-" + tag + ".in");
		fout << t;
	};
	for (int a : {49, 50}) {
		for (int b : {49, 50}) {
			std::vector<double> diffs;
			diffs.push_back(0.8);
			diffs.push_back(0.3);
			diffs.push_back(1.1);
			diffs.push_back(0.3);
			diffs.push_back(0.8);
			for (int i = 0; i < a; i++) diffs.push_back(0.6);
			diffs.push_back(0.9);
			diffs.push_back(0.9);
			for (int i = 0; i < b; i++) diffs.push_back(0.6);
			test_case t = make_case(diffs);
			write_case("single-chain-mid-gap", t);
		}
	}
	for (int a : {30, 31}) {
		for (int b : {30, 31}) {
			std::vector<double> diffs;
			diffs.push_back(1.1);
			for (int z = 0; z < a-1; z++) {
				diffs.push_back(0.8);
			}
			for (int z = 0; z < 30+1; z++) {
				diffs.push_back(1.1);
			}
			for (int z = 0; z < b-1; z++) {
				diffs.push_back(0.8);
			}
			test_case t = make_case(diffs);
			write_case("double-chain-stutter", t);
		}
	}
}
