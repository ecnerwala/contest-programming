#include <bits/stdc++.h>
using namespace std;

struct variable {
    int ind;
    bool flip;
};

const variable F{-1, false};
const variable T{-1, true};

struct op {
    string opcode;
    vector<variable> args;
};

size_t input_length = 0;
vector<op> ops;
vector<variable> outputs;

variable operator ~ (const variable &a) {
    return variable{a.ind, !a.flip};
}

variable operator | (variable a, variable b) {
    if (b.ind == -1) {
        swap(a,b);
    }
    if (a.ind == -1) {
        if (a.flip) {
            return T;
        } else {
            return b;
        }
    }
    int ind = int(ops.size());
    ops.push_back(op{"or", {a, b}});
    return variable{ind, false};
}

variable operator & (variable a, variable b) {
    return ~((~a) | (~b));
}

variable operator ^ (variable a, variable b) {
    if (b.ind == -1) {
        swap(a,b);
    }
    if (a.ind == -1) {
        if (a.flip) {
            return ~b;
        } else {
            return b;
        }
    }
    int ind = int(ops.size());
    ops.push_back(op{"xor", {a, b}});
    return variable{ind, false};
}

variable input() {
    int ind = int(ops.size());
    assert(ind == int(input_length));
    input_length ++;
    ops.push_back(op{"input", {}});
    return variable{ind, false};
}

void output(variable a) {
    assert(a.ind != -1);
    outputs.push_back(a);
}

typedef array<variable, 128> num;

void output(const num &a) {
    for (size_t i = 0; i < 128; i++) {
        output(a[i]);
    }
}

num operator + (const num &l, const num &r) {
    num res;
    variable carry = F;
    for (size_t i = 0; i < 128; i++) {
        variable a = l[i], b = r[i];

        variable x = a ^ b;
        res[i] = x ^ carry;
        carry = (x & carry) | (a & b);
    }
    // Drop the final carry
    return res;
}

num mul(const variable A[64], const variable B[64]) {
    num res;
    for (size_t i = 0; i < 128; i++) {
        res[i] = F;
    }
    for (size_t i = 0; i < 64; i++) {
        num v;
        for (size_t j = 0; j < 128; j++) {
            v[j] = F;
        }
        for (size_t j = 0; j < 64; j++) {
            v[i+j] = A[i] & B[j];
        }
        res = res + v;
    }
    return res;
}

void go() {
    variable A[2][2][64];
    variable B[2][2][64];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int z = 0; z < 64; z++) {
                A[i][j][z] = input();
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int z = 0; z < 64; z++) {
                B[i][j][z] = input();
            }
        }
    }
    num C[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            C[i][j] = mul(A[i][0], B[0][j]) + mul(A[i][1], B[1][j]);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            output(C[i][j]);
        }
    }
}

ostream& operator << (ostream &out, variable v) {
    return out << '[' << v.ind << ',' << (v.flip ? "true" : "false") << ']';
}
ostream& operator << (ostream &out, const op &o) {
    out << '[' << '\"' << o.opcode << '\"' << ',' << '[';
    bool printed = false;
    for (variable v : o.args) {
        if (printed) out << ',';
        printed = true;
        out << v;
    }
    out << ']';
    out << ']';
    return out;
}

void print() {
    cout << '[' << input_length << ',' << '[';
    {
        bool printed = false;
        for (size_t i = input_length; i < ops.size(); i++) {
            if (printed) cout << ',';
            printed = true;
            cout << ops[i];
        }
    }
    cout << ']' << ',' << '[';
    {
        bool printed = false;
        for (variable v : outputs) {
            if (printed) cout << ',';
            printed = true;
            cout << v;
        }
    }
    cout << ']';
    cout << ']';
    cout << endl;
}

int main() {
    go();
    cerr << (ops.size() - input_length) << " ops" << '\n';
    print();
}
