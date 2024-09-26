IDIR = ./inc
CC = gcc
#CFLAGS = -I
CURRENT_HASH := '"$(shell git rev-parse HEAD)"'
CURRENT_DATE := '"$(shell date --utc +"%Y%m%dT%H%M%SZ")"'
CURRENT_NAME := '"cautil"'

cautil: cautil.cpp G2INIT.cpp
	$(CC) cautil.cpp G2INIT.cpp -o cautil -DCURRENT_DATE=$(CURRENT_DATE) \
	-DCURRENT_HASH=$(CURRENT_HASH) -DCURRENT_NAME=$(CURRENT_NAME)

debug: cautil.cpp G2INIT.cpp
	$(CC) -g cautil.cpp G2INTI.cpp -o cautil -DCURRENT_DATE=$(CURRENT_DATE) \
	-DCURRENT_HASH=$(CURRENT_HASH) -DCURRENT_NAME=$(CURRENT_NAME)

clean:
	rm cautil