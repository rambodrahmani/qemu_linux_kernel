#*******************************************************************************
# file delay.s

.text

.global          delay
delay:           int   $240               # sottoprogramma di interfaccia
                 ret
#*******************************************************************************
