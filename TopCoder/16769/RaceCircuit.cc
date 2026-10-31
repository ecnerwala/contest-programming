#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class RaceCircuit {

public:

	int construct(int R, int C, vector <int> costRight, vector <int> costDown, vector <int> costCorner) {
		/*
		for (int j = 0; j < C; j++) {
			for (int i1 = 0, i2 = R-1; i1 < i2; i1++, i2--) {
				swap(costRight[i1*C+j], costRight[i2*C+j]);
				swap(costCorner[i1*C+j], costCorner[i2*C+j]);
			}
			for (int i1 = 0, i2 = R-2; i1 < i2; i1++, i2--) {
				swap(costDown[i1*C+j], costDown[i2*C+j]);
			}
		}
		for (int i = 0; i < R; i++) {
			for (int j1 = 0, j2 = C-1; j1 < j2; j1++, j2--) {
				swap(costDown[i*C+j1], costDown[i*C+j2]);
				swap(costCorner[i*C+j1], costCorner[i*C+j2]);
			}
			for (int j1 = 0, j2 = C-2; j1 < j2; j1++, j2--) {
				swap(costRight[i*C+j1], costRight[i*C+j2]);
			}
		}
		*/
		using state = array<int, 13>;
		// we use C+1 entries: the first C are the connectivity, and the last one is true if the last cell is down
		state start_state; start_state.fill(0);
		for (int j = 0; j < C; j++) start_state[j] = -1;
		start_state[C] = false;

		map<state, int> dp;
		dp[start_state] = 0;
		int ans = -1;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				//cerr << "starting " << i << ' ' << j << '\n';
				map<state, int> ndp;
				auto check = [&](const state& s, int cnd) {
					auto it = ndp.find(s);
					if (it == ndp.end()) {
						ndp.insert({s, cnd});
					} else {
						it->second = min(it->second, cnd);
					}
				};
				for (const auto& it : dp) {
					const auto& s = it.first;
					int v = it.second;
					if (s[j] != -1) {
						if (i > 0 && costDown[(i-1) * C + j] != -1) {
							// we must connect there
							if (!(j > 0 && s[j-1] == -2)) {
								state ns = s;
								int nv = v + costDown[(i-1)*C+j];
								if (j > 0 && s[C]) nv += costCorner[i*C+(j-1)];
								ns[C] = false;
								check(ns, nv);
							}
							if (j > 0 && costRight[i*C+(j-1)] != -1) {
								if (s[j-1] == -2) {
									assert(s[C] == false);
									state ns = s;
									int nv = v + costDown[(i-1)*C+j] + costRight[i*C+(j-1)] + costCorner[i*C+j] + costCorner[i*C+(j-1)];
									ns[s[j]] = j-1;
									ns[j-1] = s[j];
									ns[j] = -1;
									check(ns, nv);
								} else if (s[j-1] >= 0) {
									state ns = s;
									ns[C] = false;
									int nv = v + costDown[(i-1)*C+j] + costRight[i*C+(j-1)] + costCorner[i*C+j];
									if (!s[C]) nv += costCorner[i*C+(j-1)];
									if (s[j] == j-1) {
										if (i == R-1 && j == C-1) {
											bool is_good = true;
											for (int k = 0; k < j-1; k++) if (ns[k] != -1) is_good = false;
											if (is_good) {
												if (ans == -1 || nv < ans) ans = nv;
											}
										}
									} else {
										int a = s[j-1];
										int b = s[j];
										ns[a] = b;
										ns[b] = a;
										ns[j-1] = ns[j] = -1;
										check(ns, nv);
									}
								} else if (s[j-1] == -1) {
									// invalid
								} else assert(false);
							}
						}
					} else {
						assert(s[j] == -1);
						if (!(j > 0 && s[j-1] == -2) && j != C-1) {
							state ns = s;
							int nv = v;
							if (j > 0 && s[C]) nv += costCorner[i*C+(j-1)];
							ns[C] = false;
							ns[j] = -2;
							check(ns, nv);
						}
						if (j > 0 && costRight[i*C+(j-1)] != -1) {
							// connect above only
							state ns = s;
							int nv = v + costRight[i*C+(j-1)];
							ns[C] = true;
							if (j == C-1) {
								ns[C] = false;
								nv += costCorner[i*C+j];
							}
							if (s[j-1] == -2) {
								assert(s[C] == false);
								nv += costCorner[i*C+(j-1)];
								ns[j-1] = j;
								ns[j] = j-1;
								check(ns, nv);
							} else if (s[j-1] >= 0) {
								if (!s[C]) nv += costCorner[i*C+(j-1)];
								ns[s[j-1]] = j;
								ns[j] = s[j-1];
								ns[j-1] = -1;
								check(ns, nv);
							} else if (s[j-1] == -1) {
								// invalid
							} else assert(false);
						}
					}
				}
				dp = std::move(ndp);
				/*
				for (const auto& it : dp) {
					cerr << "[";
					for (int k = 0; k < C; k++) {
						cerr << it.first[k] << ",";
					}
					cerr << " ;" << it.first[C] << "] : " << it.second << '\n';
					for (int k = 0; k < C; k++) {
						if (it.first[k] >= 0) assert(it.first[it.first[k]] == k);
						if (it.first[k] == -2) {
							assert(k == j && j != C-1);
						}
						if (j == 0 || j == C-1) assert(it.first[C] == false);
					}
				}
				*/
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << RaceCircuit().construct(12, 12, vector<int>(144, 999), vector<int>(144, 999), vector<int>(144, 999)) << '\n';
	mt19937 mt(49);
	int R = 12;
	int C = 11;
	vector<int> cRight1(R*C), cDown1(R*C), cCorner1(R*C);
	vector<int> cRight2(R*C), cDown2(R*C), cCorner2(R*C);
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			cRight1[i*C+j] = cDown2[j*R+i] = int(mt() % 1001) - 1;
			cDown1[i*C+j] = cRight2[j*R+i] = int(mt() % 1001) - 1;
			cCorner1[i*C+j] = cCorner2[j*R+i] = int(mt() % 1000);
		}
	}
	cerr << RaceCircuit().construct(R, C, cRight1, cDown1, cCorner1) << '\n';
	cerr << RaceCircuit().construct(C, R, cRight2, cDown2, cCorner2) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::RaceCircuit;

// vim:ft=cpp
