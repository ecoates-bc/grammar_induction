import typing
import re
import numpy as np
from random import seed, shuffle, sample
from nltk import ngrams
from collections import Counter
from sklearn.decomposition import TruncatedSVD

"""
Spectral learning of the language divisible by n
    - 1st step: get 10,000 strings representing decimal numbers, separate them into a positive and negative sample
    - Construct a Hankel basis: find the top-k most frequent substrings up to length 5
        - f_s(ps): sum of, for each string, the number of times prefix-suffix pair ps appears in that string, times the probability of the string
            - since each string appears only once, the number of times ps appears * 1/10000
    - Perform SVD, and get <a_tilde_1, a_tilde_inf, A_sigma>
    - Use Lemma 1 to get <a_1, a_inf, A_sigma>
"""


class IntDataset:
    # generate a number of strings and create two labels: the strings representing numbers divisible by n, 
    #   and those that are aren't. Split into train/dev sets.
    def __init__(self, num_strings: int, n: int, pct_dev: float, to_shuffle: bool) -> None:
        # keep track of the alphabet
        self.alphabet = []

        # get data/label pairs
        full_data = []
        for i in range(num_strings):
            entry = {'data':str(i)}
            if i % n == 0:
                entry['label'] = 1
            else:
                entry['label'] = 0
            full_data.append(entry)

            # update alphabet
            # self.alphabet.update(set(str(i)))
            for c in str(i):
                if c not in self.alphabet:
                    self.alphabet.append(c)

        if to_shuffle:
            shuffle(full_data)

            # create train/dev split. We can take the last part of the dataset as the dev set.
            n_dev = int(num_strings * pct_dev)
            self.train = full_data[:num_strings-n_dev]
            self.dev = full_data[num_strings-n_dev:]
        else:
            # get a random sample along the full data, if we're not shuffling
            n_dev = int(num_strings * pct_dev)
            self.dev = sample(full_data, k=n_dev)
            self.dev = sorted(self.dev, key=lambda ex: int(ex['data']))
            self.train = [ex for ex in full_data if ex not in self.dev]

    # get the expectation of finding the input string as a substring
    def get_subseq_e(self, subs: str) -> float:
        total_expect = 0
        for ex in self.train:
            n_matches = len(re.findall(subs, ex['data']))
            if n_matches > 0 and ex['label'] == 1:
                # scale by the inverse magnitude of the training dataset
                total_expect += n_matches / len(self.train)

        return total_expect

    # get the top k most frequent substrings in the dataset
    def top_k(self, k: int) -> list:
        top_dict = Counter()
        for ex in self.train:
            if ex['label'] == 1:
                for i in range(1, 6):
                    substrs = [''.join(j) for j in ngrams(list(ex['data']), n=i)]
                    top_dict.update(substrs)

        assert len(top_dict) >= k
        return [t[0] for t in top_dict.most_common(k)]  # just want the substrings, not their frequencies too


class CVDataset:
    # Consonant/Vowel sequences in NL data
    def __init__(self, pos_path: str, neg_path:str, n_tot: int, pct_dev: float) -> None:
        # keep track of the alphabet
        self.alphabet = []

        # get data/label pairs
        raw_pos_data = [line.rstrip() for line in open(pos_path, "r").readlines()]
        raw_neg_data = [line.rstrip() for line in open(neg_path, "r").readlines()]

        n_each = n_tot // 2

        full_data = []
        for ex in raw_pos_data[:n_each]:
            entry = {'data':ex}
            entry['label'] = 1
            full_data.append(entry)

            # update alphabet
            # self.alphabet.update(set(str(i)))
            for c in ex:
                if c not in self.alphabet:
                    self.alphabet.append(c)

        for ex in raw_neg_data[:n_each]:
            entry = {'data':ex}
            entry['label'] = 0
            full_data.append(entry)

        
        # get a random sample along the full data, if we're not shuffling
        n_dev = int(n_tot * pct_dev)
        self.dev = sample(full_data, k=n_dev)
        self.train = [ex for ex in full_data if ex not in self.dev]

    # get the expectation of finding the input string as a substring
    def get_subseq_e(self, subs: str) -> float:
        total_expect = 0
        for ex in self.train:
            n_matches = len(re.findall(subs, ex['data']))
            if n_matches > 0 and ex['label'] == 1:
                # scale by the inverse magnitude of the training dataset
                total_expect += n_matches / len(self.train)

        return total_expect

    # get the top k most frequent substrings in the dataset
    def top_k(self, k: int) -> list:
        top_dict = Counter()
        for ex in self.train:
            if ex['label'] == 1:
                for i in range(1, 5):
                    substrs = [''.join(j) for j in ngrams(list(ex['data']), n=i)]
                    top_dict.update(substrs)

        assert len(top_dict) >= k
        return [t[0] for t in top_dict.most_common(k)]  # just want the substrings, not their frequencies too


class Automaton:
    def __init__(self, a_1: np.ndarray, a_inf: np.ndarray, A: list, alphabet: list) -> None:
        self.init_weights = a_1
        self.final_weights = a_inf
        self.transitions = A
        self.alphabet = alphabet
        self.n_states = a_1.shape[0]

    def get_char_mat(self, c: str) -> np.ndarray:
        return self.transitions[self.alphabet.index(c)]

    def get_string_weight(self, s: str) -> float:
        tot_weight = self.init_weights
        for c in list(s):
            tot_weight = tot_weight @ self.get_char_mat(c)
        tot_weight = tot_weight @ self.final_weights
        return tot_weight


class SpectralLearner:
    def __init__(self, dset: IntDataset) -> None:
        self.dset = dset

    def get_hankel_basis(self, k: int) -> np.ndarray:
        # the prefixes/suffixes will be the top_k expected values, plus the empty string
        span = self.dset.top_k(k)
        span.insert(0, '')

        h_mat = np.zeros((k, k))
        for i in range(k):
            for j in range(k):
                substr = span[i] + span[j]
                if substr:
                    h_mat[i, j] = self.dset.get_subseq_e(substr)

        return h_mat

    def get_prefix_closed_basis(self, p: str, H_l: np.ndarray) -> np.ndarray:
        # 'turn off' rows of H_l where p does not appear as a prefix
        take_rows = [True if re.search('^' + p, pref) else False for pref in self.dset.top_k(H_l.shape[0])]
        sub_mat = H_l.copy()
        for i in range(len(take_rows)):
            if not take_rows[i]:
                sub_mat[i,...] = 0
        return sub_mat

    def train(self, k: int, n: int) -> Automaton:
        # learn an automaton from a SVD of the Hankel basis
        # k: top-k, n: number of states, assumed rank of H
        # since we're learning f_s from a top k basis, also re-write the learned automaton to represent f
        basis = self.get_hankel_basis(k)

        # get V from a truncated SVD
        tsvd = TruncatedSVD(n_components=n, algorithm='arpack')
        tsvd.fit(basis)
        V = tsvd.components_.T

        # get h_lambda (since we're using the top k basis, we can assume h_p == h_s)
        h_lambda = basis[...,0]

        # print('Hankel basis')
        # print(basis)
        # print()
        # print('h_lambda')
        # print(h_lambda)

        # get a_tilde_1, a_tilde_inf
        squiggle_a1 = h_lambda @ V
        HV_pinv = np.linalg.pinv(basis @ V)
        squiggle_ainf = HV_pinv @ h_lambda

        # print('Pseudo-inverse')
        # print(HV_pinv)

        # get A_sigma for sigma in dset.alphabet
        transitions = []
        for c in self.dset.alphabet:
            H_sig = self.get_prefix_closed_basis(c, basis)
            A_sig = HV_pinv @ H_sig @ V
            # print(A_sig)
            # print()
            transitions.append(A_sig)

        # get the actual values a_1 and a_inf
        # they are multiplied by (I - A)^-1, so we multiply by (I - A) (a.k.a divide by the inverse)
        A = np.zeros_like(transitions[0])
        for Asig in transitions:
            A = np.add(A, Asig)

        a_1 = (np.eye(n) - A) @ squiggle_a1   # tilde_a_1 (I-A) = a1 (I - A)^-1(I_A)
        a_inf = (np.eye(n) - A) @ squiggle_ainf   # (I-A)tilde_a_inf = (I-A)(I -A)^1 a_inf

        # we now have a learned automaton!
        return Automaton(a_1, a_inf, transitions, self.dset.alphabet)


if __name__ == "__main__":
    # Add <s> </s> symbols
    seed(0)
    np.random.seed(0)
    data = CVDataset('../nl_data/nav_cv.txt', '../nl_data/nav_cv_neg.txt', 200, 0.5)
    model = SpectralLearner(data)
    wfa = model.train(20, 5)

    # for i in range(230, 240):
    #     print(str(i), '\t', wfa.get_string_weight(str(i)))
    
    # classification accuracy
    n_correct = 0
    dset = data.dev
    for ex in dset:
        pred = int(wfa.get_string_weight(ex['data']) > 0)
        if pred == ex['label']:
            n_correct += 1

    print('Classification accuracy: {:.3f}'.format(n_correct / len(dset)))