# 1. 컴파일러 및 컴파일 옵션 설정
CC = gcc
CFLAGS = -Wall -Wextra -g -I$(INC_DIR) # 모든 경고 켜기, 디버깅 심볼 추가, 헤더 파일 폴더 지정

# 2. 디렉토리 설정
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# 3. 최종 생성될 실행 파일의 이름과 경로
TARGET = $(BIN_DIR)/CPU_Scheduler

# 4. 소스 파일 및 목적 파일 자동 추적
# src/ 폴더 안의 모든 .c 파일을 찾아 SRCS에 저장 (예: src/main.c src/board.c)
SRCS = $(wildcard $(SRC_DIR)/*.c)
# SRCS의 목록에서 'src/'를 'obj/'로, '.c'를 '.o'로 바꿔서 OBJS에 저장 (예: obj/main.o obj/board.o)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# 5. 기본 빌드 타겟 (터미널에 'make'만 쳤을 때 실행되는 곳)
all: prep $(TARGET)

# 필요한 폴더(obj, bin)가 없으면 자동으로 생성
prep:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# 6. 실행 파일 링킹 (obj/*.o 들을 묶어서 최종 바이너리 생성)
# $@는 타겟(TARGET), $^는 의존성 배열(OBJS)을 의미함
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✨ Build Complete: $(TARGET)"

# 7. 개별 소스 코드 컴파일 (변경된 .c 파일만 똑똑하게 다시 .o로 컴파일)
# $<는 첫 번째 의존성(해당 .c 파일), $@는 타겟(해당 .o 파일)을 의미함
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "🔨 Compiled: $< -> $@"

# 8. 청소 타겟 (터미널에 'make clean' 입력 시 실행)
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "🗑️ Cleaned build files (obj/, bin/ removed)."

# 9. 편의 타겟 (터미널에 'make run' 입력 시 빌드 후 자동 실행)
run: all
	@echo "🚀 Running $(TARGET)...\n"
	@./$(TARGET)

# .PHONY: 동일한 이름의 파일이 폴더에 있어도 무시하고 명령어로서 실행하도록 보장
.PHONY: all prep clean run