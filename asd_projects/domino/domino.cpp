#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <cstdint>

using namespace std;

/* represents as binary strings the places that are occupied by the right
parts of the horizontal dominoes, to be more precise I keep two arrays
of the best results for the current and the next column possible to obtain
having in these rows the right parts of the dominants, in which in the current
binary sequence occurs 1: for example, the sequence 1001 means that in the 1st
and 4th row of a given column the field is occupied by a horizontal dominoes
protruding into the previous column, the remaining fields are free. Of course,
such strings are represented as integers of type int, and besides, I keep
a bool telling which array is responsible for the current column and the next
one, thus avoiding assignments of the results from the array of the next column
to the current one, I just remember that when I go to the next col I only
remember to change bool to the opposite, of course, and to set all the cells
in the array now responsible for the results of the next column as -1.

In addition, we can observe that it makes no sense to examine sequences where
a certain number 1 occurs next to each other, such as 1110, because, for
example, instead of placing in 2. and 3. (or 1. and 2.) rows of the current
and previous columns horizontal dominoes, we could give two vertical ones
covering the same fields, so we would check unnecessarily 2 times each of
such cases. This is why we keep the bool canHoriz in backtracking to tell us
if we can put a horizontal dominoes, and if we put a horizontal one, we can't
in the next one.

If I could take vertical domino and decided not to take anything, then in the
next step I have to take something, so if it will not be profitable for me to
take any dominoes (vertical or horizontal) then, I should do a return, because
there will not be the best answer from this (it was better to take vertical
domino then) (bool needTake)
*/

int n, k; // board sizes
vector<vector<int64_t>> board; // board as a vector of vectors
vector<int64_t> false_array; // array of results of current column (initially)
vector<int64_t> true_array; // array of results of next column (initially)
bool which = false; // tells which array holds the results
// of the current column
int64_t result = 0;
int curr_col = 0;

// occupied means if current field is occupied by any domino
// curr_mask informs if next field in column is occupied by right part of
// horizontal domino (in fact, the first bit of the mask tells you about it)
// next_mask actually tells us in which rows we chose to put horizontal domino,
// because then in next column there will be right parts of all those dominoes
// curr_score ofc sum of currently occupied positions in a board
// needTake if I should take some domino
void backtracking(int row, bool canHoriz, bool occupied, int curr_mask,
                  int next_mask, int64_t curr_score, bool needTake) {
    if (row == k) { // we passed through a whole column
        result = max(result, curr_score);
        if (!which) // update score for mask in next col
            true_array[next_mask] = max(curr_score, true_array[next_mask]);
        else
            false_array[next_mask] = max(curr_score, false_array[next_mask]);
        return;
    }
    // if next bit of curr_mask is 1, then it means there is a right part
    // of a horizontal domino
    curr_mask = curr_mask >> 1;
    bool next_occupied = (curr_mask & 1) != 0;
    if (occupied) {
        // we cannot do anything, so we just iterate to the next row
        backtracking(row + 1, true, next_occupied,
                     curr_mask, next_mask, curr_score, false);
    } else {
        bool couldTakeVertical = false; // if I could take vertical domino

        // we should check if we can put domino vertically and if it is worth
        // to do it, so we shouldn't be in last row because then bottom half
        // of domino would exceed our number of rows and also cell in which
        // we want to put it should not be occupied
        if (row < k - 1 && !next_occupied) {
            // we should put dominoes only if value of the fields covered by
            // them is sth more than a zero
            int64_t domino = board[row][curr_col] + board[row + 1][curr_col];
            if (domino > 0) {
                couldTakeVertical = true;
                // value of canHoriz is ofc not important because next cell
                // in this col will be occupied anyway
                backtracking(row + 1, true, true,
                             curr_mask, next_mask, curr_score + domino, false);
            }

        }
        if (canHoriz) {
            // we should put dominoes only if value of the fields covered by
            // them is sth more than a zero
            int64_t domino = board[row][curr_col] + board[row][curr_col + 1];
            if (domino > 0) {
                // canHoriz will be false in next row because we decided
                // to put it right now
                // we should note that we put domino horizontally in this
                // row in the next mask
                backtracking(row + 1, false, next_occupied, curr_mask, 
                    next_mask | (1 << row), curr_score + domino, false);
            }
        }
        if (!needTake) { // we did not have to take any domino now
            // we can choose not to cover this cell in a board, but if we could
            // take vertical domino now and decided not to do it, then in the 
            // next row we must take some domino (to take next cell) if possible
            backtracking(row + 1, true, next_occupied,
                         curr_mask, next_mask, curr_score, couldTakeVertical);
        }
    }
}

int main() {
    cin >> n >> k;
    board.resize(k, vector<int64_t>(n + 1));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> board[i][j];
        }
    }
    for (int i = 0; i < k; ++i) {
        // we make additional col to always remember not to
        // take horizontal domino in the last col
        board[i][n] = INT_MIN;
    }
    int bin_seq = pow(2, k); // binary sequences
    false_array.resize(bin_seq, -1);
    true_array.resize(bin_seq, -1);

    // 1st column is empty, not occupied fields by right parts of the
    // horizontal dominoes, so curr_mask is 0
    // next_mask will be always 0 at first and in backtracking function
    // I will change these bits to 1, where I will put horizontal dominoes
    backtracking(0, true, false, 0, 0, 0, false);

    for (int col = 1; col < n; ++col) {
        curr_col = col;
        // after we ended operations for the last column, our next column
        // became current, so array in which we held results for masks of
        // next col is current now we should change which:
        which = 1 - which;
        // which tells us where are results for current col
        if (which) {
            // we should change every cell of false_array to -1, because right
            // now there will be stored results for next col
            for (int i = 0; i < bin_seq; ++i) {
                false_array[i] = -1;
            }
        } else {
            // we should change every cell of true_array to -1, because right
            // now there will be stored results for next col
            for (int i = 0; i < bin_seq; ++i) {
                true_array[i] = -1;
            }
        }

        for (int i = 0; i < bin_seq; ++i) {
            if (which) {
                // results for masks of current col in true_array
                if (true_array[i] >= 0) {
                    // if it is < 0 then it cannot lead to best result
                    // we start always at the first row, canHoriz is set
                    // to true, but if place is occupied then it won't
                    // even matter
                    bool occupied = (i & 1) != 0;
                    backtracking(0, true, occupied, i, 0, true_array[i], false);
                    // row, canHoriz, occupied, curr_mask, next_mask,
                    // curr_score, needTake
                }
            } else {
                // results for masks of current col in false_array
                if (false_array[i] >= 0) {
                    bool occupied = (i & 1) != 0;
                    backtracking(0, true, occupied, i, 0, false_array[i], false);
                    // row, canHoriz, occupied, curr_mask, next_mask,
                    // curr_score, needTake
                }
            }
        }
    }
    cout << result;
}