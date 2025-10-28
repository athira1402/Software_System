CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# Source files
SERVER_SRCS = src/server.c src/customer_ops.c src/employee_ops.c
MAIN_SERVER_SRCS = src/main_server.c src/client.c src/employee_client.c

# Output binaries
SERVER_BIN = server
MAIN_SERVER_BIN = main_server

# Default rule
all: $(SERVER_BIN) $(MAIN_SERVER_BIN)

$(SERVER_BIN): $(SERVER_SRCS) include/bank.h
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRCS)

$(MAIN_SERVER_BIN): $(MAIN_SERVER_SRCS) include/bank.h
	$(CC) $(CFLAGS) -o $(MAIN_SERVER_BIN) $(MAIN_SERVER_SRCS)

clean:
	rm -f $(SERVER_BIN) $(MAIN_SERVER_BIN) *.o

run:
	./$(MAIN_SERVER_BIN)
