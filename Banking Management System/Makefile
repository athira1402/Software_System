CC = gcc

CFLAGS = -Wall -Wextra -g -std=c11

LDFLAGS = -pthread


SERVER_SRCS = src/server.c src/customer_ops.c src/transactions.c src/employee_ops.c src/manager_ops.c src/admin_ops.c 

# --- Executables (Targets) ---

SERVER_EXEC = bankServer
CLIENT_EXEC = bankClient
INIT_EXEC = initdb

.PHONY: all
all: $(SERVER_EXEC) $(CLIENT_EXEC) $(INIT_EXEC)

$(SERVER_EXEC): $(SERVER_SRCS) src/customer_client.c src/manager_client.c src/admin_client.c src/employee_client.c
	@echo "Building Bank Server ($(SERVER_EXEC))..."
	# $^ expands to all prerequisites (source files with src/)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(CLIENT_EXEC): src/client.c src/common.c
	@echo "Building Simple Client ($(CLIENT_EXEC))..."
	$(CC) $(CFLAGS) $^ -o $@


$(INIT_EXEC): initdb.c
	@echo "Building Database Initializer ($(INIT_EXEC))..."
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@echo "Cleaning up build files..."
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC) $(INIT_EXEC)
# 	# This command removes ALL files and subdirectories inside the 'data/' directory, 
# 	# but keeps the 'data/' directory itself.
# 	rm -f data/*
	rm -f *.o
