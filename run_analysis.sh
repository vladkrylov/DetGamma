#!/bin/bash   

PLOTSONLY=false ;
if [[ $PLOTSONLY == "true" ]] ; then 
    echo "================================================================================";
    echo "RUN PLOTS PRODUCTION ONLY!" ;
    echo "================================================================================";
fi

LENGTH=15;

#for DETECTOR in box cylinder ; do 
for DETECTOR in box ; do 
    #    for CATHODE in standard photonis ; do
    for CATHODE in standard ; do
	
	if [[ $DETECTOR == "box" && $CATHODE == "photonis" ]] ; then continue; fi

	for ADDSECONDPM in false; do 

#	for WITHWINDOW in true false ; do 
	    for WITHWINDOW in true ; do 

#	    for COATING in black white diffuse_white black_and_white black_and_diffuse_white; do 
		for COATING in white; do 
		
		if [[ $DETECTOR == "box" && $ADDSECONDPM == "true" ]] ; then continue; fi
		if [[ $CATHODE == "standard" && $ADDSECONDPM == "true" ]] ; then continue; fi
		if [[ $CATHODE == "standard" && $COATING == "black_and_white" ]] ; then continue; fi
		if [[ $ADDSECONDPM == "true" &&  $COATING == "black_and_white" ]] ; then continue; fi
		if [[ $DETECTOR == "box" && $COATING == "black_and_white" ]] ; then continue; fi
		if [[ $CATHODE == "standard" && $COATING == "black_and_diffuse_white" ]] ; then continue; fi
		if [[ $ADDSECONDPM == "true" &&  $COATING == "black_and_diffuse_white" ]] ; then continue; fi
		if [[ $DETECTOR == "box" && $COATING == "black_and_diffuse_white" ]] ; then continue; fi

		DIR="det_"$DETECTOR"_"$CATHODE"_"$COATING ;
		case $ADDSECONDPM  in 
		    (true) DIR=$DIR"_secondpm";;
		esac

		case $WITHWINDOW  in 
		    (false) DIR=$DIR"_nowindow";;
		esac

		if [[ $PLOTSONLY == "false" ]] ; then 
		    
		    if ( [ ! -d  $DIR ] ) then echo "Create directory "$DIR ; mkdir $DIR ; fi
		    rm -rf $DIR/*
		    
		    echo "#DIRECTORY $DIR" > $DIR/det_config.mac ;
		    
		    echo "/DetGamma/detector/CrystalLength $LENGTH" >> $DIR/det_config.mac ;
		    echo "/DetGamma/detector/CrystalShape $DETECTOR" >> $DIR/det_config.mac ;
		    echo "/DetGamma/detector/Coating $COATING" >> $DIR/det_config.mac ;
		    
		    case $CATHODE in 		
			(standard) echo "/DetGamma/detector/UsePhotonisQE false" >>  $DIR/det_config.mac ;;
			(photonis) echo "/DetGamma/detector/UsePhotonisQE true" >>  $DIR/det_config.mac ;;
			(*) echo "ERROR 1"; exit ;;
		    esac
		    
		    case $DETECTOR in 
			(box)  
			    echo "/DetGamma/detector/AddGrease true" >>  $DIR/det_config.mac ;;
#			    echo "/DetGamma/detector/GreaseName OCF463" >>  $DIR/det_config.mac ;;
			(*) echo "/DetGamma/detector/AddGrease false">>  $DIR/det_config.mac ;;
		    esac

		    
		    case $ADDSECONDPM  in 
			(true) echo "/DetGamma/detector/AddSecondPM true" >>  $DIR/det_config.mac ;;
		    esac
		    
		    case $WITHWINDOW  in 
			(false) echo "/DetGamma/detector/WithWindow false" >>  $DIR/det_config.mac ;;
			(*) echo "/DetGamma/detector/WithWindow true" >>  $DIR/det_config.mac ;;
		    esac
	    
		    echo "/DetGamma/output/RootFileName $DIR/DetGamma.root" >>  $DIR/det_config.mac ;
		    echo "/DetGamma/output/Library /home/rita/geant4_workdir/tmp/Linux-g++/mainDetGamma/libDetGammaClassesDict.so" >> $DIR/det_config.mac ;

		    mainDetGamma $DIR/det_config.mac run2.mac ;

		fi

		makePlots  -input_file $DIR/DetGamma.root  -out_dir $DIR > $DIR/log

	    done
	    done
	done
    done
done
