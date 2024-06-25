
.DEFAULT_GOAL := run

GCC := gcc -x c -E -P -DLSL_C
SED := sed -E 's/_START "([^"]*)" _END/"\1"/g; s/_START ([0-9\.]+) _END/"\1"/g; s/_START ([^"]*) _END/"" + (string)(\1) + ""/g; s/" ~ "//g'

.PHONY : run
run :
	@mkdir -p out
	@$(GCC) projects/$(target).cpp | $(SED) > out/$(target).lsl

.PHONY : test
test :
	@mkdir -p out
	@$(GCC) misc/test.h | $(SED) > out/test.lsl
	@python misc/test.py

.PHONY : clean
clean : 
	@rm -rf out
