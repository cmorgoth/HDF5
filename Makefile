CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

INC = $(shell pwd)

HDF5=/Users/cmorgoth/Downloads/hdf5-1.10.1_build/hdf5

CPPFLAGS := $(shell root-config --cflags) -I$(INC)/include -I/usr/local/opt/szip/include
LDFLAGS := -L/usr/local/Cellar/hdf5/1.10.2_1/lib /usr/local/Cellar/hdf5/1.10.2_1/lib/libhdf5_hl_cpp.a /usr/local/Cellar/hdf5/1.10.2_1/lib/libhdf5_cpp.a /usr/local/Cellar/hdf5/1.10.2_1/lib/libhdf5_hl.a /usr/local/Cellar/hdf5/1.10.2_1/lib/libhdf5.a -L/usr/local/opt/szip/lib -lsz -lz -ldl -lm $(STDLIBDIR)

LDFLAGS := $(LDFLAGS) $(shell root-config --glibs) -lMathMore
CPPFLAGS += -g

TARGET = example

SRC = example.cc
OBJ = $(SRC:.cc=.o)

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(LD) $(CPPFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

%.o : %.cc
	$(CXX) $(CPPFLAGS) -o $@ -c $<
	@echo $@
	@echo $<
clean :
	rm -f *.o src/*.o $(TARGET) *~
