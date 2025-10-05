#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to represent each node in the segment tree
struct Node {
    int value = 0;         // The value to propagate down the tree
    int max_len = 0;       // The length of the longest non-decreasing subsequence in the segment
    int prefix_len = 0;    // The length of the longest non-decreasing subsequence starting at the beginning of the segment
    int suffix_len = 0;    // The length of the longest non-decreasing subsequence ending at the end of the segment
    int left_value = 0;    // The value of the first element in the segment
    int right_value = 0;   // The value of the last element in the segment
    int ans_prefix = -1;   // The "answer" prefix length for query calculations
    int ans_suffix = -1;   // The "answer" suffix length for query calculations
    int ans_max = -1;      // The maximum non-decreasing subsequence length in the segment (used for queries)
};

class SegmentTree {
private:
    vector<Node> tree;  // The segment tree is stored as a array
    int n;              // The size of the input array

    // Helper function to propagate the value of the current node to its children
    void unpack(int idx) {
        // No propagation needed
        if (tree[idx].value == 0) return;

        int left_child = 2 * idx;
        int right_child = 2 * idx + 1;

        // Propagate value to children (we do the unpack function only on
        // not-a-leaf node)

        tree[left_child].value += tree[idx].value;
        tree[left_child].left_value += tree[idx].value;
        tree[left_child].right_value += tree[idx].value;

        tree[right_child].value += tree[idx].value;
        tree[right_child].left_value += tree[idx].value;
        tree[right_child].right_value += tree[idx].value;

        // Reset the current node's value after propagation
        tree[idx].value = 0;
    }

    // Updates the current node based on the values of its children
    void update_node(int idx, int l, int r) {
        int mid = (l + r) / 2;
        int left_child = 2 * idx;
        int right_child = 2 * idx + 1;

        tree[idx].left_value = tree[left_child].left_value;
        tree[idx].right_value = tree[right_child].right_value;

        tree[idx].prefix_len = tree[left_child].prefix_len;
        // Whole left child is non-decreasing - maybe prefix is larger than that
        if (tree[left_child].prefix_len == mid - l + 1
            && tree[left_child].right_value <= tree[right_child].left_value) {
            tree[idx].prefix_len += tree[right_child].prefix_len;
        }

        tree[idx].suffix_len = tree[right_child].suffix_len;
        // If the entire right child is non-decreasing and can be extended with the left child
        if (tree[right_child].suffix_len == r - mid && tree[left_child].right_value <= tree[right_child].left_value) {
            tree[idx].suffix_len += tree[left_child].suffix_len;
        }

        // Calculate the maximum non-decreasing subsequence length
        tree[idx].max_len = max({tree[left_child].max_len, tree[right_child].max_len});
        if (tree[left_child].right_value <= tree[right_child].left_value) {
            tree[idx].max_len = max(tree[idx].max_len, tree[left_child].suffix_len + tree[right_child].prefix_len);
        }
    }

    // Apply fertilizer (increment) to a given range
    void apply_fertilizer(int idx, int l, int r, int a, int b, int k) {
        if (a > r || b < l) {   // Out of range
            return;
        }

        if (a <= l && r <= b) {   // we are entirely in the searched segment
            tree[idx].value += k;
            tree[idx].left_value += k;
            tree[idx].right_value += k;
            return;
        }

        // Propagate values down before proceeding
        unpack(idx);

        int mid = (l + r) / 2;
        apply_fertilizer(2 * idx, l, mid, a, b, k);
        apply_fertilizer(2 * idx + 1, mid + 1, r, a, b, k);

        // Update the current node after processing children
        update_node(idx, l, r);
    }

    // Query the maximum non-decreasing subsequence in a given range
    bool query(int idx, int l, int r, int a, int b) {
        // Out of range
        if (a > r || b < l) return false;

        // Whole segment of this node inside a searched segment
        if (a <= l && r <= b) {
            tree[idx].ans_prefix = tree[idx].prefix_len;
            tree[idx].ans_suffix = tree[idx].suffix_len;
            tree[idx].ans_max = tree[idx].max_len;
            return true;
        }

        // Propagate values down before proceeding
        unpack(idx);

        int mid = (l + r) / 2;
        bool left = query(2 * idx, l, mid, a, b);
        bool right = query(2 * idx + 1, mid + 1, r, a, b);

        // Update results based on children
        if (!left) {
            // If left-child has nothing in common with searched segment
            tree[idx].ans_suffix = tree[2 * idx + 1].ans_suffix;
            tree[idx].ans_max = tree[2 * idx + 1].ans_max;
            tree[idx].ans_prefix = -1;
            return true;
        }

        if (!right) {
            // -||-
            tree[idx].ans_prefix = tree[2 * idx].ans_prefix;
            tree[idx].ans_max = tree[2 * idx].ans_max;
            tree[idx].ans_suffix = -1;
            return true;
        }

        // If both children have fragments of the answer
        tree[idx].ans_max = max({tree[2 * idx].ans_max, tree[2 * idx + 1].ans_max});
        tree[idx].ans_prefix = tree[2 * idx].ans_prefix;
        tree[idx].ans_suffix = tree[2 * idx + 1].ans_suffix;

        // Checking if the suffix of the left child can be combined with the prefix of the right child
        if (tree[2 * idx].right_value <= tree[2 * idx + 1].left_value) {
            tree[idx].ans_max = max(tree[idx].ans_max,
                                         tree[2 * idx].ans_suffix + tree[2 * idx + 1].ans_prefix);

            // Extend the prefix if the prefix of the left child covers the entire interval
            if (tree[2 * idx].ans_prefix == (mid - l + 1)) {
                tree[idx].ans_prefix = tree[2 * idx].ans_prefix + tree[2 * idx + 1].ans_prefix;
            }

            // Extension of the suffix if the right child suffix covers the entire interval
            if (tree[2 * idx + 1].ans_suffix == (r - mid)) {
                tree[idx].ans_suffix = tree[2 * idx + 1].ans_suffix + tree[2 * idx].ans_suffix;
            }
        }

        return true;
    }

    void build(int idx, int l, int r) {
        // leaf
        if (l == r) {
            tree[idx] = {0, 1, 1, 1, 0, 0, -1, -1, -1};
            return;
        }
        tree[idx] = {0, r - l + 1, r - l + 1, r - l + 1, 0, 0, -1, -1, -1};
        int mid = (l + r) / 2;
        build(2 * idx, l, mid);
        build(2 * idx + 1, mid + 1, r);
    }

public:
    SegmentTree(int m) {
        /*
            at first max_len, prefix_len, suffix_len =
            length of segment in every node
        */
        n = m;
        // 4 * m is enough for segment tree which will hold segment [1, m]
        tree.resize(4 * m, Node());
        // only root has current value (height of trees)
        // value, max_len, ...
        tree[1] = {1, m, m, m, 1, 1, -1, -1, -1};
        if (m > 1) {
            int mid = (1 + m) / 2;
            build(2, 1, mid);
            build(3, mid + 1, m);
        }
    }

    void apply_fertilizer(int a, int b, int k) {
        apply_fertilizer(1, 1, n, a, b, k);
    }

    int max_non_decreasing(int a, int b) {
        query(1, 1, n, a, b);
        return tree[1].ans_max;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    SegmentTree segTree(n);

    for (int i = 0; i < m; ++i) {
        char type;
        cin >> type;
        if (type == 'N') {
            int a, b, k;
            cin >> a >> b >> k;
            segTree.apply_fertilizer(a, b, k);
        } else if (type == 'C') {
            int a, b;
            cin >> a >> b;
            cout << segTree.max_non_decreasing(a, b) << "\n";
        }
    }

    return 0;
}