FCC = gcc
LDC = gcc
FLAGS = -std=c11
LD_FLAGS = -std=c11
PROG = mvp-student.x
RM = /bin/rm


all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)
%.o: %.c
	$(FCC)  $(FLAGS) -c $<

clean:
	$(RM) -rf $.o $(PROG) *.mod
