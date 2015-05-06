#!/bin/zsh

# Our distance program
SIMILARYPROG=./fastDistance

# Number of source images to test
NBIMGTESTS=3

# Number of noisyfied copies

NBTESTS=2
MAXNOISE=0.5
sum=0.0
variance=0.0
cpt=0.0

for ((i=0; i < $NBIMGTESTS; i++)); do
    # Pick a random class
    CLASSID=`expr $RANDOM % 61 + 1`
    CLASSNAME=`head -$CLASSID ../.classification.order | tail -1 | sed -e 's/,//'`
    echo "Classname: "$CLASSNAME

    # Pick a random image in this class
    IMGID=`expr $RANDOM % 14 + 1`
    IMGNAME=../database/$CLASSNAME-$IMGID.pgm
    echo $IMGNAME

    if [[ -e $IMGNAME ]]; then
        for ((j=0; j < $NBTESTS; j+=1)); do
            # Random scale + rotation + noise
            ./addNoise $IMGNAME $MAXNOISE

            DIST=`$SIMILARYPROG $IMGNAME temp.pgm`
            sum=$(( $sum + $DIST)) # No float operations in bash!
            variance=$(( $variance + $DIST*$DIST ))
            echo "Similarity= "$DIST
            cpt=$(($cpt + 1.0))
        done
    fi
done
echo
echo "Average similarity= " $(( $sum / $cpt ))
echo "Variance= " $(( $variance / $cpt - ($sum / $cpt)*($sum / $cpt)  ))
rm test.pgm
