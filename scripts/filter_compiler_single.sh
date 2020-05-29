

#REV: 8 mar 2020 need to specify a way user can make helper functs for his algos so he doesn't have to do everything inline...

#REV: go through and copy all files in /filters/ into cpp files in a function whose name is "algname", and register it at the end of that CPP file.
#REV: I never need to have a header file with a declaration...because the REGISTER takes care of it I guess?
#REV: it does need to #include <filter_registrar.hpp>

#REV: it will always make in "build/compfilters"

BUILD_DIR="build/COMPILED_FILTERS";
rm -r $BUILD_DIR
mkdir -p $BUILD_DIR

ARGSET="( localmapdict& inputmaps, std::shared_ptr<FeatMapImplCollection>& outputcollection, void*& userdata, scratchmats& scratch, const std::map<std::string,std::string>& params, const uint64_t& curr_time , const std::string& nickname, const std::string& desc )"

compfname=$BUILD_DIR"/COMPILED_FILTERS.cpp"

echo "//// START OF GENERATED FILE (this file will be overwritten!) ////" > $compfname  #REV: first overwrites
echo "#include <salmap_rv/include/salmap_rv_filter_includes.hpp>" >> $compfname
echo "using namespace salmap_rv;" >> $compfname
#echo "int salmap_rv::filter_func_dummy::DUMMY_VAR_FOR_LINKER = 42;" >> $compfname
echo "void salmap_rv::filter_func_dummy::DUMMY_FUNC_FOR_LINKER() { }" >> $compfname

#flist=$(find $1 -maxdepth 1 -type f -exec basename {} \;)
flist=$(find $1 -maxdepth 1 -type f) #REV: add constraint to only find .cpp files?
for f in $flist;
do
    
    compfnamebase=$(basename $f) #REV: includes .cpp
    #alg=${compfnamebase%.hpp}
    alg=${compfnamebase%.cpp}
    #echo $compfnamebase
    #cat $f >
    echo $f "  >>  " $compfname
    #    echo "#include <filter_registrar.hpp>" >> $compfname
    echo "void ""$alg""$ARGSET" >> $compfname
    echo "{" >> $compfname

    #REV: @sh will print actual newlines.
    #REV: @bash will keep the \n
    #echo "DEBUGPRINTF(stdout, \"Inside algo funct "$alg"\n\");" >> $compfname; #REV: debug...
    #echo "fprintf(stdout, \"Inside algo funct "$alg"\n\");" >> $compfname; #REV: debug...
    cat $f >> $compfname

    #REV: only works if output name is output lol.
    #echo "if(output->size().width <= 0 || output->size().height <=0 ) {fprintf(stderr, \"Filter [%s], output size is 0! Exiting\n\", nickname.c_str() ); exit(1); }" >> $compfname
    
    #REV: OPENCL stuff
    echo "#ifdef USE_OPENCL" >> $compfname
    echo "cv::ocl::finish();" >> $compfname
    echo "#endif" >> $compfname
    
    echo "}" >> $compfname
    echo "REGISTER_SAL_FILT_FUNC(""$alg"")" >> $compfname
done

echo "//// END OF GENERATED FILE ////" >> $compfname


#for file in *; do 
#    if [ -f "$file" ]; then 
#        echo "$file" 
#    fi 
#done
