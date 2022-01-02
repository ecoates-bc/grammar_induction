#ifndef RPNI_CPP_RPNI_EVAL_HPP
#define RPNI_CPP_RPNI_EVAL_HPP

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>

#include "automata.hpp"

using namespace std;

class RPNIEval {
    public:
        RPNIEval();
        void get_dev_acc(vector<string> dev_pos, vector<string> dev_neg, Automaton rpni);
        static vector<string> read_wordlist(const string& path);
};

#endif
