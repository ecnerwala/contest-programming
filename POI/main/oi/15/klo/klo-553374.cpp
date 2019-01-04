#include<iostream>
#include<set>

using namespace std;

typedef long long ll;

int N, K;
ll H[200000];
int S, T; // size of lower and higher
multiset<ll> lower;
ll median;
multiset<ll> higher;
ll lsum, hsum;

void settle() {
	while(lower.size() > S) {
		higher.insert(median);
		hsum += median;
		median = *(--lower.end());
		lsum -= median;
		lower.erase(--lower.end());
	}
	while(higher.size() > T) {
		lower.insert(median);
		lsum += median;
		median = *higher.begin();
		hsum -= median;
		higher.erase(higher.begin());
	}
}

inline ll getVal() {
	return median * S - lsum + hsum - median * T;
}

ll go() {
	if(K == 1) {
		return 0;
	}
	S = (K - 1) / 2;
	T = K - 1 - S;
	for(int i = 0; i < K; i++) {
		higher.insert(H[i]);
		hsum += H[i];
	}
	median = *higher.begin();
	hsum -= median;
	higher.erase(higher.begin());
	settle();

	ll minVal = getVal();
	int bestStart = 0;
	int bestMed = median;
	//cerr << getVal() << ' ' << median << '\n';

	for(int i = 1; i + K <= N; i++) {
		//remove H[i - 1]
		ll v = H[i - 1];
		if(v > median) {
			higher.erase(higher.find(v));
			hsum -= v;
		} else if(v == median) {
			median = *higher.begin();
			hsum -= median;
			higher.erase(higher.begin());
		} else {
			lower.erase(lower.find(v));
			lsum -= v;
		}
		//insert H[i + K - 1]
		v = H[i + K - 1];
		if(v >= median) {
			higher.insert(v);
			hsum += v;
		} else {
			lower.insert(v);
			lsum += v;
		}
		
		settle();
	//cerr << getVal() << ' ' << lsum << ' ' << hsum << ' '  << median << '\n';
		v = getVal();
		if(v < minVal) {
			minVal = v;
			bestStart = i;
			bestMed = median;
		}
	}

	for(int i = 0; i < K; i++) {
		H[bestStart + i] = bestMed;
	}
	return minVal;
}

int main() {
	cin >> N >> K;
	for(int i = 0; i < N; i++) cin >> H[i];

	cout << go() << '\n';
	for(int i = 0; i < N; i++) cout << H[i] << '\n';
}
