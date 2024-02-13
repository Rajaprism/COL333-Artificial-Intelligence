#!/bin/bash

submission=$1
# if test_dir already exixts, delete it
if [ -d "./test_dir" ]; then
	rm -rf ./test_dir
fi
mkdir ./test_dir

echo $submission

# Accept both .tar.gz and .gz or any other gz ;)
fileNameRegex="lab1_[0-9]{4}[A-Z]{2}.[0-9]{4}.*.gz"

if ! [[ $submission =~ $fileNameRegex ]]; then
	# not considering zip for now
	echo "File doesn't match the naming convention"
	exit
fi

# Extract the Entry number
entryNoRegex="[0-9]{4}[A-Z]{2}.[0-9]{4}"
if [[ $submission =~ $entryNoRegex ]]; then
	entryNum=${BASH_REMATCH[0]}
	echo "Entry Number: $entryNum"
fi

echo "Setting the test directory"

tar -xzvf "$submission" -C ./test_dir
cp out* *.sh ./test_dir
cd ./test_dir

# Change the gcc binary
sed -i 's/gcc-10\|gcc-9/gcc/g' Makefile

echo "Executing the test cases"

pkill qemu-system-x86
pkill qemu-system-i386
make clean

make xv6.img
#make

echo "Running..0"
timeout 30s ./test_lab1.sh 1 | grep -E "LEFT|MID|RIGHT" | sed 's/(qemu) //g'|sort > res_lab1_0

echo "Running..1"
timeout 30s ./test_lab1.sh 2 | grep -E "LEFT|MID|RIGHT" | sed 's/(qemu) //g'|sort > res_lab1_1

echo "Running..2"
timeout 30s ./test_lab1.sh 4 | grep -E "LEFT|MID|RIGHT" | sed 's/(qemu) //g'|sort > res_lab1_2

check_test=3
total_test=0

echo "" > .output
marks="$entryNum"
for ((t=0;t<$check_test;++t))
do
	echo -n "Test #${t}: "

	# NOTE: we are doing case insensitive matching.  If this is not what you want,
	# just remove the "-i" flag
	# if diff -iZwB to -iwB
	if diff -iwB <(sort out_lab1_$t) <(sort res_lab1_$t) > /dev/null
	then
		echo -e "\e[0;32mPASS\e[0m"
		marks="$marks,1" # 1 mark for pass
		((total_test++))
	else
		echo -e "\e[0;31mFAIL\e[0m"
		marks="$marks,0" # 0 mark for fail
		echo "Output for test case $t:" >> .output
		cat res_lab1_$t >> .output
	fi
done
echo "$total_test test cases passed for $entryNum"
echo $marks >> ../marks.csv
