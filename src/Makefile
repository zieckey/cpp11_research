# weizili@360.cn 

CC=gcc
CXX=g++
AR=ar
ARFLAGS=cru
CURRENT_DIR=$(shell echo `pwd`)
PARENT_DIRNAME=$(shell basename $(shell dirname $(CURRENT_DIR)))
SOURCE_DIR=$(CURRENT_DIR)/src
CFLAGS=$(TOP_CFLAGS) -g -c -D_DEBUG -D_USE_GTEST -fPIC -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wall -I${SOURCE_DIR}/../3rdparty -MMD
CPPFLAGS=-Woverloaded-virtual -Wsign-promo -fno-gnu-keywords  -std=c++0x
LDFLAGS=-L/usr/local/lib -lpthread -lgtest -lgtest_main
DIRNAME=$(shell basename `pwd`)

#SRCS := $(filter-out main.cc,$(ALLSRCS))
SRCS := $(wildcard *.cc)
OBJS := $(patsubst %.cc, %.o, $(SRCS))
DEPS := $(patsubst %.o, %.d, $(OBJS))

TARGET=$(PARENT_DIRNAME)_$(DIRNAME)

all : $(TARGET) 
	$(MAKE) install

check : $(TARGET)
	./$^

$(TARGET) : $(OBJS)
	$(MAKE) -C ..
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

install: $(TARGET) 
	cp -f $(TARGET) $(PREFIX)/bin

clean:
	rm -rf *.o *.d $(OBJS) $(DEPS) $(TARGET) *.tmp decoded.txt log

