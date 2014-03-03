################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Cycle.cpp \
../src/Machine.cpp \
../src/Main.cpp \
../src/Point.cpp \
../src/PointArray.cpp 

OBJS += \
./src/Cycle.o \
./src/Machine.o \
./src/Main.o \
./src/Point.o \
./src/PointArray.o 

CPP_DEPS += \
./src/Cycle.d \
./src/Machine.d \
./src/Main.d \
./src/Point.d \
./src/PointArray.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


