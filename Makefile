
.DEFAULT_GOAL := run

GCC := gcc -x c -E -P -DLSL_C
SED := sed -E -f convert.sed

# detect shell type: Windows CMD vs. Bash-like
ifeq ($(OS),Windows_NT)
	MKDIR_P = if not exist out mkdir out
	RM_RF = if exist out rmdir /S /Q out
else
	MKDIR_P = mkdir -p out
	RM_RF = rm -rf out
endif

.PHONY : run
run :
	@$(MKDIR_P)
	@$(GCC) projects/$(target).cpp | $(SED) > out/$(target).lsl

.PHONY : test
test :
	@$(MKDIR_P)
	@$(GCC) misc/test.h | $(SED) > out/test.lsl
	@python misc/test.py

.PHONY : clean
clean :
	@$(RM_RF)
