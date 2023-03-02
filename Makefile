
.DEFAULT_GOAL = run
.PHONY: run

run : lsl/$(target).c
	@mkdir -p out
	@gcc -x c -E -P -DLSL=on lsl/$(target).c | sed -E 's/" ~ "//g' > out/$(target).lsl

.PHONY : garble
garble :
	@python garble.py

.PHONY : clean
clean : 
	@rm -rf out