# Code for LING 448: Grammar Induction

## Overview
This repository contains the coding projects I worked on for LING 448.
The first is an implementation of two proofs for PAC-learning boolean algebra
concepts, and the last two are implementations of regular language-learning algorithms.

## Coding project: consonant/vowel patterns
Every language has its own set of allowed/impossible vowel/consonant sequences.
For example, in english we don't have more than three consonants in sequence (i.e. "strengths" has the pattern CCCVCCC).
Russian, however, allows longer sequences: vzgliat' (to gaze) is CCCCVVC.

It's possible to learn a grammar that represents a language's accepted vowel/consonant sequences,
using a pronunciation dataset.

### Language data
I converted IPA pronunciation data from WikiPron into sequences of the symbols C and V, using
the library PanPhon to generate phonological feature data. The languages I chose were Modern Greek,
Russian, and Navajo. (I attempted Japanese, English, and Nauhatl too, but ran into some convergence issues later.)

#### Negative data
Both algorithms I implemented use negative data in order to converge and/or calculate classification accuracy.
To generate impossible consonant/vowel sequences, I first found the longest consecutive sequence of vowels, and then consonants.
Then, I generated a new "negative" dataset where each example has "too many" vowels or consonants in sequence, chosen randomly.
For example, if a language doesn't allow CCCC or VVV sequences, and and the positive example is CVC, the negative example would either
be CCCCVCCCC or CVVVVC.

### RPNI
The algorithm "Regular Positive and Negative Inference" is a "state-merging" approach to learning non-probabilistic regular languages.
It's a graph-traversal algorithm that I implemented in c++ - the code is found in the folder `rpni_cpp.`
The script `run_rpni.sh` compiles the code with g++, passes data to the output executable,
and visualizes each step of the RPNI process, using graphviz. The folder `learned_automata` has the final automata generated
by RPNI along with a csv file of dev accuracies.

#### Results
It appears that my RPNI implementation can learn some regular languages that are more complicated than "toy" examples like `a+`.
The machines learned by this implementation have a "core" section of a few states each that looks like the "canonical" language,
and a couple "outer rings" of states that represent outlier sequences. It learned machines with dev accuracies of 90%-100%, but the running
time becomes very long once the training dataset size gets much larger than 1000.

A few languages didn't converge, and I believe it's because of a bug where a transition from both C and V and one on just C don't get recognized as "brothers".

### Spectral learning
I also attempted a spectral learning algorithm for learning weighted finite automata from the consonant/vowel data. It uses
a matrix of subsequence frequencies to learn a matrix representation of an automata for the language. The file `spectral/results.csv`
contains the dev accuracy results for Greek, Russian, and Navajo.

#### Results
From the dev-set classification accuracies of the above three languages, it looks like my specral learning model is able to learn
these languages, although less successfully than RPNI. Since this is a weighted automata approach, I had issues with numerical stability. In addition,
the hyperparameters k and n were very sensitive, and I wasn't able to determine a pattern between them, or between them and the dev accuracy.

### Final thoughts
Grammar Induction is a field of very difficult problems, with lots of not completely satisfying solutions. So, I'm not surprised that my algorithm implementations
weren't that robust, and had convergence issues. It looks like I was on the right track with both of them, however, because some learning
was being displayed. Both of these algorithms were quite space/time/data-efficient, so I think a more robust implementation of either would make
very powerful classification models.