#include "automata.hpp"

Automaton::Automaton() {
    // begin with no states
    max_state = 0;
}

// create a prefix tree automaton from a list of tokens
Automaton::Automaton(std::vector<string> tokens) {
    // begin with a single start state
    max_state = 0;
    add_state(true, false);

    // follow states/transitions until there are no suitable transitions, then create states
    for (string token: tokens) {
        int curr_state = 0;
        for (char c: token) {
            shared_ptr<Transition> trans = get_transition_from(curr_state, c);
            if (trans != nullptr) {
                curr_state = trans->to;
            } else {
                // create a new state/transition
                add_state(false, false);
                add_transition(curr_state, max_state-1, c);
                curr_state = max_state-1;
            }
        }
        // make this state accepting
        shared_ptr<State> final_state = get_state(curr_state);
        final_state->accept = true;
    }
}

// Create a copy of a previous automaton
Automaton::Automaton(const Automaton &orig) {
    // copy states
    for (shared_ptr<State> s: orig.states) {
        add_state(s->start, s->accept, s->id);
    }

    // copy transitions
    for (shared_ptr<Transition> t: orig.transitions) {
        for (char c: t->on) {
            add_transition(t->from, t->to, c);
        }
    }
}

void Automaton::add_state(bool start, bool accept) {
    shared_ptr<State> s = make_shared<State>();
    s->id = max_state;
    s->start = start;
    s->accept = accept;

    states.push_back(s);
    max_state = get_max_state() + 1;
}

void Automaton::add_state(bool start, bool accept, int id) {
    // add a state with a specific ID number, important for maintaining order after merging states
    for (auto s: states) {
        if (s->id == id) {
            cout << "Warning: attempting to duplicate state " << id << endl;
            return;
        }
    }

    shared_ptr<State> s = make_shared<State>();
    s->id = id;
    s->start = start;
    s->accept = accept;

    states.push_back(s);
    max_state++;
}

void Automaton::add_transition(int from, int to, char on) {
    // first, see if there is already a transition from/to these states
    for (shared_ptr<Transition> t: transitions) {
        if (t->from == from && t->to == to) {
            if (find(t->on.begin(), t->on.end(), on) == t->on.end()) {
                t->on.push_back(on);  // don't repeat characters
            }
            return;
        }
    }

    // otherwise, create a new transition
    shared_ptr<Transition> new_t = make_shared<Transition>();
    new_t->from = from;
    new_t->to = to;
    new_t->on.push_back(on);
    transitions.push_back(new_t);
}

shared_ptr<Transition> Automaton::get_transition_from(int from, char on) {
    for (shared_ptr<Transition> t: transitions) {
        if (t->from == from) {
            if (count(t->on.begin(), t->on.end(), on)) {
                return t;
            }
        }
    }

    return nullptr;
}

vector<shared_ptr<Transition>> Automaton::transitions_from(int from) {
    vector<shared_ptr<Transition>> from_vect;
    for (shared_ptr<Transition> t: transitions) {
        if (t->from == from) {
            from_vect.push_back(t);
        }
    }

    return from_vect;
}

shared_ptr<State> Automaton::get_state(int id) {
    for (shared_ptr<State> s: states) {
        if (s->id == id) {
            return s;
        }
    }

    return nullptr;
}

bool Automaton::accept(string s) {
    shared_ptr<State> curr_state;
    for (shared_ptr<State> s: states) {
        if (s->start) {
            curr_state = s;
            break;
        }
    }

    // first, if s is empty, check if the start state is also accepting
    if (s == "") {
        return curr_state->accept;
    }

    // follow characters/transitions and after, return if the current state is accepting
    for (char c: s) {
        shared_ptr<Transition> trans = get_transition_from(curr_state->id, c);
        if (trans != nullptr) {
            curr_state = get_state(trans->to);
        } else {
            // if there are no transitions from this state with this character, reject string
            return false;
        }
    }

    return curr_state->accept;
}

void Automaton::generate_dot() {
    string dot_string = "digraph {";

    // create arcs
    for (shared_ptr<Transition> t: transitions) {
        string arc_string;

        // start with from/to id
        arc_string = arc_string + to_string(t->from);
        arc_string = arc_string + "->";
        arc_string = arc_string + to_string(t->to);

        // create a transition label
        string label_string = "[label=";
        for (char c: t->on) {
            label_string = label_string + c;
        }
        label_string = label_string + ']';
        arc_string = arc_string + label_string + ";";
        dot_string = dot_string + arc_string;
    }

    // create final states
    for (shared_ptr<State> s: states) {
        if (s->accept) {
            string accept_string = to_string(s->id);
            accept_string = accept_string + " [shape=doublecircle];";
            dot_string = dot_string + accept_string;
        }
    }

    // create start state
    for (shared_ptr<State> s: states) {
        if (s->start) {
            string start_string = to_string(s->id);
            start_string = start_string + " [color=green];";
            dot_string = dot_string + start_string;
        }
    }

    dot_string = dot_string + "}";
    cout << dot_string << endl;
}

Automaton Automaton::merge_states(int a_id, int b_id) {
    // create a new automaton - a copy of this one, but states a and b are merged
    Automaton merged_a = Automaton();

    // generate state copies
    for (shared_ptr<State> s: states) {
        merged_a.add_state(s->start, s->accept, s->id);
    }

    // generate transition copies
    for (shared_ptr<Transition> t: transitions) {
        for (char c: t->on) {
            merged_a.add_transition(t->from, t->to, c);
        }
    }

    // merge states in this new copy
    for (shared_ptr<Transition> t: merged_a.transitions) {
        if (t->from == b_id) {
            t->from = a_id;
        } 
        if (t->to == b_id) {
            t-> to = a_id;
        }
    }

    // set merged state's start/accept properties
    shared_ptr<State> a_ptr = merged_a.get_state(a_id);
    shared_ptr<State> b_ptr = merged_a.get_state(b_id);
    
    // TODO: force start states to always have ID 0
    if (b_ptr->start) {
        a_ptr-> start = true;
    }
    if (b_ptr->accept) {
        a_ptr-> accept = true;
    }

    // remove state b
    merged_a.remove_state(b_id);

    return merged_a;
}

void Automaton::remove_state(int id) {
    // for (int i=0; i<states.size(); i++) {
    //     if (states[i]->id == id) {
    //         states.erase(states.begin()+i);
    //         max_state = get_max_state();
    //         return;
    //     }
    // }
    
    // Let's try simply making a new vector
    vector<shared_ptr<State>> new_states;
    for (auto s: states) {
        if (s->id != id) {
            new_states.push_back(s);
        }
    }
    states = new_states;
    max_state = get_max_state() + 1;
}

int Automaton::get_max_state() {
    // returns the max state id value
    int max_id = 0;
    for (shared_ptr<State> s: states) {
        if (s->id > max_id) {
            max_id = s->id;
        }
    }
    return max_id;
}
Automaton Automaton::d_step() {
    // Breadth-first search for a state with multiple outgoing transitions on the same character
    // If found, return the merger of those states

    queue<shared_ptr<State>> state_list;
    // add the start state
    for (auto s: states) {
        if (s->start && state_list.size() == 0) {  // just to make sure there aren't multiple starting points
            state_list.push(s);
        }
    }

    vector<int> visited;

    // traverse and find outgoing transitions
    while (state_list.size() > 0) {
        shared_ptr<State> top = state_list.front();
        visited.push_back(top->id);

        // check if two branches share the same outgoing character
        for (shared_ptr<Transition> i: transitions_from(top->id)) {
            for (shared_ptr<Transition> j: transitions_from(top->id)) {
                // check if i->on shares any characters with j->on
                // for (int i_on: i->on) {
                //     for (int j_on: j->on) {
                //         if (i_on == j_on) {
                //             return merge_states(i->to, j->to);
                //         }
                //     }
                // }
                if (i->on == j->on && i->to != j->to) {
                    return merge_states(i->to, j->to);
                }
            }
        }

        // add branches to the queue
        for (shared_ptr<Transition> t: transitions_from(top->id)) {
            int to_id = t->to;
            if (find(visited.begin(), visited.end(), to_id) == visited.end()) {
                state_list.push(get_state(to_id));
            }
        }
        // remove top from the queue
        state_list.pop();
    }

    // if there is nothing to merge, return a copy of this automaton
    return Automaton(*this);
    
}

bool Automaton::is_equal(const Automaton &other) {
    // check if max states are equal
    // if (max_state != other.max_state) {
    //     return false;
    // }

    // check if states are equal
    vector<int> this_ids;
    for (shared_ptr<State> s: states) {
        this_ids.push_back(s->id);
    }

    vector<int> other_ids;
    for (shared_ptr<State> s: other.states) {
        other_ids.push_back(s->id);
    }

    if (this_ids != other_ids) {
        return false;
    }

    // check if transitions are the same
    vector<tuple<int, int, vector<char>>> this_trans;
    for (shared_ptr<Transition> t: transitions) {
        this_trans.push_back(make_tuple(t->from, t->to, t->on));
    }

    vector<tuple<int, int, vector<char>>> other_trans;
    for (shared_ptr<Transition> t: other.transitions) {
        other_trans.push_back(make_tuple(t->from, t->to, t->on));
    }

    if (this_trans != other_trans) {
        return false;
    }

    // otherwise, they are equal
    return true;
}

Automaton Automaton::dmerge(int a_id, int b_id) {
    // start with merging a and b, and then "fold" by repeatedly calling d_step
    // Once d_step doesn't change the automaton, return

    Automaton prev_merged = merge_states(a_id, b_id);
    Automaton d_fold = prev_merged.d_step();

    while (!prev_merged.is_equal(d_fold)) {
        prev_merged = d_fold;
        d_fold = prev_merged.d_step();
    }

    return d_fold;
}

Automaton Automaton::rpni(vector<string> neg) {
    // Create a vector of "red" states, starting with the top node
    // When a red state is added, append its child nodes to a queue of "blue" nodes (unless they're already red)
    // Breadth-firstly try to merge each blue state with each of the red states, if unsuccessful, add the blue state to the red queue

    vector<int> visited;

    vector<shared_ptr<State>> reds;
    for (shared_ptr<State> s: states) {
        if (s->start) {
            reds.push_back(s);
            visited.push_back(s->id);
            break;
        }
    }

    // initialize blue queue
    vector<shared_ptr<State>> blues;
    for (shared_ptr<Transition> t: transitions_from(reds.front()->id)) {
        blues.insert(blues.begin(), get_state(t->to));
        visited.push_back(t->to);
    }

    Automaton prev_step = Automaton(*this);

    // main loop
    while (!blues.empty()) {
        // generate dot string for graphviz
        prev_step.generate_dot();

        shared_ptr<State> top = blues.back();
        Automaton red_merge = prev_step.blue_step(top->id, reds, neg);

        if (!prev_step.is_equal(red_merge)) {
            // make this the new automaton
            // re-initialize reds, blues, and visited
            visited.clear();
            reds.clear();
            for (shared_ptr<State> s: red_merge.states) {
                if (s->start) {
                    reds.push_back(s);
                    visited.push_back(s->id);
                }
            }

            blues.clear();
            for (shared_ptr<Transition> t: red_merge.transitions_from(reds.front()->id)) {
                if (find(visited.begin(), visited.end(), t->to) == visited.end()) {
                    blues.insert(blues.begin(), red_merge.get_state(t->to));
                }
            }

            prev_step = red_merge;
        } else {
            // push the current blue state to reds
            reds.push_back(top);

            // add the children to blues
            for (shared_ptr<Transition> t: prev_step.transitions_from(top->id)) {
                if (find(visited.begin(), visited.end(), t->to) == visited.end()) {
                    blues.insert(blues.begin(), get_state(t->to));
                    visited.push_back(t->to);
                }
            }

            blues.erase(blues.end());
        }
    }

    return prev_step;
}

vector<shared_ptr<State>> Automaton::remove_state_from_rpni_colorset(vector<shared_ptr<State>> colorset, Automaton red_step) {
    vector<shared_ptr<State>> update_set;
    for (shared_ptr<State> cs: colorset) {
        for (shared_ptr<State> rs: red_step.states) {
            if (rs->id == cs->id) {
                update_set.push_back(cs);
            }
        }
    }
    return update_set;
}

Automaton Automaton::blue_step(int a_id, vector<shared_ptr<State>> reds, vector<string> neg) {
    // attempt to merge the blue state with each of the red states
    // test if the merged automaton accepts any of the negative sample
    // if no red states are accepted, return the original automaton

    for (shared_ptr<State> r: reds) {
        if (a_id == r->id) {
            continue;
        }

        Automaton merge_step = dmerge(a_id, r->id);

        // test if the merged automaton accepts anything in the negative set
        bool accept_flag = false;
        for (string s: neg) {
            if (merge_step.accept(s)) {
                accept_flag = true;
                break;
            }
        }

        // return if none of the negative sample were accepted
        if (!accept_flag) {
            return merge_step;
        }
    }

    // TODO: remove merged states from red/blue collections
    return Automaton(*this);
}