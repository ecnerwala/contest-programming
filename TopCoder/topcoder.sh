#/bin/bash
DIRECTORY=$( cd "$( dirname $0 )" && pwd )
javaws $DIRECTORY/ContestAppletProd.jnlp
