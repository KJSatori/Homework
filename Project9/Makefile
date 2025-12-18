CC = g++
CCFLAGS = -g -w
CCLIBS = -lncurses

# 这样可以避免重复的把 *.o 写好多次
SRCS := main.C Gui.C Game.C Item.C Pizza.C Noodle.C Rabbit.C Wolf.C Tiger.C Grass.C
OBJS := $(SRCS:.C=.o)
DEPS := $(SRCS:.C=.d)

# 文件名
TARGET := mygame

.PHONY: all clean run debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^ $(CCLIBS)

# 将.C 编译为 .o
%.o: %.C
	$(CC) $(CCFLAGS) -MMD -MP -c -o $@ $<

-include $(DEPS)

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS := -g -O0 -Wall -std=c++17 -I.
debug: clean all

# 清理
clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)