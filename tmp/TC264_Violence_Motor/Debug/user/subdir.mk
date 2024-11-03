################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/cpu0_main.c \
../user/cpu1_main.c \
../user/isr.c 

OBJS += \
./user/cpu0_main.o \
./user/cpu1_main.o \
./user/isr.o 

COMPILED_SRCS += \
./user/cpu0_main.src \
./user/cpu1_main.src \
./user/isr.src 

C_DEPS += \
./user/cpu0_main.d \
./user/cpu1_main.d \
./user/isr.d 


# Each subdirectory must supply rules for building sources it contributes
user/%.src: ../user/%.c
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fE:/AURIX_Project_2/Violence_Motor_1/TC264_Violence_Motor/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo '已结束构建： $<'
	@echo ' '

user/%.o: ./user/%.src
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo '已结束构建： $<'
	@echo ' '


