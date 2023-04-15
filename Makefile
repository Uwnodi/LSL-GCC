
.DEFAULT_GOAL := run
.PRECIOUS : out/%.lsl

GCC := gcc -x c -E -P -DLSL_C
SED := sed -E 's/_START "(.*)" _END/"\1"/g; s/_START ([0-9]+) _END/"\1"/g; s/_START (.*) _END/"" + (string)(\1) + ""/g; s/" ~ "//g'

.PHONY : run
run : out/$(target).lsl

.PHONY : test
test : out/test.lsl lsl/test.lsl
	@diff --strip-trailing-cr lsl/test.lsl out/test.lsl >> /dev/null || ( \
		printf "\n=== Tests Failed! ===\n\n" && \
		diff --strip-trailing-cr -u lsl/test.lsl out/test.lsl)
	@printf "\n=== Tests Passed! ===\n"

.PHONY : garble
garble :
	@python garble.py

.PHONY : clean
clean : 
	@rm -rf out

out/%.lsl : lsl/%.cpp
	@mkdir -p out
	$(GCC) $< | $(SED) > $@
