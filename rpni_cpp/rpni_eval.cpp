#include "rpni_eval.hpp"

RPNIEval::RPNIEval() = default;

void RPNIEval::get_dev_acc(vector<string> dev_pos, vector<string> dev_neg, Automaton rpni) {
    // calculate the number of correctly classified positive examples
    float n_correct = 0;

    for (string p_ex: dev_pos) {
        if (rpni.accept(p_ex)) {
            n_correct++;
        }
    }

    // same for the negative examples
    int n_correct_neg = 0;
    for (string n_ex: dev_neg) {
        if (!rpni.accept(n_ex)) {
            n_correct++;
        }
    }

    float total = dev_pos.size() + dev_neg.size();
    float res = n_correct / total;
    printf("Dev accuracy: %.2f", res);
}

vector<string> RPNIEval::read_wordlist(const string& path) {
    ifstream reader(path);
    vector<string> wlist;
    string line;

    // see if the path is valid
    if (!reader.is_open()) {
        return wlist;
    }

    // otherwise, read each line and return wlist
    while (getline(reader, line)) {
        wlist.push_back(line);
    }

    return wlist;
}