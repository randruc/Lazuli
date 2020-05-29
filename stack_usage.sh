#
# Simple bash one liner to display the stack usage of the whole project nicely
# presented and sorted.
#

#!/bin/bash

find . -name *.su | xargs cat | column -td | sort -rn -k2 | less
