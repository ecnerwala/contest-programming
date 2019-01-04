package main

import (
	"fmt"
	"log"

	"gopkg.in/src-d/go-git.v4"
	"gopkg.in/src-d/go-git.v4/plumbing"
	"gopkg.in/src-d/go-git.v4/plumbing/filemode"
	"gopkg.in/src-d/go-git.v4/plumbing/object"
)

const MOD = 1000 * 1000 * 1000 + 7

type TreeSum struct {
	CurHash int64
	LenCoeff int64
	PrefInd int64
	NumLines int64
	TotSum int64
	TotLen int64
}

func NewTreeSum(lines int64) TreeSum {
	res := TreeSum{}
	res.CurHash = 0
	res.LenCoeff = 0
	res.PrefInd = 0
	res.NumLines = lines
	res.TotSum = 0
	res.TotLen = 0
	return res
}

func (t TreeSum) Value() int64 {
	return (t.CurHash + t.TotSum) % MOD // 1-index
}

func (t TreeSum) AddChar(c rune) TreeSum {
	res := t

	res.CurHash += res.LenCoeff
	res.CurHash %= MOD

	res.PrefInd += (res.NumLines) * (res.NumLines - 1) % MOD * ((MOD + 1) / 2) % MOD
	res.PrefInd %= MOD

	res.CurHash += res.PrefInd * int64(c) % MOD
	res.CurHash %= MOD

	res.TotSum += int64(c) * res.NumLines % MOD
	res.TotSum %= MOD

	res.LenCoeff += (res.NumLines) * (res.NumLines + 1) % MOD * ((MOD + 1) / 2) % MOD * int64(c) % MOD
	res.LenCoeff %= MOD

	res.TotLen += res.NumLines
	res.TotLen %= MOD

	return res
}

func (t TreeSum) AddString(s string) TreeSum {
	res := t
	r := []rune(s)
	for i := len(r) - 1; i >= 0; i-- {
		res = res.AddChar(r[i])
	}
	return res
}

func (t TreeSum) Concat(u TreeSum) TreeSum {
	res := TreeSum{}

	res.CurHash = t.CurHash + t.TotLen * u.TotSum + u.CurHash
	res.CurHash %= MOD

	res.PrefInd = t.PrefInd + u.PrefInd + t.TotLen * u.NumLines
	res.PrefInd %= MOD
 
	res.LenCoeff = t.LenCoeff + u.LenCoeff + u.TotSum * t.NumLines
	res.LenCoeff %= MOD

	res.NumLines = t.NumLines + u.NumLines
	res.NumLines %= MOD

	res.TotSum = t.TotSum + u.TotSum
	res.TotSum %= MOD
	
	res.TotLen = t.TotLen + u.TotLen
	res.TotLen %= MOD

	return res
}

type Repo struct {
	r *git.Repository
	hashCache map[plumbing.Hash]TreeSum
}

func (r *Repo) ComputeTreeSum(t *object.Tree) TreeSum {
	if cachedSum, ok := r.hashCache[t.Hash]; ok {
		return cachedSum
	}

	res := NewTreeSum(0)
	for _, entry := range t.Entries {
		if entry.Mode == filemode.Dir {
			s, err := r.r.TreeObject(entry.Hash)
			if err != nil {
				log.Fatal(err)
			}
			cur := r.ComputeTreeSum(s)
			cur = cur.AddChar('/')
			cur = cur.AddString(entry.Name)
			res = res.Concat(cur)
		} else {
			cur := NewTreeSum(1)
			cur = cur.AddChar('\n')
			cur = cur.AddString(entry.Name)
			res = res.Concat(cur)
		}
	}

	r.hashCache[t.Hash] = res
	return res
}

func main() {
	repo, err := git.PlainOpen("./git")
	if err != nil {
		log.Fatal(err)
	}

	r := Repo{repo, map[plumbing.Hash]TreeSum{}}

	h, err := r.r.ResolveRevision("hard")
	if err != nil {
		log.Fatal(err)
	}
	c, err := r.r.CommitObject(*h)
	if err != nil {
		log.Fatal(err)
	}

	log.Println(c)

	t, err := c.Tree()
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(r.ComputeTreeSum(t).Value())
}
