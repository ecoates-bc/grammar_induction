#ifndef _AUTOMATON
#define _AUTOMATON

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include <queue>
#include <tuple>

using namespace std;

// A transition in an automaton: it includes the ID of the 'from' and 'to' nodes, and a vector of characters
struct Transition {
    int from;
    int to;
    vector<char> on;
};

// A node in an automaton: it has a unique integer ID, and if it's a start and/or accept state
struct State {
    int id;
    bool start;
    bool accept;
};


class Automaton {
    public:
        int max_state;
        vector<shared_ptr<State>> states;
        vector<shared_ptr<Transition>> transitions;

        Automaton();
        Automaton(vector<string> tokens);
        Automaton(const Automaton &orig);

        bool is_equal(const Automaton &other);

        void add_state(bool start, bool accept);
        void add_state(bool start, bool accept, int id);
        void remove_state(int id);
        void add_transition(int from, int to, char on);
        bool accept(string s);

        shared_ptr<Transition> get_transition_from(int from, char on);
        vector<shared_ptr<Transition>> transitions_from(int from);
        shared_ptr<State> get_state(int id);

        Automaton merge_states(int a_id, int b_id);
        int get_max_state();

        Automaton d_step();
        Automaton dmerge(int a_id, int b_id);

        Automaton rpni(vector<string> neg);
        Automaton blue_step(int a_id, vector<shared_ptr<State>> reds, vector<string> neg);

        void generate_dot();

    private:
        vector<shared_ptr<State>> remove_state_from_rpni_colorset(vector<shared_ptr<State>> colorset, Automaton red_step);
};

#endif