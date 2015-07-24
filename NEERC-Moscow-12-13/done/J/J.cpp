#include<bits/stdc++.h>
using namespace std;

void rec(vector<int> &a) {
	int s = int(a.size());
	if(s == 1) return;
	vector<int> l(a.begin(), a.begin() + s / 2);
	vector<int> r(a.begin() + s / 2, a.end());
	rec(l);
	rec(r);

	for(int i = 0, j = 0; i + j < int(a.size()); ) {
		if(i == int(l.size())) {
			a[i + j] = r[j];
			j++;
		} else if(j == int(r.size())) {
			a[i + j] = l[i];
			i++;
		} else {
			printf("1 %d %d\n", l[i], r[j]);
			fflush(stdout);
			char c[4];
			scanf(" %s", c);
			if(c[0] == 'Y') {
				a[i + j] = l[i];
				i++;
			} else {
				a[i + j] = r[j];
				j++;
			}
		}
	}
}

int main() {
	int N; scanf("%d", &N);
	vector<int> v;
	for(int i = 1; i <= N; i++) {
		v.push_back(i);
	}
	rec(v);
	printf("0");
	for(int i : v) {
		printf(" %d", i);
	}
	printf("\n");
	fflush(stdout);
	return 0;
}
