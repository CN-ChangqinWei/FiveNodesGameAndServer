################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/TLD7002_driver/TLD7002FuncLayer.c \
../code/TLD7002_driver/TLD7002_ControlLayer.c \
../code/TLD7002_driver/TLD7002_ServiceLayer.c 

OBJS += \
./code/TLD7002_driver/TLD7002FuncLayer.o \
./code/TLD7002_driver/TLD7002_ControlLayer.o \
./code/TLD7002_driver/TLD7002_ServiceLayer.o 

COMPILED_SRCS += \
./code/TLD7002_driver/TLD7002FuncLayer.src \
./code/TLD7002_driver/TLD7002_ControlLayer.src \
./code/TLD7002_driver/TLD7002_ServiceLayer.src 

C_DEPS += \
./code/TLD7002_driver/TLD7002FuncLayer.d \
./code/TLD7002_driver/TLD7002_ControlLayer.d \
./code/TLD7002_driver/TLD7002_ServiceLayer.d 


# Each subdirectory must supply rules for building sources it contributes
code/TLD7002_driver/%.src: ../code/TLD7002_driver/%.c
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fE:/AURIX_Project_2/Violence_Motor_1/TC264_Violence_Motor/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo '已结束构建： $<'
	@echo ' '

code/TLD7002_driver/%.o: ./code/TLD7002_driver/%.src
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo '已结束构建： $<'
	@echo ' '


