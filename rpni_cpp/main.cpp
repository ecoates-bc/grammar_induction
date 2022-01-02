#include <iostream>
#include <fstream>
#include <string>

#include "automata.hpp"
#include "rpni_eval.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // read files of positive/negative examples, call RPNI on them
    if (argc != 5) {
        cout << "Arguments: ./rpni pos_train neg_train pos_dev neg_dev" << endl;
        return 1;
    }

    vector<string> pos_train = RPNIEval::read_wordlist(argv[1]);
    vector<string> neg_train = RPNIEval::read_wordlist(argv[2]);
    vector<string> pos_dev = RPNIEval::read_wordlist(argv[3]);
    vector<string> neg_dev = RPNIEval::read_wordlist(argv[4]);

    // Start with a prefix tree of the positive set
    Automaton prefix_tree = Automaton(pos_train);

    // Call the RPNI algorithm
    Automaton learn_rpni = prefix_tree.rpni(neg_train);

    // Calculate dev set accuracy
    RPNIEval ev;
    ev.get_dev_acc(pos_dev, neg_dev, learn_rpni);

    return 0;
}