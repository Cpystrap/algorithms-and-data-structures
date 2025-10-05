# Domino

**Algorithms and Data Structures Laboratory — Assignment 1**  
**Available memory:** 128 MB  

---

## Problem Description

An integer is written in each cell of a rectangular board of size \(k \times n\).  
A placement of \(1 \times 2\) domino tiles on this board consists of placing some dominoes so that each domino covers exactly two cells, and no cell is covered by more than one domino.  

The **value** of such a placement is defined as the sum of the numbers written on all covered cells.

Your task is to determine the **maximum possible value** of a domino placement on the board.

---

## Input

The first line of input contains two integers:  
`n`, `k` — the dimensions of the board  
\((1 \le n \le 1000,\ 1 \le k \le 12)\).

Each of the next `k` lines contains `n` integers separated by single spaces — the numbers written on the board cells.  
These integers are in the range \([-10^6, 10^6]\).

Formally, the \(i\)-th line (\(1 \le i \le k\)) contains the values:

\[
a_{i,1}, a_{i,2}, \ldots, a_{i,n}
\]

where each value satisfies \(-10^6 \le a_{i,j} \le 10^6\) for \(1 \le j \le n\).  
The cell at coordinates \((i, j)\) contains the number \(a_{i,j}\).

---

## Output

Your program should print a single integer —  
the **maximum possible value** of a domino placement on the board.

---

## Example

### Input
3 2
0 2 5
-2 -3 7


### Output
14

---

## Explanation

One optimal placement is:
- Place a horizontal domino covering the two cells with values **5** and **7** (bottom row, columns 2–3),  
- and another domino covering the cells **0** and **2** (top row, columns 1–2).  

The total sum of covered cells is:

\[
(0 + 2) + (5 + 7) = 14
\]
