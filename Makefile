
.DEFAULT_GOAL := run
.PRECIOUS : out/%.lsl

GCC := gcc -x c -E -P -DLSL_C
SED := sed -E 's/_START "([^"]*)" _END/"\1"/g; s/_START ([0-9\.]+) _END/"\1"/g; s/_START ([^"]*) _END/"" + (string)(\1) + ""/g; s/" ~ "//g'

.PHONY : run
run : out/$(target).lsl

.PHONY : clean
clean : 
	@rm -rf out

out/%.lsl :
	@mkdir -p out
	@$(GCC) $< | $(SED) > $@
