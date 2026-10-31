#!/usr/bin/env python3

from pathlib import Path, PurePath
import pyperclip
import json

ID = "oi/21"

PROBLEM_LIST = """
Zadanie Bar sałatkowy (bar)	
Zadanie Hotele (hot)	
Zadanie Klocki (klo)	
Zadanie Kurierzy (kur)	
Zadanie Wąż (waz)	
Second stage
Trial day
Zadanie Karty (kar)	
First day
Zadanie Przestępcy (prz)	
Zadanie Superkomputer (sup)	
Second day
Zadanie Ptaszek (pta)	
Zadanie Rajd (raj)	
Final stage
Trial day
Zadanie FarmerCraft (far)	
First day
Zadanie Dookoła świata (doo)	
Zadanie Mrowisko (mro)	
Turystyka (tur)	
Second day
Zadanie Lampy słoneczne (lam)	
Zadanie Panele słoneczne (pan)	
Zadanie Załadunek (zal)	
"""

PROBLEMS = PROBLEM_LIST.strip().split('\n')
PROBLEMS = [l.strip() for l in PROBLEMS]
PROBLEMS = [l for l in PROBLEMS if l not in ['First stage', 'Second stage', 'Final stage', 'Trial day', 'First day', 'Second day']]
for l in PROBLEMS:
    assert l[-5] == '(' and l[-1] == ')', f"Bad row {l = }"
PROBLEMS = [l[-4:-1] for l in PROBLEMS]

SUBMISSION_DIR = Path(__file__).resolve().parent / ID
assert SUBMISSION_DIR.is_dir()

def load_json(path):
    with path.open() as f:
        return json.load(f)

submissions_json = load_json(SUBMISSION_DIR / 'submissions.json')

for p in SUBMISSION_DIR.iterdir():
    if p.name == 'submissions.json': continue
    if p.name not in PROBLEMS:
        print(f"WARNING: Szkopul does not have problem {p.name}")

for prob in PROBLEMS:
    print(f"Problem {prob}:")
    d = SUBMISSION_DIR / prob
    if not d.exists():
        print(f"No submissions; SKIP")
        best_score = 0
        best_file = "SKIP_ME"
        best_file_contents = ''
    else:
        best_score = -1
        best_file = None
        for f in sorted(d.iterdir()):
            if f.suffix == '.json':
                data = load_json(f)
                id_ = data["id"]
                score = data["points"]["received"]
                max_score = data["points"]["maximum"]
                print(f"Got submission {id_}: {score}/{max_score}")
                if score >= best_score:
                    best_score = score
                    best_file = d / (f.stem + '.cpp')
        with best_file.open() as f:
            best_file_contents = f.read()
    while True:
        pyperclip.copy(best_file_contents)
        print(f"Copied submission {best_file} ({best_score})")
        t = input("Continue? [Yn]")
        if not t: break
        elif t.lower().startswith('y'): break
        elif t.lower().startswith('n'): continue
        else: continue
