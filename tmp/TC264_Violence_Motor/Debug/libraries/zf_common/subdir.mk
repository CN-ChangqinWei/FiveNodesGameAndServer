################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/zf_common/zf_common_clock.c \
../libraries/zf_common/zf_common_debug.c \
../libraries/zf_common/zf_common_fifo.c \
../libraries/zf_common/zf_common_font.c \
../libraries/zf_common/zf_common_function.c \
../libraries/zf_common/zf_common_interrupt.c 

OBJS += \
./libraries/zf_common/zf_common_clock.o \
./libraries/zf_common/zf_common_debug.o \
./libraries/zf_common/zf_common_fifo.o \
./libraries/zf_common/zf_common_font.o \
./libraries/zf_common/zf_common_function.o \
./libraries/zf_common/zf_common_interrupt.o 

COMPILED_SRCS += \
./libraries/zf_common/zf_common_clock.src \
./libraries/zf_common/zf_common_debug.src \
./libraries/zf_common/zf_common_fifo.src \
./libraries/zf_common/zf_common_font.src \
./libraries/zf_common/zf_common_function.src \
./libraries/zf_common/zf_common_interrupt.src 

C_DEPS += \
./libraries/zf_common/zf_common_clock.d \
./libraries/zf_common/zf_common_debug.d \
./libraries/zf_common/zf_common_fifo.d \
./libraries/zf_common/zf_common_font.d \
./libraries/zf_common/zf_common_function.d \
./libraries/zf_common/zf_common_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/zf_common/%.src: ../libraries/zf_common/%.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fE:/AURIX_Project_2/Violence_Motor_1/TC264_Violence_Motor/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo '�ѽ��������� $<'
	@echo ' '

libraries/zf_common/%.o: ./libraries/zf_common/%.src
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo '�ѽ��������� $<'
	@echo ' '


