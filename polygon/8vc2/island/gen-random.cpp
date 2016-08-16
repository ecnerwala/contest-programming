#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	int n = atoi(argv[1]);
    int t = atoi(argv[2]);

    vector<int> p(n);
	for(int i = 0; i < n; i++) {
		if(i > 0) p[i] = rnd.wnext(i, t);
	}

    printf("%d\n", n);
    vector<int> A(n);
	for(int i = 0; i < n; i++) A[i] = i;
	shuffle(A.begin(), A.end());
	for(int i = 0; i < n; i++) {
		if(i) printf(" ");
		printf("%d", A[i]);
	}
	printf("\n");

	shuffle(A.begin(), A.end());
	for(int i = 0; i < n; i++) {
		if(i) printf(" ");
		printf("%d", A[i]);
	}
	printf("\n");

    vector<int> perm(n);
	for(int i = 0; i < n; i++) perm[i] = i;
    shuffle(perm.begin() + 1, perm.end());
    vector<pair<int,int> > edges;

    for (int i = 1; i < n; i++) {
        if (rnd.next(2)) {
            edges.push_back(make_pair(perm[i], perm[p[i]]));
		} else {
            edges.push_back(make_pair(perm[p[i]], perm[i]));
		}
	}

    shuffle(edges.begin(), edges.end());

    for (int i = 0; i + 1 < n; i++)
        printf("%d %d\n", edges[i].first + 1, edges[i].second + 1);

    return 0;
}
