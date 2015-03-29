#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class SquareScores {

	static const int MAXN = 2000;
	double exp[30][MAXN];

	static const int D = 26;

	int N;

public:

	double calcexpectation(vector <int> p, string s) {
		while(p.size() < D) p.push_back(0);
		N = s.size();
		for(int i = 0; i < D; i++) {
			exp[i][0] = 1;
			exp[i][1] = p[i] * 0.01;
			for(int p = 2; p <= N; p++) {
				exp[i][p] = exp[i][p - 1] * exp[i][1];
			}
		}
		for(int p = 0; p <= N; p++) {
			exp[D][p] = 0;
			for(int i = 0; i < D; i++) {
				exp[D][p] += exp[i][p];
			}
		}

		double res = 0;

		for(int i = 0; i < N; i++) {
			int val = D;
			int cnt = 0;
			for(int j = i; j < N; j++) {
				if(s[j] != '?') {
					int v = s[j] - 'a';
					if(val == D) {
						val = v;
					} else if(val != v) {
						break;
					}
				} else {
					cnt++;
				}

				res += exp[val][cnt];
			}
		}

		return res;
	}
};

// vim:ft=cpp
