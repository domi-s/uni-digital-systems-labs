all: thumbsim

thumbsim thumbsim2: %: %.c
	gcc -Wall -g -O2 $< -o $@

thumbsim.c: thumbsim.w
	ctangle $<

thumbsim2.c: thumbsim.w extra.ch
	ctangle $^ $@

%.pdf: %.tex
	pdftex $<
	pdftk $@ cat end 1-r2 output tmp.pdf
	mv tmp.pdf $@

%.tex: %.w
	cweave $<

TESTS = $(patsubst %.s,%,$(sort $(wildcard test*[0-9].s)))

test: $(TESTS)
	@echo "*** Passed"

test%: thumbsim force
	arm-none-eabi-as -march=armv6 -mthumb test$*.s -o a.o
	arm-none-eabi-ld -T thumb.ld a.o -o a.out
	arm-none-eabi-objcopy -O binary a.out a.bin
	arm-none-eabi-objdump -d a.out
	$(SIM) a.bin $$(sed -n '/^@args *\(.*\)/s//\1/p' test$*.s) \
		| tee a.res
	sed -n '/^@result \(.*\)/s//\1/p' test$*.s >a.exp
	sed -n '/^exit \(.*\)/s//\1/p' a.res | diff a.exp -

clean: force
	rm -f *.o *.bin thumbsim.idx thumbsim.log thumbsim.pdf \
		thumbsim.scn thumbsim.tex thumbsim.toc \
		thumbsim.c thumbsim a.exp a.out a.res \
		thumbsim2.c thumbsim2

force:

SIM = thumbsim
