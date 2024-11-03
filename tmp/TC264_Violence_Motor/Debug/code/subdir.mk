################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Img.c \
../code/Img_Base.c \
../code/Img_Cross.c \
../code/Img_Error.c \
../code/Img_Island.c \
../code/Img_Park.c \
../code/Init.c \
../code/List.c \
../code/Menu.c \
../code/RectChange.c \
../code/Servo.c \
../code/Task.c \
../code/euler.c \
../code/filter.c \
../code/key.c \
../code/lcd.c \
../code/led.c \
../code/maths.c \
../code/motor.c \
../code/pico_gy.c \
../code/pico_link_ii.c \
../code/pid.c \
../code/vofa.c \
../code/zf_device_dot_matrix_screen.c \
../code/zf_device_tld7002.c 

OBJS += \
./code/Img.o \
./code/Img_Base.o \
./code/Img_Cross.o \
./code/Img_Error.o \
./code/Img_Island.o \
./code/Img_Park.o \
./code/Init.o \
./code/List.o \
./code/Menu.o \
./code/RectChange.o \
./code/Servo.o \
./code/Task.o \
./code/euler.o \
./code/filter.o \
./code/key.o \
./code/lcd.o \
./code/led.o \
./code/maths.o \
./code/motor.o \
./code/pico_gy.o \
./code/pico_link_ii.o \
./code/pid.o \
./code/vofa.o \
./code/zf_device_dot_matrix_screen.o \
./code/zf_device_tld7002.o 

COMPILED_SRCS += \
./code/Img.src \
./code/Img_Base.src \
./code/Img_Cross.src \
./code/Img_Error.src \
./code/Img_Island.src \
./code/Img_Park.src \
./code/Init.src \
./code/List.src \
./code/Menu.src \
./code/RectChange.src \
./code/Servo.src \
./code/Task.src \
./code/euler.src \
./code/filter.src \
./code/key.src \
./code/lcd.src \
./code/led.src \
./code/maths.src \
./code/motor.src \
./code/pico_gy.src \
./code/pico_link_ii.src \
./code/pid.src \
./code/vofa.src \
./code/zf_device_dot_matrix_screen.src \
./code/zf_device_tld7002.src 

C_DEPS += \
./code/Img.d \
./code/Img_Base.d \
./code/Img_Cross.d \
./code/Img_Error.d \
./code/Img_Island.d \
./code/Img_Park.d \
./code/Init.d \
./code/List.d \
./code/Menu.d \
./code/RectChange.d \
./code/Servo.d \
./code/Task.d \
./code/euler.d \
./code/filter.d \
./code/key.d \
./code/lcd.d \
./code/led.d \
./code/maths.d \
./code/motor.d \
./code/pico_gy.d \
./code/pico_link_ii.d \
./code/pid.d \
./code/vofa.d \
./code/zf_device_dot_matrix_screen.d \
./code/zf_device_tld7002.d 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fE:/AURIX_Project_2/Violence_Motor_1/TC264_Violence_Motor/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo '已结束构建： $<'
	@echo ' '

code/%.o: ./code/%.src
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo '已结束构建： $<'
	@echo ' '


