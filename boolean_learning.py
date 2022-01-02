import random
import string
from collections import OrderedDict
from sympy import *
import re
from itertools import product, combinations


class Oracle:
    def __init__(self, expression):
        self.n_literals = 0
        self.expression = self.string_2_expression(expression)
        self.truth_table = self.generate_truth_table()

    def string_2_expression(self, exp):
        # convert a string to a sympy expression
        literals = OrderedDict.fromkeys(re.sub('[|~&() ]', '', exp)).keys()
        self.n_literals = len(literals)
        print('Literals:', ', '.join(literals))
        symbol_list = symbols(','.join(literals))

        return Lambda(symbol_list, re.sub(' ', '', exp))

    def oracle(self, bvect):
        possible = get_possible_implicants(bvect)
        results = [self.expression(*p) for p in possible]
        return False not in results

    def generate_truth_table(self):
        ttable_builder = tuple([[0, 1] for i in range(self.n_literals)])
        inputs = [p for p in product(*ttable_builder)]
        evaluated = {0: [], 1: []}
        for i in inputs:
            if self.oracle(i):
                evaluated[1].append(i)
            else:
                evaluated[0].append(i)
        return evaluated

    def examples(self):
        return random.choice(self.truth_table[1])

    def check_equivalence(self, g_lambda):
        ttable_builder = tuple([[0,1] for i in range(self.n_literals)])
        inputs = [p for p in product(*ttable_builder)]

        for i in inputs:
            if self.oracle(i) != g_lambda(*i):
                return False
        return True


def get_possible_implicants(v):
    conv = ()
    for i in range(len(v)):
        if v[i] == "*":
            conv += ([0, 1],)
        else:
            conv += ([v[i]],)
    return [p for p in product(*conv)]


class Learner:
    def __init__(self, oracle, exp_type):
        self.oracle = oracle
        self.n_literals = len(self.oracle.examples())
        if exp_type == "cnf":
            self.expression = self.init_cnf_expression()
        elif exp_type == "dnf":
            self.expression = self.init_dnf_expression()

    def init_dnf_expression(self):
        literals = tuple([string.ascii_lowercase[i] for i in range(self.n_literals)])
        print('Learner literals:', ', '.join(list(literals)))
        return literals, 'false'

    def init_cnf_expression(self):
        literals = tuple([string.ascii_lowercase[i] for i in range(self.n_literals)])
        possible_combinations = [p for p in literals] + ['~'+p for p in literals]

        all_monomials = []
        for i in range(1,self.n_literals+1):
            prod = ['|'.join(c) for c in combinations(possible_combinations, i)]
            for p in prod:
                all_monomials.append('({})'.format(p))

        return literals, '&'.join(all_monomials)

    def check_double_arrow(self, v, g_literals, g_expression):
        g = Lambda(g_literals, g_expression)
        return g(*v)

    def learn_dnf(self, l):
        for i in range(l):
            ex = self.oracle.examples()

            if not self.check_double_arrow(ex, self.expression[0], self.expression[1]):
                input_prod = []
                for p in range(self.n_literals):
                    if ex[p]:
                        input_prod.append(self.expression[0][p])
                    else:
                        input_prod.append('~' + self.expression[0][p])

                new_m = '&'.join(input_prod)
                print('After example {}, added monomial {}'.format(i+1, new_m))
                self.expression = (self.expression[0], self.expression[1] + '|({})'.format(new_m))

    def learn_cnf(self, l):
        for i in range(l):
            changed = False
            ex = self.oracle.examples()

            monomials = self.expression[1].split('&')
            for m in monomials:
                if not self.check_double_arrow(ex, self.expression[0], m):
                    monomials.remove(m)
                    changed = True
            if changed:
                print('After example {}, there are now {} monomials.'.format(i+1,len(monomials)))
                print('New expression:', self.expression[1])
            self.expression = (self.expression[0], '&'.join(monomials))

    def get_lambda(self):
        return Lambda(self.expression[0], self.expression[1])


orc = Oracle('(p|q)&(r|~r)')
learner = Learner(orc, 'cnf')
learner.learn_cnf(25)
print(orc.check_equivalence(learner.get_lambda()))