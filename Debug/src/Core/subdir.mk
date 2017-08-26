################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Core/MThread.cpp \
../src/Core/MultipleTCPSocketsListener.cpp \
../src/Core/Semaphore.cpp \
../src/Core/TCPSocket.cpp 

OBJS += \
./src/Core/MThread.o \
./src/Core/MultipleTCPSocketsListener.o \
./src/Core/Semaphore.o \
./src/Core/TCPSocket.o 

CPP_DEPS += \
./src/Core/MThread.d \
./src/Core/MultipleTCPSocketsListener.d \
./src/Core/Semaphore.d \
./src/Core/TCPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/Core/%.o: ../src/Core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


