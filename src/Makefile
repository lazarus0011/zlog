#生成共享库

CC=g++
#生成的目标名称
TARGET = zlog

#当前目录下所有的cpp源文件
SRCS = $(wildcard *.cpp)

#所有的.o文件列表
OBJS=$(SRCS:.cpp=.o)

CCFLAGS = -Wall 
#头文件路径
INCS = -I$(PROJECT_INCS)
CCFLAGS += $(INCS) 

#链接选项
#LKFLAGS = -shared

#依赖的动态库
LIBS = -lpthread 


$(TARGET):$(OBJS)
	$(CC) $(LIBS) -o $(TARGET) $^
	cp $(TARGET) $(PROJECT_BIN)
	rm -rf $(OBJS)

clean:
	rm -rf $(TARGET) $(OBJS)
%.o:%.c
	$(CC) $(CCFLAGS) -o $@ -fPIC -c $<

#include $(PROJECT_BASE_MKFILE)/Makefile.GNU
