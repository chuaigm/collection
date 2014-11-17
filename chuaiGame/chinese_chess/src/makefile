#chuaiGM multifile project makefile
# a test project
cc = g++
obj_dir = ../obj
lib_dir = ../lib
target_dir = ../bin
C++FLAGS = -w -g
#C++FLAGS = -static

# src directory
src_dir   = ./
src_file  = main_sh.cpp
# include dir
INCLUDE_DIR = -I./

# source add dir and file up
SOURCE  = $(addprefix $(src_dir)/,$(src_file))
# obj
OBJECT  = $(src_file:.cpp=.o)

SRC_ALL = $(SOURCE)
OBJ_ALL = $(OBJECT)
OBJ_OUT = $(addprefix $(obj_dir)/,$(OBJ_ALL))

TARGET_FILE = chinese_chess
# all
$(TARGET_FILE) : $(OBJ_ALL)
	$(cc) $(C++FLAGS) -o $@ $(OBJ_OUT)
	#cp $(TARGET_FILE) $(target_dir)

.PHONY : clean
clean:
	rm -f $(TARGET_FILE) $(obj_dir)/*

.SUFFIXES: .cpp .o
.cpp.o:
	@echo " "
	@echo "___Comple___..."
	$(cc) -c $(C++FLAGS) -o $(addprefix $(obj_dir)/,$(notdir $*).o) $(notdir $*).cpp $(INCLUDE_DIR)
