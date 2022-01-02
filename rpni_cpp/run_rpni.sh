#!/usr/bin/env bash

# compile the program
g++ *.cpp -g -o rpni

# run the code and create a temp file of DOT strings
PREF=../nl_data
TRAIN_N=250
DEV_N=250
LANG=nci
head -n $TRAIN_N $PREF/${LANG}_cv.txt > ${LANG}_pos_train.txt
head -n $TRAIN_N $PREF/${LANG}_cv_neg.txt > ${LANG}_neg_train.txt
tail -n $DEV_N $PREF/${LANG}_cv.txt > ${LANG}_pos_dev.txt
tail -n $DEV_N $PREF/${LANG}_cv_neg.txt > ${LANG}_neg_dev.txt
./rpni ${LANG}_pos_train.txt ${LANG}_neg_train.txt ${LANG}_pos_dev.txt ${LANG}_neg_dev.txt | uniq > dot_file.txt

# generate SVG files
if [ -d viz ]
then
  cd viz
  rm *.svg
  cd ..
else
  mkdir viz
fi

i=0
while read l; do
  if [[ $l =~ Dev.* ]]
  then
    echo $l
  else
    echo $l | dot -Tsvg > viz/${i}.svg
    i=$((i+1));
  fi
done < dot_file.txt

rm *.txt