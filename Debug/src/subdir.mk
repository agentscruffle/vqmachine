################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Cycle.cpp \
../src/EndingTuple.cpp \
../src/Machine.cpp \
../src/Main.cpp \
../src/Segment.cpp \
../src/SegmentArray.cpp \
../src/StartingTuple.cpp \
../src/Tuple.cpp 

OBJS += \
./src/Cycle.o \
./src/EndingTuple.o \
./src/Machine.o \
./src/Main.o \
./src/Segment.o \
./src/SegmentArray.o \
./src/StartingTuple.o \
./src/Tuple.o 

CPP_DEPS += \
./src/Cycle.d \
./src/EndingTuple.d \
./src/Machine.d \
./src/Main.d \
./src/Segment.d \
./src/SegmentArray.d \
./src/StartingTuple.d \
./src/Tuple.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


