CC = gcc
LD = gcc
CFLAGS = -Ofast
LDFLAGS = -Ofast
LIBS = -lm

OBJS = sim.o funcs.o utils.o file_ops.o timestep.o

EXECUTABLE = simulate

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

sim.o: sim.c funcs.h structs.h file_ops.h
	$(CC) $(CFLAGS) -c sim.c

funcs.o: funcs.c funcs.h
	$(CC) $(CFLAGS) -c funcs.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

file_ops.o: file_ops.c file_ops.h
	$(CC) $(CFLAGS) -c file_ops.c

timestep.o: timestep.c timestep.h
	$(CC) $(CFLAGS) -c timestep.c

clean:
	rm -f $(EXECUTABLE) $(OBJS) *~

