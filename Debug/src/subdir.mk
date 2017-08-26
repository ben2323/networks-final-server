################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TCPMessengerServer.cpp \
../src/TCPMsnDispatcher.cpp \
../src/UsersManager.cpp \
../src/server.cpp 

OBJS += \
./src/TCPMessengerServer.o \
./src/TCPMsnDispatcher.o \
./src/UsersManager.o \
./src/server.o 

CPP_DEPS += \
./src/TCPMessengerServer.d \
./src/TCPMsnDispatcher.d \
./src/UsersManager.d \
./src/server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


