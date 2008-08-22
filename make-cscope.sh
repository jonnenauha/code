PREFIX=${HOME}/Code
OUTDIR=${PREFIX}/tags

DIRS="opensim"
FILES="cs cpp hpp h"

outfile=${OUTDIR}/cscope.files
outscope=${OUTDIR}/cscope.out

rm -f ${outfile}
cd ~ 
for d in ${DIRS}
do
    dir=${PREFIX}/${d} 

    for f in ${FILES}
    do
        find ${dir} -name *.${f} >> ${outfile}
    done
done

rm -f ${outscope}
pushd ${OUTDIR} 
cscope -b 
popd
