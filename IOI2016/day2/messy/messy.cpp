#include <vector>

#include "messy.h"
#include<bits/stdc++.h>
using namespace std;

vector<int> restore_permutation(int N, int W, int R) {
	for(int m = 1; m < N; m *= 2) {
		for(int i = 0; i < N; i++) {
			if(!(i & m)) continue;
			string s(N, '0');
			for(int j = 0; j < N; j++) {
				if((i & (m-1)) != (j & (m-1))) {
					s[j] = '1';
				} else if(j == i) {
					s[j] = '1';
				}
			}
			add_element(s);
		}
	}
    compile_set();
	vector<int> P(N);
	for(int i = 0; i < N; i++) P[i] = 0;
	for(int m = 1; m < N; m *= 2) {
		vector<int> np(N);
		for(int i = 0; i < N; i++) {
			string s(N, '0');
			for(int j = 0; j < N; j++) {
				if((P[i] & (m-1)) != (P[j] & (m-1))) {
					s[j] = '1';
				} else if(j == i) {
					s[j] = '1';
				}
			}
			if(check_element(s)) {
				P[i] |= m;
			}
		}
	}
	return P;
}
