#/bin/bash
DIRECTORY=$( cd "$( dirname $0 )" && pwd )
javaws -headless $DIRECTORY/ContestAppletProd.jnlp
