


# Dummy to compile filters first
# Need filters first otherwise the other makefile OBJS variable will not contain all the needed build targets (since the .cpp files don't exist yet).
$(MAKECMDGOALS): SCRIPTCOMPILE
	make -j4 -f Makefile2 $(MAKECMDGOALS)

SCRIPTCOMPILE:
#	@sh scripts/filter_compiler.sh filter_stubs
	@bash scripts/filter_compiler_single.sh filter_stubs

.PHONY: SCRIPTCOMPILE
