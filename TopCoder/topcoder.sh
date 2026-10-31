#/bin/bash
DIRECTORY=$( cd "$( dirname $0 )" && pwd )
javaws -nosecurity $DIRECTORY/ContestAppletProd7.1.4.jnlp
