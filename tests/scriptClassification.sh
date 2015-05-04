#!/bin/zsh

zmodload -i zsh/mathfunc

##YOUR Classification progname
CLASSIFPROG=./fastRetrieval

#nb of source images to test
NBIMGTESTS=3

#nb of noisyfied copies
NBTESTS=2

for ((i=0; i < $NBIMGTESTS; i++)); do
    ##Pick a random class
    CLASSID=`expr $RANDOM % 61 + 1`
    CLASSNAME=`head -$CLASSID ../.classification.order | tail -1 | sed -e 's/,//'`
    echo "Classname: "$CLASSNAME

    ##Pick a random image in this class
    IMGID=`expr $RANDOM % 14 + 1`
    IMGNAME=../database/$CLASSNAME-$IMGID.pgm
    echo $IMGNAME

    if [[ -e $IMGNAME ]]; then
        for ((j=0; j < $NBTESTS; j+=1)); do
            ##Random scale+rotation
            ./helloworld $IMGNAME

            ##Running the retrieval
            $CLASSIFPROG test.pgm > scores_tmp.txt
            RANK=` ./getRank scores_tmp.txt $CLASSID`
            echo "Rank=$RANK"
            
            ##Number of correct results in the first 10 
            if [[ $RANK -le 10 ]]; then
               correct=$(($correct + 1))
            fi
            cpt=$(($cpt + 1))
        done
    fi
done
echo
echo "Number of 'correct' classification (first 10 classes)= "$correct
echo "Total number of tests= "$cpt
#echo "Final score= " $(( $correct / $cpt ))
rm test.pgm
#rm scores_tmp.txt
