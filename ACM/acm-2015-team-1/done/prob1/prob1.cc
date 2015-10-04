#include<bits/stdc++.h>
using namespace std;
#define fori(i,s,e) for(int i = (s); i < ((int)e); i++)

int n;

int s[1008];
int t[1008];
const int OO = 1000<<20;
int solveb(int b) {
	int lai = -1;
	int minx = OO;
	int maxc = 0;
	fori (j, 0, n) {
		if (s[j] > b) {
			// every >b
			if (lai != -1) {
				if ((j - lai) & 1) {
					// bad
					if (j - lai == 1) {
						// super bad
						return OO;
					} else if (minx != -1) {
						maxc = max(maxc, minx);
					}
				}
			}
			// every >b goes here
			minx = OO;
			lai = j;
		} else {
			minx = min(minx, s[j]);
		}
	}
	return b + maxc;
}

int solve() {
	if (n == 1) return s[0];
	fori(i, 0, n){
		t[i] = s[i];
	}
	sort(t, t+n);
	int ac = OO;
	fori(i, 0, n) {
		ac = min(ac, solveb(t[i]));
	}
	return t[n-1] + ac;
}

int main() {
  ios_base::sync_with_stdio(0);
  scanf("%d", &n);
  for(int i = 0; i < n; i++) scanf("%d", &s[i]);
  printf("%d\n", solve());
  return 0;
}
