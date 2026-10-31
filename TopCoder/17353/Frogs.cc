#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Frogs {

public:
	std::pair<std::vector<int>, std::vector<std::pair<int, int>>> move_to_front(vector<int> start) {
		std::vector<int> res;
		int N = int(start.size());
		assert(N >= 3);
		std::vector<std::pair<int, int>> people(N);
		for (int i = 0; i < N; i++) people[i] = {start[i], i};
		std::sort(people.begin(), people.end());
		for (int i = 0; i < N; i++) {
			if (people[i].first == i) continue;
			assert(people[i].first > i);
			if (i == N-1) {
				if (people[i].first == 2*N) {
					res.push_back(i-2);
					res.push_back(2*N-1);
					people[i-2].first = 2*N-1;
					res.push_back(2*N);
					res.push_back(2*N-2);
					res.push_back(2*N-2);
					res.push_back(i-2);
					people[i].first = i-2;
					std::swap(people[i], people[i-2]);
				}
				res.push_back(i-1);
				res.push_back(people[i].first+1);
				res.push_back(people[i].first+1);
				res.push_back(i+1);
				res.push_back(people[i].first);
				res.push_back(i);
				res.push_back(i+1);
				res.push_back(i-1);
				people[i].first = i;
			} else {
				res.push_back(people[i+1].first);
				res.push_back(i);
				people[i+1].first = i;
				std::swap(people[i], people[i+1]);
			}
		}
		return {res, people};
	}

	std::pair<std::vector<int>, std::vector<std::pair<int, int>>> move_to_mid(vector<int> start, int offset) {
		int N = int(start.size());
		auto it = move_to_front(start);
		auto res = it.first;
		auto people = it.second;
		std::vector<int> mid(N);
		for (int i = 0; i < N; i++) mid[i] = 2*i+offset;
		auto jt = move_to_front(mid);
		auto mid_ops = jt.first;
		auto mid_people = jt.second;
		res.insert(res.end(), mid_ops.rbegin(), mid_ops.rend());
		for (int i = 0; i < N; i++) {
			assert(people[i].first == i && mid_people[i].first == i);
			people[i].first = 2*mid_people[i].second + offset;
		}
		std::sort(people.begin(), people.end());
		return {res, people};
	}

	vector <int> move_one(vector <int> start, vector <int> goal) {
		auto it = move_to_mid(start, 1);
		auto jt = move_to_mid(goal, 0);
		auto res = it.first;
		auto people = it.second;
		auto res2 = jt.first;
		auto people2 = jt.second;

		int N = int(start.size());
		for (int i = 0; i < N; i++) {
			assert(people2[i].first == 2*i);
			for (int j = i; j < N; j++) {
				if (people[j].second == people2[i].second) {
					if (j == i) {
						if (j+1 < N) {
							res.push_back(people[j].first);
							res.push_back(people[j+1].first+1);
							res.push_back(people[j+1].first+1);
							res.push_back(people2[i].first);
							people[j].first = people2[i].first;
						} else {
							res.push_back(people[j].first);
							res.push_back(people[j-1].first-1);
							res.push_back(people[j-1].first-1);
							res.push_back(people2[i].first);
							people[j].first = people2[i].first;
						}
					} else {
						for (int k = j; k > i; k--) {
							int dest = (k-1 == i) ? people2[i].first : people[k-1].first-1;
							res.push_back(people[k].first);
							res.push_back(dest);
							people[k].first = dest;
							std::swap(people[k], people[k-1]);
						}
						// now move people i+1 to 2*j+1
						if (j-i > 1) {
							for (int k = i+1; k < j; k++) {
								int dest = (k+1==j) ? 2*j+1 : people[k+1].first+1;
								res.push_back(people[k].first);
								res.push_back(dest);
								people[k].first = dest;
								std::swap(people[k], people[k+1]);
							}
						} else {
							assert(j == i+1);
							if (i > 0) {
								res.push_back(people[i+1].first);
								res.push_back(people[i].first-1);
								res.push_back(people[i].first-1);
								res.push_back(2*i+3);
								people[i+1].first = 2*i+3;
							} else {
								res.push_back(people[i+1].first);
								res.push_back(people[i+2].first+1);
								res.push_back(people[i+2].first+1);
								res.push_back(2*i+3);
								people[i+1].first = 2*i+3;
							}
						}
					}
					break;
				}
			}
		}

		res.insert(res.end(), res2.rbegin(), res2.rend());
		return res;
	}

	vector <int> move(vector <int> start, vector <int> goal) {
		int N = int(start.size());
		vector<int> res = move_one(start, goal);
		for (auto& v : start) v = 2*N-v;
		for (auto& v : goal) v = 2*N-v;
		vector<int> res2 = move_one(start, goal);
		for (auto& v : res2) v = 2*N-v;
		if (res2.size() < res.size()) return res2;
		else return res;
	}
};

void my_tests() {
	// FILL IN TESTS
	std::vector<int> start(30);
	std::iota(start.begin(), start.end(), 31);
	std::vector<int> end = start;
	std::reverse(end.begin(), end.end());

	auto res = Frogs().move(start, end);
	cerr << res.size()/2 << '\n';
}

}  // namespace ecnerwala

using ecnerwala::Frogs;

// vim:ft=cpp
