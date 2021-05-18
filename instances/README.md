RCP Format
==========

## Instances

Each RCP file contains a sequence of whitespace delimited numbers.
Whitespace includes tab, space and newline characters.

The first line contains two integers, the number of jobs `n` and the number of 
resources `r`.

The second line contains numbers `R[k]`, `k = 1..r` corresponding
to the amount of available units of resource `k` at any time.

The next `n` lines correspond to the jobs `i = 1..n`.
One line contains at least `2 + r` numbers:
The processing time `p[i]`, the resource requirements `r[i,k]` and the number
of successors `a[i]`.
If `a[i] > 0` then `a[i]` 1-indexed successor IDs follow.


Source: PSPLIB ([http://www.om-db.wi.tum.de/psplib/library.html](http://www.om-db.wi.tum.de/psplib/library.html))

## Solutions

The solution format does not correspond to the PSPLIB's format.

The first line contains a single integer, the number of jobs `n`.
`n` lines follow, each containing two integers, the job number `i = 1..n` and
the starting time `s[i]`.
