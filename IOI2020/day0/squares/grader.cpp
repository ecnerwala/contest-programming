#include "squares.h"
#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>

static int max_k = 0;
static int r, n, k, q, x;
static std::vector<int> labels, c, answers;

int main() {
	assert(scanf("%d", &r) == 1);
	for (int tc = 0; tc < r; tc++) {
		assert(scanf("%d%d", &n, &q) == 2);
		labels = paint(n);
		if ((int)labels.size() != n + 1) {
			printf("Number of labels not equal to %d\n", n + 1);
			exit(0);
		}
		for (int i = 0; i < n; i++) {
			if (labels[i] != 0 && labels[i] != 1) {
				printf("Label not 0 or 1\n");
				exit(0);
			}
		}
		k = labels[n];
		if (k < 0 || k > 1000) {
			printf("Label not in range 0 to 1000\n");
			exit(0);
		}
		if (k > max_k) {
			max_k = k;
		}
		for (int i = 0; i < q; i++) {
			assert(scanf("%d", &x) == 1);
			c.clear();
			
			for (int j = x; j < x + k; j++) {
				if (j >= n) {
					c.push_back(-1);
				} else {
					c.push_back(labels[j]);
				}
			}
			answers.push_back(find_location(n, c));
		}
	}
	printf("%d\n", max_k);
	for (int ans : answers) {
		printf("%d\n", ans);
	}
	exit(0);
}
