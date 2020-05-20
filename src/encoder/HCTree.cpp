#include "HCTree.hpp"

/* TODO: Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() { /*recursiveDestructor(root);*/
}

/*void HCTree::recursiveDestructor(HCNode* n) {
    if (n && n->c0 == NULL) delete n;

    recursiveDestructor(n->c0);
    recursiveDestructor(n->c1);
    delete n;
}*/

/**
 * TODO: Build the HCTree from the given frequency vector. You can assume
 * the vector must have size 256 and each value at index i represents the
 * frequency of char with ASCII value i. Only non-zero frequency symbols
 * should be used to build the tree. The leaves vector must be updated so
 * that it can be used in encode() to improve performance.
 *
 * When building the HCTree, you should use the following tie-breaking rules
 * to match the output from reference solution in checkpoint:
 *
 *    1. HCNode with lower count should have higher priority. If count is
 * the same, then HCNode with a larger ascii value symbol should have higher
 * priority. (This should be already defined properly in the comparator in
 * HCNode.hpp)
 *    2. When popping two highest priority nodes from PQ, the higher
 * priority node will be the ‘c0’ child of the new parent HCNode.
 *    3. The symbol of any parent node should be taken from its 'c0' child.
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for (int index = 0; index < 256; index++) {
        if (freqs[index] > 0) {
            HCNode* n = new HCNode(freqs[index], (char)index);
            pq.push(n);
            leaves.push_back(n);
        }
    }

    while (pq.size() > 1) {
        HCNode* c0 = pq.top();
        pq.pop();
        HCNode* c1 = pq.top();
        pq.pop();

        int count = c0->count + c1->count;
        HCNode* newNode = new HCNode(count, c0->symbol);

        c0->p = newNode;
        c1->p = newNode;
        newNode->c0 = c0;
        newNode->c1 = c1;

        pq.push(newNode);
        // cout << ":added parent" << endl;
        // cout << "symbol =" << newNode->symbol << endl;
        // cout << "c0 symbol" << newNode->c0->symbol << endl;
        // cout << "c1 symbol" << newNode->c1->symbol << endl;
    }
    root = pq.top();
}

/**
 * TODO: Write the encoding bits of the given symbol to the ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
// void HCTree::encode(byte symbol, BitOutputStream& out) const {}

/**
 * TODO: Write the encoding bits of the given symbol to ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been called
 * beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    char s = (char)symbol;
    ////cout << "CHAR::::::" << s << endl;
    vector<char> bits;
    for (char c : bits) {
        // cout << c << "..............";
        out << c;
    }
    for (HCNode* m : leaves) {
        if (m->symbol == symbol) {
            // cout << "leaves[s]........" << symbol << endl;
            HCNode* n = m;
            while (n != root) {
                // cout << n->symbol << "----" << endl;
                if (n->p->symbol == n->symbol) {
                    bits.push_back('0');
                    // cout << "Pushed0" << endl;
                } else {
                    bits.push_back('1');
                    /*cout << "Pushed1" << endl;*/
                }
                n = n->p;
            }
        }
    }
    for (char c : bits) {
        // cout << c << "..............";
        out << c;
    }
}

/**
 * TODO: Decode the sequence of bits (represented as a char of either '0' or
 * '1') from the istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/**
 * TODO: Decode the sequence of bits (represented as char of either '0' or '1')
 * from istream to return the coded symbol. For this function to work, build()
 * must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const {
    HCNode* node = root;
    char m;

    while (node && node->c0) {
        in >> m;
        // cout << "CHAR" << m;
        if (m == '0') {
            node = node->c0;
        } else if (m == '1') {
            node = node->c1;
        }
    }

    if (node) return node->symbol;
    return ' ';
}
