

#------------------------------------------------------------------------
# Prompt for response, store result in Answer

Answer=""

AskQuestion() {
    Test=$1
    DefaultAns=$2
    echo -n "${1}"
    Answer="$DefaultAns"
    read Answer
}

#------------------------------------------------------------------------
# Prompt for yes or no answer - returns non-zero for no

AskYNQuestion() {
    while echo -n "${*} (y/n): "
    do
        read answer rest
        case $answer in
        [yY]*)
            return 0
            ;;
        [nN]*)
            return 1
            ;;
        *)
            echo "Please answer y or n"
            ;;
        esac
    done
}


#------------------------------------------------------------------------
# Run process and check status


runAndCheckExit() {
    Cmd=$*

#    echo $Cmd
    $Cmd

    ExitCode=$?

    if [ $ExitCode -ne 0 ]
      then
        echo "Install aborted: The command $Cmd "
        echo "                 failed with error code $ExitCode"
        exit
    fi
}

#------------------------------------------------------------------------
# Check for a previous install 


checkInstallUser() {

    if [ "`whoami`" != "root" ];
      then
        echo ""
        echo "--- Warning ----------------------------------------------"
        echo ""
        echo "    You need to be 'root' user to install"
        echo ""
        exit
    fi
}



#== Main Program ==========================================================


InteractiveInstall=1
export InteractiveInstall

checkInstallUser


# Ok, here we are installing from a CVS tree

if [ -e firebird ] 
  then
    ScriptsSrcDir=./firebird/skywalker/install

    ( $ScriptsSrcDir/super/SSpreinstall.sh )

    cp $ScriptsSrcDir/misc/firebird interbase
    cp $ScriptsSrcDir/misc/README interbase

    for i in SSchangeRunUser.sh SSrestoreRootRunUser.sh changeDBAPassword.sh
      do
        cp $ScriptsSrcDir/misc/$i interbase/bin/$i
        chmod ugo=rx interbase/bin/$i
      done


    ($ScriptsSrcDir/super/SSinstall.sh)

    ($ScriptsSrcDir/super/SSpostinstall.sh)
fi


# Here we are installing from a install tar.gz file

if [ -e scripts ]
  then
    echo "Extracting install data"
    runAndCheckExit "tar -xzf interbase.tar.gz"

    (./scripts/SSpreinstall.sh)
    (./scripts/SSinstall.sh)
    (./scripts/SSpostinstall.sh)

    rm -rf interbase
fi

echo "Install completed"

