################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools.cpp \
../pixel_tbb.cpp \
../New_Algorithm.cpp

OBJS += \
./tools.o \
./pixel_tbb.o \
./New_Algorithm.o

CPP_DEPS += \
./tools.d \
./pixel_tbb.d \
./New_Algorithm.d

ADD_RULES = -L/usr/share/tbb/lib/intel64/cc4.1.0_libc2.4_kernel2.6.16.21 -ltbb

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(ADD_RULES) -O2 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


