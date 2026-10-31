#include "stations.h"
#include <cstdio>
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>

static int max_label = 0;
static int r, n, k, q;
static std::vector<int> u, v, labels, answers;
static std::map<int, int> reverse_labels;
static std::vector<std::vector<int>> adjlist;
static int s, t, w;
static std::vector<int> c;

int main() {
	assert(scanf("%d", &r) == 1);
	for (int tc = 0; tc < r; tc++) {
		assert(scanf("%d%d", &n, &k) == 2);
		u.resize(n - 1);
		v.resize(n - 1);
		adjlist.clear();
		adjlist.resize(n);
		for (int i = 0; i < n - 1; i++) {
			assert(scanf("%d%d", &u[i], &v[i]) == 2);
			adjlist[u[i]].push_back(v[i]);
			adjlist[v[i]].push_back(u[i]);
		}
		labels = label(n, k, u, v);
		if ((int)labels.size() != n) {
			printf("Number of labels not equal to %d\n", n);
			exit(0);
		}
		reverse_labels.clear();
		for (int i = 0; i < n; i++) {
			if (labels[i] < 0 || labels[i] > k) {
				printf("Label not in range 0 to %d\n", k);
				exit(0);
			}
			if (reverse_labels.find(labels[i]) != reverse_labels.end()) {
				printf("Labels not unique\n");
				exit(0);
			}
			reverse_labels[labels[i]] = i;
			if (labels[i] > max_label) {
				max_label = labels[i];
			}
		}
		assert(scanf("%d", &q) == 1);
		for (int i = 0; i < q; i++) {
			assert(scanf("%d%d%d", &s, &t, &w) == 3);
			c.clear();
			for (int v : adjlist[s]) {
				c.push_back(labels[v]);
			}
			std::sort(c.begin(), c.end());
			int answer = find_next_station(labels[s], labels[t], c);
			if (!std::binary_search(c.begin(), c.end(), answer)) {
				printf("Label %d returned by find_next_station not found in c\n", answer);
				exit(0);
			}
			answers.push_back(reverse_labels[answer]);
		}
	}
	printf("%d\n", max_label);
	for (int index : answers) {
		printf("%d\n", index);
	}
	exit(0);
}
