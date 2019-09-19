# EXTENSION 2016-07-06

#-------------------------------------------------------------------------------
# Definition of the ceread primitive.
# Just calls the IO_TIPO_CEREAD interrupt and returns.
# The IO_TIPO_CEREAD interrupt is handled in io/io.s/a_ceread ->
# io/io.cpp/c_ceread() 
#-------------------------------------------------------------------------------
.GLOBAL ceread
#-------------------------------------------------------------------------------
ceread:
	int $IO_TIPO_CEREAD
	ret

# EXTENSION 2016-07-06

