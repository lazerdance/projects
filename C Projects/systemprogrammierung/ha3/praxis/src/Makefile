src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

IDIR    =   ./../include/
ODIR    =   ./../obj/
DEP    =   ./../dep/

LDFLAGS = -lm
CFLAGS  =   -I$(IDIR) -Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c -o $(addprefix $(ODIR), $@) $<

scheduler: $(obj)
	$(CC) -o $@ $(addprefix $(ODIR), $^) $(LDFLAGS)

-include $(dep)

%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(src) > $(addprefix $(DEP), $*.d)

.PHONY: clean

clean:
	rm -f $(addprefix $(ODIR), $(obj)) scheduler
	rm -f $(dep) $(addprefix $(DEP), $(dep))
