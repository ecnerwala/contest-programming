#!/usr/bin/env python3
"""Rebuild the public repo as the filtered history of HEAD's public files.

A `.public` file holds gitignore-syntax patterns, relative to its directory,
for files to publish; deeper `.public`s override shallower ones (`!` excludes).
`/*` publishes the whole directory. Patterns are matched against every path in
history after applying MOVES, so a moved directory keeps its history under its
current name. The `.public` files themselves are not
published.

Without --push, rewrites history into $GIT_DIR/publish.git with
git-filter-repo, fetches <remote> master there, and prints the diffstat;
nothing is pushed. With --push, force-pushes the reviewed result to <remote>
master (with a lease on the fetched commit).

Usage:
  publish.py [--remote REMOTE]          # remote name or URL; default: public
  publish.py [--remote REMOTE] --push
"""

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

MARKER = '.public'
PUBLIC_REF = 'refs/remotes/public/master'
# Directory moves in history, oldest first; filter-repo follows no renames itself.
MOVES = [
    ('TopCoder/2015-TopCoder-Open-Algorithm', 'TopCoder/2015-TopCoder-Open-Algorithm-3B'),
    ('IPSC/ipsc-2016', 'IPSC/ipsc2016'),
    ('DistributedCodeJam/.dcj', 'DistributedCodeJam/.template/dcj'),
    ('HackerCup', 'FBHackerCup'),
    ('AtCoder/B100', 'AtCoder/ABC100'),
    ('AtCoder/B96', 'AtCoder/ABC096'),
    ('POI/21/klo/klo_files', 'POI/21/klo/problem/klo_files'),
    ('POI/21/lam/lam_files', 'POI/21/lam/problem/lam_files'),
    ('POI/21/sup/sup_files', 'POI/21/sup/problem/sup_files'),
    ('AtCoder/Misc', 'AtCoder/ABC099'),
]


def git(*args: str, env: dict[str, str] | None = None, input: str | None = None) -> str:
    return subprocess.run(['git', *args], env=env, input=input, text=True,
                          check=True, stdout=subprocess.PIPE).stdout


def all_paths(rev: str) -> set[str]:
    return set(git('log', '--format=', '--name-only', '--no-renames', '-z', rev).split('\0')) - {''}


def prefixes(path: str) -> list[str]:
    parts = path.split('/')
    return ['/'.join(parts[:i]) for i in range(1, len(parts))]


def apply_moves(path: str) -> str:
    for o, n in MOVES:
        if path == o or path.startswith(o + '/'):
            path = n + path[len(o):]
    return path


def marked(paths: set[str], tmp: Path) -> set[str]:
    """The subset of `paths` matched by HEAD's .public files."""
    work = tmp / 'work'
    for path in git('ls-tree', '-r', '-z', '--name-only', 'HEAD').split('\0'):
        if path.rsplit('/', 1)[-1] == MARKER:
            dst = work / path
            dst.parent.mkdir(parents=True, exist_ok=True)
            dst.write_text(git('show', f'HEAD:{path}'))
    empty = git('hash-object', '-w', '--stdin', input='').strip()
    env = {**os.environ, 'GIT_INDEX_FILE': str(tmp / 'marked-index')}
    out = set()
    while paths:
        # A path that was a file in some commits and a directory in others can't share an index.
        dirs = {d for p in paths for d in prefixes(p)}
        batch = paths - dirs
        paths = paths & dirs
        Path(env['GIT_INDEX_FILE']).unlink(missing_ok=True)
        git('update-index', '-z', '--index-info', env=env,
            input=''.join(f'100644 {empty}\t{p}\0' for p in batch))
        out.update(git(f'--work-tree={work}', 'ls-files', '-z', '-ci',
                       f'--exclude-per-directory={MARKER}', env=env).split('\0'))
    return {p for p in out if p and p.rsplit('/', 1)[-1] != MARKER}


def check_head(public: set[str], tmp: Path) -> None:
    env = {**os.environ, 'GIT_INDEX_FILE': str(tmp / 'head-index')}
    git('read-tree', 'HEAD', env=env)
    head = {}
    for line in git('ls-files', '-z', '-s', env=env).split('\0'):
        if line:
            info, path = line.split('\t', 1)
            head[path] = info.split()[0]
    paths = [p for p in head if p in public]
    attrs = git('check-attr', '-z', '--cached', '--stdin', 'filter', env=env,
                input=''.join(p + '\0' for p in paths)).split('\0')
    bad = [p for p, v in zip(attrs[0::3], attrs[2::3]) if v == 'lfs']
    bad += [p for p in paths if head[p] == '160000']
    if bad:
        sys.exit('LFS files and submodules cannot be published:\n' + '\n'.join(bad))


def path_filter(paths: set[str], keep: set[str]) -> list[str]:
    """Minimal filter-repo path lines selecting exactly `keep` out of `paths`."""
    mixed = {d for p in paths - keep for d in prefixes(p)}
    lines = set()
    for p in keep:
        lines.add(next((d + '/' for d in prefixes(p) if d not in mixed), p))
    return sorted(lines)


def remote_url(remote: str) -> str:
    return remote if '/' in remote or ':' in remote else git('remote', 'get-url', remote).strip()


def build(remote: str, target: Path) -> None:
    branch = git('symbolic-ref', 'HEAD').strip()
    paths = all_paths('HEAD')
    with tempfile.TemporaryDirectory() as d:
        tmp = Path(d)
        moved = {p: apply_moves(p) for p in paths}
        public = marked(set(moved.values()), tmp)
        check_head(public, tmp)
        keep = {p for p, q in moved.items() if q in public}
        lines = path_filter(paths, keep)
        lines += [f'{o}==>{n}' for o, n in MOVES if any(p == o or p.startswith(o + '/') for p in keep)]
        spec = tmp / 'paths'
        spec.write_text(''.join(line + '\n' for line in lines))
        shutil.rmtree(target, ignore_errors=True)
        git('init', '-q', '--bare', str(target))
        export = subprocess.Popen(
            ['git', '--literal-pathspecs', 'fast-export', '--show-original-ids', '--signed-tags=strip',
             '--tag-of-filtered-object=rewrite', '--fake-missing-tagger', '--reference-excluded-parents',
             '--use-done-feature', '--mark-tags', '--reencode=yes', branch,
             '--', '--', *(line for line in lines if '==>' not in line)],
            stdout=subprocess.PIPE)
        subprocess.run(['git', 'filter-repo', '--quiet', '--stdin', '--target', str(target),
                        '--paths-from-file', str(spec)], stdin=export.stdout, check=True)
        if export.wait():
            sys.exit('git fast-export failed')
    tgt = ['-C', str(target)]
    git(*tgt, 'fetch', '-q', remote_url(remote), f'+master:{PUBLIC_REF}')
    git(*tgt, 'symbolic-ref', 'HEAD', branch)
    subprocess.run(['git', *tgt, '--no-pager', 'diff', '--stat', PUBLIC_REF, 'HEAD'], check=True)
    count = git(*tgt, 'rev-list', '--count', 'HEAD').strip()
    print(f'\n{count} commits. Review: git -C {target} log --stat / diff {PUBLIC_REF} HEAD')
    print(f'Push: {sys.argv[0]} --push')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('--remote', default='public')
    parser.add_argument('--push', action='store_true')
    args = parser.parse_args()
    os.chdir(git('rev-parse', '--show-toplevel').strip())
    target = Path(git('rev-parse', '--path-format=absolute', '--git-common-dir').strip()) / 'publish.git'
    if not args.push:
        build(args.remote, target)
        return
    lease = git('-C', str(target), 'rev-parse', PUBLIC_REF).strip()
    subprocess.run(['git', '-C', str(target), 'push', f'--force-with-lease=master:{lease}',
                    remote_url(args.remote), 'HEAD:master'], check=True)


if __name__ == '__main__':
    main()
