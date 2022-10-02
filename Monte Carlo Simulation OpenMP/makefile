CC = mpicc
CPP = mpic++
LDC = mpicc
LD_FLAGS = -lm -fopenmp -lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgproc
FLAGS = -fopenmp
CPPFLAGS = -I/usr/include.opencv $(FLAGS)
PROG = la3.cx
RM = /bin/rm

#all rule
OBJS = $(PROG:%.cx=%.o)

all: $(PROG)

$(PROG): $(OBJS)
		$(LDC) $^ $(LD_FLAGS) -o $@

%.o: %.c
		$(CC) $(FLAGS) -c $^ -o $@

#clean rule
clean:
		$(RM) -rf *.o *.x *.cx *.mod
