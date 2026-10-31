#!/usr/bin/env python3

import lightgbm as lgb
import numpy as np
import sys
from sklearn.metrics import balanced_accuracy_score
from sklearn.model_selection import train_test_split


def train_and_eval(X, y, X_val, y_val, X_test, y_test):
    train_data = lgb.Dataset(np.array(X), label=np.array(y))
    val_data = lgb.Dataset(np.array(X_val), label=np.array(y_val))
    param = {'num_leaves': 20,
             'objective': 'multiclass',
             'num_threads':1,
             'verbosity': -1,
             'num_classes':len(set(y_train)),
             'metric': 'multi_logloss'}
    callback = lgb.early_stopping(
                stopping_rounds=20,
                verbose=False
    )
    
    bst = lgb.train(param, train_data, 200, valid_sets=val_data, callbacks=[callback])
    y_hat = bst.predict(X_test)
        
    return balanced_accuracy_score(y_test, np.argmax(y_hat,axis=1))

try:
    inp = sys.argv[1]
    tes = sys.argv[2]
    oup = sys.argv[3]
    
    X_train = []
    y_train = []
    with open(inp) as f:
        N,M,K = (int(x) for x in f.readline().strip().split())
        for _ in range(N):
            parts = f.readline().strip().split()
            X_train.append(list(float(x) for x in parts))
        for _ in range(N):
            y_train.append(int(f.readline().strip()))

    X_hidden = []
    y_hidden = []
    with open(tes) as f:
        lines = f.readlines()
        min_score, max_score = map(float, lines[0].split())
        for line in lines[1:]:
            parts = line.strip().split()
            if len(parts) > 1:
                X_hidden.append(list(map(float, parts)))
            else:
                y_hidden.append(int(parts[0]))


    with open(oup) as f:
        to_remove = set([int(x) for x in f.readlines()])
        assert len(to_remove) == K, f"output file wrong format, found {len(to_remove)} lines, expected {K}"
        assert (all([0 <= idx <= len(X_train) for idx in to_remove])), "all indexes must be valid"

        X_train_mod = [x for i,x in enumerate(X_train) if i not in to_remove]
        y_train_mod = [y for i,y in enumerate(y_train) if i not in to_remove]


    X_test, X_val, y_test, y_val = train_test_split(X_hidden, y_hidden, test_size=0.5, random_state=17)
    score = train_and_eval(X_train_mod, y_train_mod, X_val, y_val, X_test, y_test)

    res = min(1, max(0, (score - min_score)/(max_score - min_score)))
    print(format(res, '0.5f'))


except Exception as e:
    #raise e
    print(0.0)
    
