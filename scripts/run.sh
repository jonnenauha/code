files='libGraphicsUtility qhull3.0 libRAPID SwiftppDecomposer libSwiftppDecomposer libCollisionEnvironment libSwiftpp libComputationalGeometry libPQPManager libProximityManager libSwiftppManager PQP'

for f in $files
do 
    name=$f'_a'

    rm -f $f/Makefile*
    cp -f Makefile.am.tmpl $f/Makefile.am

    pushd $f

    echo 'lib_LIBRARIES = '$f'.a' >> Makefile.am
    echo $name'_CPPFLAGS = -Iinclude' >> Makefile.am
    sourcify.sh $name src/* >> Makefile.am

    popd
done
