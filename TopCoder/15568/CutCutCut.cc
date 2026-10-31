#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class CutCutCut {

public:

	vector <int> findPieces(vector <int> X1, vector <int> Y1, vector <int> X2, vector <int> Y2, int Q) {
		const int L = 10000;
		auto linearize = [&] (int x, int y) -> int {
			assert(0 <= x && x <= L);
			assert(0 <= y && y <= L);
			if (x == 0) { return y; }
			else if (y == L) { return L + x; }
			else if (x == L) { return L + L + (L - y); }
			else if (y == 0) { return L + L + L + (L - x); }
			else assert(false);
		};
		vector<pair<int, int>> bounds(Q);
		for (int i = 0; i < Q; i++) {
			int v1 = linearize(X1[i], Y1[i]);
			int v2 = linearize(X2[i], Y2[i]);
			if (v1 > v2) swap(v1, v2);
			assert(v1 <= v2);
			bounds[i] = {v1, v2};
		}
		auto intersects = [&](pair<int, int> a, pair<int, int> b) {
			if (a.second < b.first || b.second < a.first) return false;
			if (a.first < b.first && b.second < a.second) return false;
			if (b.first < a.first && a.second < b.second) return false;
			return true;
		};
		int curAns = 1;
		vector<int> ans;
		for (int i = 0; i < Q; i++) {
			curAns++;
			for (int j = 0; j < i; j++) {
				assert(intersects(bounds[i], bounds[j]) == intersects(bounds[j], bounds[i]));
				curAns += intersects(bounds[i], bounds[j]);
			}
			ans.push_back(curAns);
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	mt19937 mt(48);
	const int L = int(10000);
	auto genPt = [&] () -> pair<int, pair<int, int>> {
		int v = mt() % (L-1) + 1;
		assert(0 < v && v < L);
		int x = mt() % 4;
		switch(x) {
			case 0:
				return {x, {0, v}};
			case 1:
				return {x, {v, 0}};
			case 2:
				return {x, {L, v}};
			case 3:
				return {x, {v, L}};
			default:assert(false);
		}
	};
	set<pair<int, int>> seen;
	auto genSeg = [&] () -> pair<pair<int, int>, pair<int, int>> {
		while (true) {
			auto a = genPt();
			auto b = genPt();
			if (a.first == b.first) continue;
			if (seen.count(a.second) || seen.count(b.second)) continue;
			seen.insert(a.second);
			seen.insert(b.second);
			return {a.second, b.second};
		}
	};
	const int Q = 500;
	vector<int> X1(Q), Y1(Q), X2(Q), Y2(Q);
	for (int i = 0; i < Q; i++) {
		auto it = genSeg();
		X1[i] = it.first.first;
		Y1[i] = it.first.second;
		X2[i] = it.second.first;
		Y2[i] = it.second.second;
	}

	auto res = CutCutCut().findPieces(X1, Y1, X2, Y2, Q);

	ofstream fout("bigTest.in");
	fout << "{"; for (int i = 0; i < Q; i++) { if (i) {fout << ",";} fout << X1[i]; } fout << "}" << '\n';
	fout << "{"; for (int i = 0; i < Q; i++) { if (i) {fout << ",";} fout << Y1[i]; } fout << "}" << '\n';
	fout << "{"; for (int i = 0; i < Q; i++) { if (i) {fout << ",";} fout << X2[i]; } fout << "}" << '\n';
	fout << "{"; for (int i = 0; i < Q; i++) { if (i) {fout << ",";} fout << Y2[i]; } fout << "}" << '\n';
	fout.close();
}

}  // namespace ecnerwala

using ecnerwala::CutCutCut;

// vim:ft=cpp
