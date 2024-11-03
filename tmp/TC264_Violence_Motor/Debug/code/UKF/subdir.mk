################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/UKF/Imu2.c \
../code/UKF/UKF.c \
../code/UKF/UKFInit.c \
../code/UKF/ffun.c \
../code/UKF/hfun.c \
../code/UKF/mrdivide_helper.c \
../code/UKF/ukf6_data.c \
../code/UKF/ukf6_initialize.c \
../code/UKF/ukf6_terminate.c \
../code/UKF/xpotrf.c 

OBJS += \
./code/UKF/Imu2.o \
./code/UKF/UKF.o \
./code/UKF/UKFInit.o \
./code/UKF/ffun.o \
./code/UKF/hfun.o \
./code/UKF/mrdivide_helper.o \
./code/UKF/ukf6_data.o \
./code/UKF/ukf6_initialize.o \
./code/UKF/ukf6_terminate.o \
./code/UKF/xpotrf.o 

COMPILED_SRCS += \
./code/UKF/Imu2.src \
./code/UKF/UKF.src \
./code/UKF/UKFInit.src \
./code/UKF/ffun.src \
./code/UKF/hfun.src \
./code/UKF/mrdivide_helper.src \
./code/UKF/ukf6_data.src \
./code/UKF/ukf6_initialize.src \
./code/UKF/ukf6_terminate.src \
./code/UKF/xpotrf.src 

C_DEPS += \
./code/UKF/Imu2.d \
./code/UKF/UKF.d \
./code/UKF/UKFInit.d \
./code/UKF/ffun.d \
./code/UKF/hfun.d \
./code/UKF/mrdivide_helper.d \
./code/UKF/ukf6_data.d \
./code/UKF/ukf6_initialize.d \
./code/UKF/ukf6_terminate.d \
./code/UKF/xpotrf.d 


# Each subdirectory must supply rules for building sources it contributes
code/UKF/%.src: ../code/UKF/%.c code/UKF/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fC:/Users/Roronoa zoro/Desktop/workspace/infineon/TC264_Violence_Motor/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

code/UKF/%.o: ./code/UKF/%.src code/UKF/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


