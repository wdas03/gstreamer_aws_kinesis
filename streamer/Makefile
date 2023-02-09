# make run

CC = gcc
CXX = g++

C_VERSION = c17
CXX_VERSION = c++17

CFLAGS = -std=$(C_VERSION) -g -Wall -Iinclude -I/opt/homebrew/include `wx-config --cxxflags` `pkg-config --cflags gstreamer-1.0 gstreamer-plugins-base-1.0 gstreamer-plugins-bad-1.0 gtkmm-4.0`
CXXFLAGS = -std=$(CXX_VERSION) -g -Wall -Iinclude -I/opt/homebrew/include `wx-config --cxxflags` `pkg-config --cflags gstreamer-1.0 gstreamer-plugins-base-1.0 gstreamer-plugins-bad-1.0 gtkmm-4.0`
LDFLAGS = `wx-config --libs` `pkg-config --libs gstreamer-1.0 gstreamer-plugins-base-1.0 gstreamer-plugins-bad-1.0 gtkmm-4.0`

TARGET = main 

OBJFILES = gstDeviceUtil.o gstStreamUtil.o $(TARGET:=.o)
DEPS := $(OBJFILES:.o=.d)

.PHONY: build
build:
	make clean; make all

.PHONY: all 
all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(OBJFILES) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#main.o: main.cpp
#	$(CC) $(CFLAGS) -c $(MAIN_FILE)

.PHONY: run
run: 
	make clean $(TARGET); make $(TARGET); ./$(TARGET)	

.PHONY: clean
clean:
	rm -rf $(OBJFILES) $(DEPS) $(TARGET)
