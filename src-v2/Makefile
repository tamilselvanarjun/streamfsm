#UNAME=$(shell uname)
#ifeq (${UNAME}, Linux)
	#MAKEFILE_AUX=Makefile.Linux
#else
	#MAKEFILE_AUX=Makefile.OSX
#endif

CXX=g++
OPTFLAGS=-O3
CFLAGS=
IFLAGS=-I../../include -I$(HOME)/research/libraries/boost_1_51_0/ #-I../lib/boost_1_51_0/ #-I$(HOME)/research/graphstreams/tools/gboost-0.1.1/src-graphmatch/vflib-2.0/include
#LFLAGS= -L$(HOME)/research/graphstreams/tools/gboost-0.1.1/src-graphmatch/vflib-2.0/lib -lvf

XMT=${MTANAME}
ifeq ($(XMT), mta)
	CXX=c++
	PROGLIB=$(OBJ_DIR)/cassnlp.pl
	CFLAGS=-par -pl ${PROGLIB}
	IFLAGS=-I$(HOME)/dev/lib -I../../include -I$(HOME)/research/libraries/boost_1_51_0/ -I$(HOME)/research/graphstreams/tools/gboost-0.1.1/src-graphmatch/vflib-2.0/include
	OPTFLAGS=
endif

include $(MAKEFILE_AUX)
TARGET=../bin/gload
SRCS=$(shell ls *.cc)
OBJ_DIR=.obj
OBJS=$(SRCS:%.cc=$(OBJ_DIR)/%.o)

all: test ${TARGET}

test:${OBJS}
	${CXX} ${OPTFLAGS} -o run_test.x ${OBJS} ${LFLAGS}
	cp test_driver.py ../bin
	cp make_batches.py ../data
${TARGET}: ${OBJS} ${HPPS} 
	${CXX} ${OPTFLAGS} -o $@ ${OBJS} ${LFLAGS} 
$(OBJ_DIR)/%.o: %.cc
	@mkdir -p $(OBJ_DIR)
	${CXX} ${OPTFLAGS} -o $@ ${CFLAGS} -c $< $(IFLAGS) 
clean:
	rm -fr ${TARGET} ${OBJS} 
