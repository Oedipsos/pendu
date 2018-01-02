
BUILD:=		pendu
OBJS:=		main.o dico.o

$(BUILD):	$(OBJS)
		gcc -o $(BUILD) $(OBJS)

include .depends

.c.o:
		gcc -c -o $@ $<

dep .depends:
		gcc -M *.c > .depends

test:		dico.c dico.h
		gcc -D"TESTMODE" dico.c -o test
		
clean:
		rm -f a.out pendu test $(OBJS) *~

distclean:	
		rm -f a.out pendu test .stats $(OBJS) *~
