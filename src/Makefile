PLATFORM := $(shell uname)
PLATFORM_VERSION := $(shell uname -r| awk -F 'el' '{printf("el%d", substr($$2,1,1))}')

ifeq ($(PLATFORM_VERSION),el6)
GCC_PATH=/usr/local/gcc-4.7.2
GCC_PATH=/home/s/gcc-4.9.2
CXX := env LD_LIBRARY_PATH=$(GCC_PATH)/lib $(GCC_PATH)/bin/g++
else
CXX := g++
endif

CURRENT_DIR=$(shell echo `pwd`)
PARENT_DIRNAME=$(shell basename $(shell dirname $(CURRENT_DIR)))
SOURCE_DIR=$(CURRENT_DIR)
PREFIX=$(CURRENT_DIR)/../install

GTESTDIR = linux_gtest

CPPFLAGS= -g -c -fPIC \
		  -D_DEBUG -D_USE_GTEST \
		  -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings \
		  -Wsign-compare -Winvalid-pch -fms-extensions -Wall \
		  -MMD -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords -std=c++11 \
		  -I ./$(GTESTDIR)\
		  -I $(SOURCE_DIR)/../3rdparty \
		  -I /home/s/safe/include \

RTFLAGS := \
    -Wl,-rpath=. \
    -Wl,-rpath=/home/s/safe/lib \
    -Wl,-rpath=$(GCC_PATH)/lib64

#LDFLAGS= $(RTFLAGS) -lgtest -lgtest_main -lpthread 
LDFLAGS= $(RTFLAGS) -L/home/s/safe/lib -lpthread -lwdurlsplit -ldl

DIRNAME=$(shell basename `pwd`)

#SRCS := $(filter-out main.cc,$(ALLSRCS))
SRCS := $(wildcard *.cc) $(GTESTDIR)/src/gtest-all.cc $(GTESTDIR)/src/gtest_main.cc
OBJS := $(patsubst %.cc, %.o, $(SRCS))
DEPS := $(patsubst %.o, %.d, $(OBJS))

TARGET=$(PARENT_DIRNAME)

all : $(TARGET) 

check : $(TARGET)
	./$^

$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

-include $(DEPS)

%.o : %.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) $< -o $@

t:
	@echo "DIRNAME:" $(DIRNAME)
	@echo "PARENT_DIRNAME:" $(PARENT_DIRNAME)
	@echo "ALLSRCS:" $(ALLSRCS)
	@echo "   SRCS:" $(SRCS)
	@echo "OBJS:" $(OBJS)
	@echo "CFLAGS:" $(CFLAGS)
	@echo "CPPFLAGS:" $(CPPFLAGS)
	@echo $(PLATFORM_VERSION)

install: $(TARGET) 
	cp -f $(TARGET) $(PREFIX)/bin

clean:
	rm -rf *.o *.d $(OBJS) $(DEPS) $(TARGET) *.tmp decoded.txt log

