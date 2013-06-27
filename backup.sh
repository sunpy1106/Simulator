#!/bin/bash

filename=$1
DATE=`date | sed 's/\(.*\) \(.*\):\(.*\):\(.*\) \(.*\) \(.*\)/\2\3\4/g'`

mv $filename ${filename}_${DATE}
