################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DMS.c \
../src/Hll.c \
../src/PID.c \
../src/Schedular.c \
../src/SchoolZone.c \
../src/adc.c \
../src/cameraController.c \
../src/driveController.c \
../src/encoder.c \
../src/encoder_set.c \
../src/imageProcess.c \
../src/interrupts.c \
../src/main.c \
../src/queueADT.c \
../src/rpm_RC.c \
../src/systemController.c \
../src/timerInputModule.c \
../src/timerOutputModule.c \
../src/vehicleController.c 

OBJS += \
./src/DMS.o \
./src/Hll.o \
./src/PID.o \
./src/Schedular.o \
./src/SchoolZone.o \
./src/adc.o \
./src/cameraController.o \
./src/driveController.o \
./src/encoder.o \
./src/encoder_set.o \
./src/imageProcess.o \
./src/interrupts.o \
./src/main.o \
./src/queueADT.o \
./src/rpm_RC.o \
./src/systemController.o \
./src/timerInputModule.o \
./src/timerOutputModule.o \
./src/vehicleController.o 

C_DEPS += \
./src/DMS.d \
./src/Hll.d \
./src/PID.d \
./src/Schedular.d \
./src/SchoolZone.d \
./src/adc.d \
./src/cameraController.d \
./src/driveController.d \
./src/encoder.d \
./src/encoder_set.d \
./src/imageProcess.d \
./src/interrupts.d \
./src/main.d \
./src/queueADT.d \
./src/rpm_RC.d \
./src/systemController.d \
./src/timerInputModule.d \
./src/timerOutputModule.d \
./src/vehicleController.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TriCore C Compiler'
	"$(TRICORE_TOOLS)/bin/tricore-gcc" -c -I"../h" -fno-common -Os -g3 -W -Wall -Wextra -Wdiv-by-zero -Warray-bounds -Wcast-align -Wignored-qualifiers -Wformat -Wformat-security -DAPPKIT_TC237TFT -fshort-double -mcpu=tc23xx -mversion-info -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


