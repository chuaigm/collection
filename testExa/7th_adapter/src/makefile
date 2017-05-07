#chuaiGM multifile project makefile
# a test project
cc = g++
obj_dir = ../obj
lib_dir = ../lib
target_dir = ../bin
C++FLAGS = -w

# src directory
src_dir   = ./
src_file  = chuaiMain.cpp
src_dir1  = ./class1
src_file1 = platform_code.cpp
src_dir2  = ./class2
src_file2 = 
# include dir
INCLUDE_DIR = -I./ -I./class1 -I./class2

# source add dir and file up
SOURCE  = $(addprefix $(src_dir)/,$(src_file))
SOURCE1 = $(addprefix $(src_dir1)/,$(src_file1))
SOURCE2 = $(addprefix $(src_dir2)/,$(src_file2))
# obj
OBJECT  = $(src_file:.cpp=.o)
OBJECT1 = $(src_file1:.cpp=.o)
OBJECT2 = $(src_file2:.cpp=.o)

SRC_ALL = $(SOURCE1) $(SOURCE2) $(SOURCE)
OBJ_ALL = $(OBJECT1) $(OBJECT2) $(OBJECT)
OBJ_OUT = $(addprefix $(obj_dir)/,$(OBJ_ALL))

TARGET_FILE = exeFile
# all
$(TARGET_FILE) : $(OBJ_ALL)
	$(cc) -o $@ $(OBJ_OUT)
#	cp $(TARGET_FILE) $(target_dir)

# obj1
$(OBJECT1) : $(SOURCE1)
	@echo " "
	@echo "___Compile___"
	$(cc) -c $(C++FLAGS) -o $(addprefix $(obj_dir)/,$(notdir $*).o) $(addprefix $(src_dir1)/,$(notdir $*).cpp) $(INCLUDE_DIR)
# obj2
$(OBJECT2) : $(SOURCE2)
	@echo " "
	@echo "___Compile___"
	$(cc) -c $(C++FLAGS) -o $(addprefix $(obj_dir)/,$(notdir $*).o) $(addprefix $(src_dir2)/,$(notdir $*).cpp) $(INCLUDE_DIR)

.PHONY : clean
clean:
	rm -f $(TARGET_FILE) $(obj_dir)/*

.SUFFIXES: .cpp .o
.cpp.o:
	@echo " "
	@echo "___Comple___..."
	$(cc) -c $(C++FLAGS) -o $(addprefix $(obj_dir)/,$(notdir $*).o) $(notdir $*).cpp $(INCLUDE_DIR)
