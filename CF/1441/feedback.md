Well that was way too hard. I didn't really have the best/most focussed day,
but that was clearly too hard for many many testers.

## A. Identify the Operations

This seems like a decent problem, though it's quite hard and theorem-ey. Maybe good D1C?

I have no idea why you gave the original permutation, because you just
immediately invert it and it adds nothing to the problem.

## B. Graph Transpositions

Also a decent problem, it felt very implementation heavy, I don't know if you
had a simpler solution than mine. I built a graph with like N*(log_2(N)+2)
nodes; I could've built 2, but that's obviously a lot of code too. Also a
decent D1C level problem I think.

## C. Sum

I couldn't figure this out... The convexity means that there's at most one pile
that you don't take all/none of, but I'm not sure how to proceed from there;
you can't do the prefix/suffix trick or the make/unmake trick because we're doing max.

The description of the operation is a little strange; you probably don't need 3
bullets to describe "remove the first element of one array, and add it to your
sum".

## D. Black, White and Grey Tree

This seemed tractable but still quite hard; I didn't spend much time and don't
have a solution, but I expect it's some kind of merge-smaller-into-bigger
convexity tree-dp.

## E. Matching

I didn't spend any time on this. I don't really like planar graphs, matchings,
or constructive problems on graphs, so it's not really my jam. Still, this seems
particularly mathy and I'm not sure how well received it would be.

Also, this seems very hard.

You could replace "You are given a planar graph on a plane" with "You are given
a planar graph and the coordinates of all it's vertices."

## F. Differentiating Games

This was an interesting problem, but it seems super knowledge based. I started
looking up papers and found
<https://www.msri.org/people/staff/levy/files/Book56/12siegel.pdf>, which gave
the classification of impartial loopy games and their sums. In particular, the
equivalence classes of impartial loopy games are the nimbers and infinite games
classified by their set of nimber-valued successors.

For this particular problem, it looks like you want one node with each state of
`*0`-`*9`, and then each other node is infinite (e.g. by adding a self-loop) and
connects to a different bitmask of `*0`-`*9`. This would actually only use 10
queries each. However, I couldn't figure out a construction that uses only 4096
changes, this would use up to 6000 changes.

One alternative solution I thought of is to make `*0`-`*19` and then connect
each other node to a distinct subset of these; by using the additional
flexibility, you probably can squeeze into 4096? That seems like a really ugly
solution to implement, so I wouldn't recommend trying to force this, especially
in this very hard round. Perhaps you can increase the limit of K to 6000 or
7000?

Overall, this problem seems to benefit a lot from knowledge of loopy
combinatorial games, which isn't great.

In the second paragraph, you could clarify that stones are allowed to occupy the
same locations, and say "A player loses if they cannot make a move, i.e. all
stones are on vertices with outdegree 0."
