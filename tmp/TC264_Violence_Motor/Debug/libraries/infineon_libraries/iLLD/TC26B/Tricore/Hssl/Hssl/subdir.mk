################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/IfxHssl_Hssl.c 

OBJS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/IfxHssl_Hssl.o 

COMPILED_SRCS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/IfxHssl_Hssl.src 

C_DEPS += \
./libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/IfxHssl_Hssl.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/%.src: ../libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/%.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fE:/AURIX_Project_2/Violence_Motor_1/TC264_Violence_Motor/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo '�ѽ��������� $<'
	@echo ' '

libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/%.o: ./libraries/infineon_libraries/iLLD/TC26B/Tricore/Hssl/Hssl/%.src
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo '�ѽ��������� $<'
	@echo ' '


