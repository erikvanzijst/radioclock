VERSION=1.1
BUILD="$(VERSION)-$(shell git rev-parse HEAD | cut -c 1-9) $(shell date)"
LOG_LEVEL=INFO

MK_DIR = mkdir -p
ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), Darwin)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/adc \
hpl/tc \
samda1b/gcc \
hpl/systick \
hpl/dmac \
stdio_redirect \
hal/src \
samda1b/gcc/gcc \
hpl/pm \
hpl/sysctrl \
hpl/eic \
hpl/sercom \
examples \
hpl/gclk \
stdio_redirect/gcc \
hal/utils/src \
hpl/rtc \
hpl/core \
src

# List the object files
OBJS +=  \
hal/src/hal_io.o \
stdio_redirect/stdio_io.o \
hpl/systick/hpl_systick.o \
hal/src/hal_calendar.o \
stdio_redirect/gcc/write.o \
hal/src/hal_init.o \
hpl/eic/hpl_eic.o \
hal/utils/src/utils_syscalls.o \
stdio_redirect/gcc/read.o \
hal/src/hal_timer.o \
hal/src/hal_delay.o \
hpl/sysctrl/hpl_sysctrl.o \
hpl/core/hpl_init.o \
samda1b/gcc/system_samda1.o \
hal/utils/src/utils_list.o \
hpl/core/hpl_core_m0plus_base.o \
hal/utils/src/utils_assert.o \
hpl/dmac/hpl_dmac.o \
hal/src/hal_adc_async.o \
hpl/pm/hpl_pm.o \
hal/src/hal_usart_sync.o \
hal/src/hal_i2c_m_async.o \
hpl/sercom/hpl_sercom.o \
hpl/gclk/hpl_gclk.o \
stdio_start.o \
hal/src/hal_spi_m_async.o \
main.o \
hpl/rtc/hpl_rtc.o \
examples/driver_examples.o \
driver_init.o \
hal/src/hal_ext_irq.o \
hal/utils/src/utils_ringbuffer.o \
hal/src/hal_gpio.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
samda1b/gcc/gcc/startup_samda1.o \
atmel_start.o \
hpl/tc/hpl_tc.o \
hal/src/hal_atomic.o \
hpl/adc/hpl_adc.o \
src/dcf.o \
src/dcf_parser.o \
src/dht.o \
src/display.o \
src/font.o \
src/ldr.o \
src/peripherals.o \
src/power.o \
src/switch.o \
src/utils.o \
src/millis.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"stdio_redirect/stdio_io.o" \
"hpl/systick/hpl_systick.o" \
"hal/src/hal_calendar.o" \
"stdio_redirect/gcc/write.o" \
"hal/src/hal_init.o" \
"hpl/eic/hpl_eic.o" \
"hal/utils/src/utils_syscalls.o" \
"stdio_redirect/gcc/read.o" \
"hal/src/hal_timer.o" \
"hal/src/hal_delay.o" \
"hpl/sysctrl/hpl_sysctrl.o" \
"hpl/core/hpl_init.o" \
"samda1b/gcc/system_samda1.o" \
"hal/utils/src/utils_list.o" \
"hpl/core/hpl_core_m0plus_base.o" \
"hal/utils/src/utils_assert.o" \
"hpl/dmac/hpl_dmac.o" \
"hal/src/hal_adc_async.o" \
"hpl/pm/hpl_pm.o" \
"hal/src/hal_usart_sync.o" \
"hal/src/hal_i2c_m_async.o" \
"hpl/sercom/hpl_sercom.o" \
"hpl/gclk/hpl_gclk.o" \
"stdio_start.o" \
"hal/src/hal_spi_m_async.o" \
"main.o" \
"hpl/rtc/hpl_rtc.o" \
"examples/driver_examples.o" \
"driver_init.o" \
"hal/src/hal_ext_irq.o" \
"hal/utils/src/utils_ringbuffer.o" \
"hal/src/hal_gpio.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"samda1b/gcc/gcc/startup_samda1.o" \
"atmel_start.o" \
"hpl/tc/hpl_tc.o" \
"hal/src/hal_atomic.o" \
"hpl/adc/hpl_adc.o" \
"src/dcf.o" \
"src/dcf_parser.o" \
"src/dht.o" \
"src/display.o" \
"src/font.o" \
"src/ldr.o" \
"src/peripherals.o" \
"src/power.o" \
"src/switch.o" \
"src/utils.o" \
"src/millis.o"

# List the directories containing header files
DIR_INCLUDES +=  \
-I"../" \
-I"../config" \
-I"../examples" \
-I"../hal/include" \
-I"../hal/utils/include" \
-I"../hpl/adc" \
-I"../hpl/core" \
-I"../hpl/dmac" \
-I"../hpl/eic" \
-I"../hpl/gclk" \
-I"../hpl/pm" \
-I"../hpl/port" \
-I"../hpl/rtc" \
-I"../hpl/sercom" \
-I"../hpl/sysctrl" \
-I"../hpl/systick" \
-I"../hpl/tc" \
-I"../hri" \
-I"../include" \
-I"../config" \
-I"../stdio_redirect" \
-I"../" \
-I"../CMSIS/Core/Include" \
-I"../samda1b/include"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"hal/src/hal_calendar.d" \
"stdio_redirect/stdio_io.d" \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"samda1b/gcc/system_samda1.d" \
"hpl/systick/hpl_systick.d" \
"hpl/eic/hpl_eic.d" \
"hal/utils/src/utils_syscalls.d" \
"stdio_redirect/gcc/write.d" \
"hal/src/hal_init.d" \
"hal/src/hal_timer.d" \
"hpl/core/hpl_core_m0plus_base.d" \
"hal/utils/src/utils_list.d" \
"hal/src/hal_adc_async.d" \
"hpl/dmac/hpl_dmac.d" \
"hal/utils/src/utils_assert.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/sysctrl/hpl_sysctrl.d" \
"stdio_start.d" \
"hpl/gclk/hpl_gclk.d" \
"hal/src/hal_spi_m_async.d" \
"hal/src/hal_usart_sync.d" \
"stdio_redirect/gcc/read.d" \
"driver_init.d" \
"hal/src/hal_i2c_m_async.d" \
"main.d" \
"examples/driver_examples.d" \
"hpl/rtc/hpl_rtc.d" \
"hal/src/hal_sleep.d" \
"hal/utils/src/utils_ringbuffer.d" \
"hpl/sercom/hpl_sercom.d" \
"hal/src/hal_gpio.d" \
"atmel_start.d" \
"hal/src/hal_atomic.d" \
"hpl/tc/hpl_tc.d" \
"hpl/pm/hpl_pm.d" \
"hpl/adc/hpl_adc.d" \
"hal/src/hal_ext_irq.d" \
"samda1b/gcc/gcc/startup_samda1.d" \
"src/dcf.d" \
"src/dcf_parser.d" \
"src/display.d" \
"src/dht.d" \
"src/font.d" \
"src/ldr.d" \
"src/peripherals.d" \
"src/power.d" \
"src/switch.d" \
"src/utils.d" \
"src/millis.d"

OUTPUT_FILE_NAME :=radioclock
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS) -Wl,--start-group -lm -Wl,--end-group -mthumb \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m0plus \
 \
-T"../samda1b/gcc/gcc/samda1e16b_flash.ld" \
-L"../samda1b/gcc/gcc"
	@echo Finished building target: $@

	"arm-none-eabi-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"arm-none-eabi-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"arm-none-eabi-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"arm-none-eabi-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"arm-none-eabi-size" "$(OUTPUT_FILE_NAME).elf"

	

# Compiler targets




%.o: %.c
	@echo Building file: $< "->" $@
	@echo ARM/GNU C Compiler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -DLOG_LEVEL=$(LOG_LEVEL) -DVERSION=$(BUILD) -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMDA1E16B__ -mcpu=cortex-m0plus  \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.s
	@echo Building file: $< "->" $@
	@echo ARM/GNU Assembler
	$(QUOTE)arm-none-eabi-as$(QUOTE) -x c -mthumb -DDEBUG -DLOG_LEVEL=$(LOG_LEVEL) -DVERSION=$(BUILD) -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMDA1E16B__ -mcpu=cortex-m0plus  \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.S
	@echo Building file: $< "->" $@
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -DLOG_LEVEL=$(LOG_LEVEL) -DVERSION=$(BUILD) -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMDA1E16B__ -mcpu=cortex-m0plus  \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

program: $(SUB_DIRS) $(OUTPUT_FILE_PATH)
	@echo Writing flash...
	openocd -f ../openocd.cfg -c "program $(OUTPUT_FILE_PATH) verify reset exit"

gdb: $(OUTPUT_FILE_PATH)
	arm-none-eabi-gdb -iex "target extended-remote localhost:3333" $(OUTPUT_FILE_PATH)

debug: $(OUTPUT_FILE_PATH)
	@echo Starting OpenOCD debug server...
	@( sleep 1 && echo Connect CLion Remote Debug session now...) &
	openocd -f ../openocd.cfg -c "init; reset halt"
	#openocd -f ../openocd.cfg -c "init; resume; exit"	# make does not propagate interrupts unfortunately

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec