# ---------------------------------------------------------
# 1. OS Detection
# ---------------------------------------------------------
ifeq ($(OS),Windows_NT)
    PLATFORM := Windows
    SHELL = C:/Program Files/Git/bin/sh.exe
else
    PLATFORM := $(shell uname -s)
endif

# ---------------------------------------------------------
# 2. Dynamic Variables
# ---------------------------------------------------------
CURRENT_HASH := $(shell git rev-parse HEAD)
CURRENT_DATE := $(shell date --utc +"%Y%m%dT%H%M%SZ")
CURRENT_NAME := cautil

# ---------------------------------------------------------
# 3. Toolchain Configuration
# ---------------------------------------------------------
ifeq ($(PLATFORM),Windows)
    # Microsoft Visual C++ (cl.exe)
    CXX = cl
    CXXFLAGS = /nologo /EHsc /D CURRENT_DATE=\"$(CURRENT_DATE)\" /D CURRENT_HASH=\"$(CURRENT_HASH)\" /D CURRENT_NAME=\"$(CURRENT_NAME)\"
    OUT_FLAG = /Fe:
    # FIX: Since SHELL is Git Bash, use POSIX 'rm -f' instead of CMD 'del'
    CLEAN_CMD = rm -f cautil.exe *.obj
else
    # GCC / Clang (Linux / Mac)
    CXX = g++
    CXXFLAGS = -Wall -DCURRENT_DATE='"$(CURRENT_DATE)"' -DCURRENT_HASH='"$(CURRENT_HASH)"' -DCURRENT_NAME='"$(CURRENT_NAME)"'
    OUT_FLAG = -o 
    CLEAN_CMD = rm -f cautil *.o
endif

# ---------------------------------------------------------
# 4. Build Targets
# ---------------------------------------------------------

cautil: cautil.cpp G2INIT.cpp
	$(CXX) $(CXXFLAGS) cautil.cpp G2INIT.cpp $(OUT_FLAG)cautil

# FIX: Makefile 'ifeq' conditionals cannot be indented inside recipes.
# We use a shell conditional or a multi-line variable here instead.
debug: cautil.cpp G2INIT.cpp
ifeq ($(PLATFORM),Windows)
	$(CXX) /Zi $(CXXFLAGS) cautil.cpp G2INIT.cpp $(OUT_FLAG)cautil
else
	$(CXX) -g $(CXXFLAGS) cautil.cpp G2INIT.cpp $(OUT_FLAG)cautil
endif

clean:
	$(CLEAN_CMD)
