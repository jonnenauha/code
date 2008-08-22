# hack script to take feature files and output something for gnuplot
# ryanm -- 200604

import os
import re

# directory where the feat*.txt files are stored
dir = 'klt/';

# check for only feature files
def is_feature_file(filename):
    if (re.match('feat[0-9]*\.txt', f) != None):
	return True
    else: 
	return False

# compare function for our text+number filename
def fncmp(a, b):
    s = 'feat.txt'
    return int(a.strip(s)) - int(b.strip(s))

# get the feature file names and sort by frame number
files = os.listdir(dir)
featurefiles = [f for f in files if is_feature_file(f)]
featurefiles.sort(fncmp)

nframes = len(featurefiles)
nfeatures = 0
dtable = {}

# read in all the lines and put the 4th (value) column in a lookup table
for n, fname in enumerate(featurefiles):
    fobj = open(dir+fname, 'r')
    lines = fobj.readlines()
    fobj.close()
	
    if len(lines) > nfeatures:
	nfeatures = len(lines)

    dtable[n] = {}

    for p, l in enumerate(lines):
    	col = l.split()
	dtable[n][p] = col[3]

# put in a data file where:
# one column listing each frame
# and one column for each point on that frame
datafilename = "points.dat"
datafile = open(datafilename, 'w')

for f in range(nframes):
    for p in range(nfeatures):
	value = int(dtable[f][p])
	datafile.write(repr(f) + ' ' + repr(value) + ' ')
    datafile.write('\n')

datafile.close()

# generate a corresp gnuplot file
gnuplotfile = open("plotdifferences.gp", 'w')
gnuplotfile.write('set title "Difference measure of features over successive frames"\nset terminal png\nset style data lines\nunset key\nplot ')

for p in range(nfeatures):
    gnuplotfile.write('"' + datafilename + '"  using 1:' + repr(p+2) + ', ')

# replace trailing comma with a newline
gnuplotfile.seek(-2,2)
gnuplotfile.write('\n')
gnuplotfile.close()

# generate a gnuplot file for the feature eigenvalues
gnuplotfile = open("ploteigenvalues.gp", 'w')
gnuplotfile.write('set title "Eigenvalues of N selected features"\nset terminal png\nset style data lines\nunset key\n')
gnuplotfile.write('plot "' + dir + 'featurelist.txt" using 1:4')
gnuplotfile.close()

# run the gnuplot files in the shell to output the png files
os.system('gnuplot plotdifferences.gp > plotdifferences.png')
os.system('gnuplot ploteigenvalues.gp > ploteigenvalues.png')
