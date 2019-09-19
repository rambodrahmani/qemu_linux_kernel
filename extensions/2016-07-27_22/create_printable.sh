#!/bin/sh

################################################################################
# File: create_printable.sh
#       Creates printable PDF version of the given source files.
#
# Usage: ./create_printable.sh
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 19/09/2019
################################################################################

# create .ps files and convert them to .pdf
for entry in "$1"/*
do
    enscript -p $entry".ps" $entry
    ps2pdf $entry".ps" $entry".pdf"
done

# create subdirectory for .ps and .pdf files
cd "printable"
mkdir "pdf"
mkdir "ps"

# move .ps files into ps subdirectory
mv *.pdf pdf/.

# move .pdf files into pdf subdirectory
mv *.ps ps/.

