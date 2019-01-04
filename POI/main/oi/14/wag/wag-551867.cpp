#include<iostream>
using namespace std;

const int MOD = 1e9;

int valPos = 0;
int numPos = 1;
int valNeg = 1;
int numNeg = 0;

void insert(int v) {
	//int nvpos = v + min(valPos, valNeg + 1);
	//int nvneg = 3 - v + min(valNeg, valPos + 1);
	if(valPos == valNeg) {
		//do nothing
	} else if(valPos > valNeg) {
		if(valPos == valNeg + 1) {
			numPos += numNeg;
			if(numPos >= MOD) numPos -= MOD;
		} else {
			numPos = numNeg;
		}
		valPos = valNeg + 1;
	} else {
		if(valNeg == valPos + 1) {
			numNeg += numPos;
			if(numNeg >= MOD) numNeg -= MOD;
		} else {
			numNeg = numPos;
		}
		valNeg = valPos + 1;
	}
	valPos += v;
	valNeg += 3 - v;
}

char c[2000];

bool isZero() {
	for(int i = 0; c[i]; i++) {
		if(c[i] != '0') return false;
	}
	return true;
}

int mod4() {
	int carry = 0;
	for(int i = 0; c[i]; i++) {
		carry *= 10;
		carry += c[i] - '0';
		c[i] = carry / 4 + '0';
		carry %= 4;
	}
	cerr << carry << '\n';
	return carry;
}

int main() {
	cin >> c;
	while(!isZero()) {
		insert(mod4());
	}
	insert(0);
	cout << numPos << '\n';
}
