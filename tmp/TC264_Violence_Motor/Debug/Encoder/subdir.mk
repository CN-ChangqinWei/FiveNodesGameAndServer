################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Encoder/Timer.c \
../Encoder/my_speed.c \
../Encoder/my_timer.c 

OBJS += \
./Encoder/Timer.o \
./Encoder/my_speed.o \
./Encoder/my_timer.o 

COMPILED_SRCS += \
./Encoder/Timer.src \
./Encoder/my_speed.src \
./Encoder/my_timer.src 

C_DEPS += \
./Encoder/Timer.d \
./Encoder/my_speed.d \
./Encoder/my_timer.d 


# Each subdirectory must supply rules for building sources it contributes
Encoder/%.src: ../Encoder/%.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fE:/AURIX_Project_2/Violence_Motor_1/TC264_Violence_Motor/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo '�ѽ��������� $<'
	@echo ' '

Encoder/%.o: ./Encoder/%.src
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo '�ѽ��������� $<'
	@echo ' '


