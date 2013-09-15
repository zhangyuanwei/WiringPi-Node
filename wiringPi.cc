#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <wiringPi.h>

using namespace v8;
using namespace node;

#define WIRING_FUNC(NAME) _wiring_##NAME
#define WIRING_NODE_FUNC(NAME)											\
	static Handle<Value> WIRING_FUNC(NAME) (const Arguments& args)

#define WIRING_RETURN(RET) return scope.Close(RET)
#define WIRING_RETURN_UNDEFINED() WIRING_RETURN(Undefined())
#define WIRING_RETURN_NUMBER(X) WIRING_RETURN(Number::New(X))

#define WIRING_BIND_METHOD(NAME) 										\
	NODE_SET_METHOD(target, #NAME, WIRING_FUNC(NAME))
#define WIRING_DEFINE_CONSTANT(NAME)									\
	(target)->Set(														\
		v8::String::NewSymbol(#NAME), 									\
		v8::Number::New(NAME), 											\
		static_cast<v8::PropertyAttribute>(								\
			v8::ReadOnly|v8::DontDelete))

#define WIRING_ARGCHK_LEN(N)											\
	do{																	\
		if (args.Length() < N) {										\
			ThrowException(Exception::TypeError(						\
				String::New("Need "#N" arguments.")));					\
			WIRING_RETURN_UNDEFINED();									\
		}																\
	}while(0)

#define WIRING_ARGCHK_INT(N, BIT)										\
	do{																	\
		if (!args[N]->IsNumber() || !args[N]->IsInt##BIT()) { 			\
			ThrowException(Exception::TypeError(						\
				String::Concat(String::New(								\
					"Argument "#N" type error. need int"#BIT", get "),	\
				args[N]->ToString())));									\
			WIRING_RETURN_UNDEFINED();									\
		}																\
	}while(0)

#define WIRING_ARGCHK_UINT(N, BIT)										\
	do{																	\
		if (!args[N]->IsNumber() || !args[N]->IsUint##BIT()) {			\
			ThrowException(Exception::TypeError(						\
				String::Concat(String::New(								\
					"Argument "#N" type error. need uint"#BIT", get "),	\
				args[N]->ToString())));									\
			WIRING_RETURN_UNDEFINED();									\
		}																\
	}while(0)

#define WIRING_ARGCHK_FUNC(N)											\
	do{																	\
		if (!args[N]->IsFunction()) {									\
			ThrowException(Exception::TypeError(						\
				String::Concat(String::New(								\
					"Argument "#N" type error. need function, get "),	\
				args[N]->ToString())));									\
			WIRING_RETURN_UNDEFINED();									\
		}																\
	}while(0)


// void xxx(int)
#define WIRING_UNDEFINED_FUNC_INT(NAME) WIRING_NODE_FUNC(NAME) {		\
	HandleScope scope;													\
	WIRING_ARGCHK_LEN(1);												\
	WIRING_ARGCHK_INT(0, 32);											\
	NAME(args[0]->Int32Value());										\
	WIRING_RETURN_UNDEFINED();											\
}

// void xxx(unsigned int)
#define WIRING_UNDEFINED_FUNC_UINT(NAME) WIRING_NODE_FUNC(NAME) {		\
	HandleScope scope;													\
	WIRING_ARGCHK_LEN(1);												\
	WIRING_ARGCHK_UINT(0, 32);											\
	NAME(args[0]->Uint32Value());										\
	WIRING_RETURN_UNDEFINED();											\
}

// void xxx(int, int)
#define WIRING_UNDEFINED_FUNC_INT_INT(NAME) WIRING_NODE_FUNC(NAME) {	\
	HandleScope scope;													\
	WIRING_ARGCHK_LEN(2);												\
	WIRING_ARGCHK_INT(0, 32);											\
	WIRING_ARGCHK_INT(1, 32);											\
	NAME(args[0]->Int32Value(), args[1]->Int32Value());					\
	WIRING_RETURN_UNDEFINED();											\
}

// int xxx(void)
#define WIRING_NUMBER_FUNC_VOID(NAME) WIRING_NODE_FUNC(NAME) {			\
	HandleScope scope;													\
	WIRING_RETURN_NUMBER(NAME());										\
}

// int xxx(int)
#define WIRING_NUMBER_FUNC_INT(NAME) WIRING_NODE_FUNC(NAME) {			\
	HandleScope scope;													\
	WIRING_ARGCHK_LEN(1);												\
	WIRING_ARGCHK_INT(0, 32);											\
	WIRING_RETURN_NUMBER(NAME(args[0]->Int32Value()));					\
}

// int xxx(int, int)
#define WIRING_NUMBER_FUNC_INT_INT(NAME) WIRING_NODE_FUNC(NAME) {		\
	HandleScope scope;													\
	WIRING_ARGCHK_LEN(2);												\
	WIRING_ARGCHK_INT(0, 32);											\
	WIRING_ARGCHK_INT(1, 32);											\
	WIRING_RETURN_NUMBER(												\
		NAME(args[0]->Int32Value(), args[1]->Int32Value()));			\
}

// Function prototypes
//	c++ wrappers thanks to a comment by Nick Lott
//	(and others on the Raspberry Pi forums)

// Basic wiringPi functions

//extern int  wiringPiSetup       (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetup)
//extern int  wiringPiSetupSys    (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetupSys)
//extern int  wiringPiSetupGpio   (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetupGpio)
//extern int  wiringPiSetupPiFace (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetupPiFace)

//extern int  piBoardRev          (void) ;
WIRING_NUMBER_FUNC_VOID(piBoardRev)
//extern int  wpiPinToGpio        (int wpiPin) ;
WIRING_NUMBER_FUNC_INT(wpiPinToGpio)

//extern int  wiringPiSetupPiFaceForGpioProg (void) ;	// Don't use this - for gpio program only

//extern void (*pinMode)           (int pin, int mode) ;
WIRING_UNDEFINED_FUNC_INT_INT(pinMode)
//extern int  (*getAlt)            (int pin) ;
WIRING_NUMBER_FUNC_INT(getAlt)
//extern void (*pullUpDnControl)   (int pin, int pud) ;
WIRING_UNDEFINED_FUNC_INT_INT(pullUpDnControl)
//extern void (*digitalWrite)      (int pin, int value) ;
WIRING_UNDEFINED_FUNC_INT_INT(digitalWrite)
//extern void (*digitalWriteByte)  (int value) ;
WIRING_UNDEFINED_FUNC_INT(digitalWriteByte)
//extern void (*gpioClockSet)      (int pin, int freq) ;
WIRING_UNDEFINED_FUNC_INT_INT(gpioClockSet)
//extern void (*pwmWrite)          (int pin, int value) ;
WIRING_UNDEFINED_FUNC_INT_INT(pwmWrite)
//extern void (*setPadDrive)       (int group, int value) ;
WIRING_UNDEFINED_FUNC_INT_INT(setPadDrive)
//extern int  (*digitalRead)       (int pin) ;
WIRING_NUMBER_FUNC_INT(digitalRead)
//extern void (*pwmSetMode)        (int mode) ;
WIRING_UNDEFINED_FUNC_INT(pwmSetMode)
//extern void (*pwmSetRange)       (unsigned int range) ;
WIRING_UNDEFINED_FUNC_UINT(pwmSetRange)
//extern void (*pwmSetClock)       (int divisor) ;
WIRING_UNDEFINED_FUNC_INT(pwmSetClock)

// Interrupts

//extern int  (*waitForInterrupt) (int pin, int mS) ;
WIRING_NUMBER_FUNC_INT_INT(waitForInterrupt)
/*
#define CALLBACK(N) isrCallback ## N
#define ISR_CALLBACK(N) static void CALLBACK(N)(void){ \
}
ISR_CALLBACK(0)
static void (*isrCallbacks [64])(void) = {
	CALLBACK(0),
};


//extern int  wiringPiISR         (int pin, int mode, void (*function)(void)) ;
WIRING_NODE_FUNC(wiringPiISR) {
	HandleScope scope;
	WIRING_ARGCHK_LEN(3);
	WIRING_ARGCHK_INT(0, 32);
	WIRING_ARGCHK_INT(1, 32);
	WIRING_ARGCHK_FUNC(2);

	int32_t pin = args[0]->Int32Value();
	Local<Function> cb = Local<Function>::Cast(args[2]);
	const unsigned argc = 1;
	Local<Value> argv[argc] = { Local<Value>::New(String::New("hello world")) };
	cb->Call(Context::GetCurrent()->Global(), argc, argv);
	WIRING_RETURN_NUMBER(wiringPiISR(args[0]->Int32Value(), args[1]->Int32Value(), isrCallbacks[pin]));
}
*/

// Threads
//extern int  piThreadCreate (void *(*fn)(void *)) ;
//TODO
//extern void piLock         (int key) ;
WIRING_UNDEFINED_FUNC_INT(piLock)
//extern void piUnlock       (int key) ;
WIRING_UNDEFINED_FUNC_INT(piUnlock)

// Schedulling priority

//extern int piHiPri (int pri) ;
WIRING_NUMBER_FUNC_INT(piHiPri)


// Extras from arduino land

//extern void         delay             (unsigned int howLong) ;
WIRING_UNDEFINED_FUNC_UINT(delay)
//extern void         delayMicroseconds (unsigned int howLong) ;
WIRING_UNDEFINED_FUNC_UINT(delayMicroseconds)
//extern unsigned int millis            (void) ;
WIRING_NUMBER_FUNC_VOID(millis)
//extern unsigned int micros            (void) ;
WIRING_NUMBER_FUNC_VOID(micros)

/*

//extern uint8_t shiftIn  (uint8_t dPin, uint8_t cPin, uint8_t order) ;
WIRING_NODE_FUNC(shiftIn) {
	HandleScope scope;
	WIRING_ARGCHK_LEN(3)
	WIRING_ARGCHK_UINT(0, 32)
	WIRING_ARGCHK_UINT(1, 32)
	WIRING_ARGCHK_UINT(2, 32)
	WIRING_RETURN(Number::New(shiftIn(args[0]->Uint32Value(), args[1]->Uint32Value(), args[2]->Uint32Value())))
}

//extern void    shiftOut (uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val) ;
WIRING_NODE_FUNC(shiftOut) {
	HandleScope scope;
	int dPin, cPin, order;

	Local<Object> bufferObj;
	char *buffer = NULL;
	size_t length = 0, index = 0;

	WIRING_ARGCHK_LEN(4)
	WIRING_ARGCHK_UINT(0, 32)
	WIRING_ARGCHK_UINT(1, 32)
	WIRING_ARGCHK_UINT(2, 32)

	dPin = args[0]->Uint32Value();
	cPin = args[1]->Uint32Value();
	order = args[2]->Uint32Value();

	if (Buffer::HasInstance(args[3])) {
		bufferObj = args[3]->ToObject();
		buffer = Buffer::Data(bufferObj);
		length = Buffer::Length(bufferObj);
		for(index = 0; index < length; index++){
			shiftOut(dPin, cPin, order, buffer[index]);
		}
	}else{
		WIRING_ARGCHK_UINT(3, 32)
		shiftOut(dPin, cPin, order, args[3]->Uint32Value());
	}
	WIRING_RETURN_UNDEFINED
}
*/

/*
// wiringShift.h
//int wiringPiSPIGetFd  (int channel) ;
//int wiringPiSPIDataRW (int channel, unsigned char *data, int len) ;
//
int wiringPiSPIDataTransfer (int channel, unsigned char *write, unsigned char *read, int len)
{
  struct spi_ioc_transfer spi ;
  channel &= 1 ;
  spi.tx_buf        = (unsigned long)write ;
  spi.rx_buf        = (unsigned long)read ;
  spi.len           = len ;
  spi.delay_usecs   = spiDelay ;
  spi.speed_hz      = spiSpeeds [channel] ;
  spi.bits_per_word = spiBPW ;

  return ioctl(spiFds[channel], SPI_IOC_MESSAGE(1), &spi);
}

WIRING_NODE_FUNC(wiringPiSPIDataTransfer) {
	HandleScope scope;
	WIRING_ARGCHK_LEN(2)
	WIRING_ARGCHK_INT(0, 32)

	char *write_buffer = NULL;
	char *read_buffer = NULL;
	size_t write_length = 0;
	size_t read_length = 0;
	size_t length = 0;
	Local<Object> write_buffer_obj;
	Local<Object> read_buffer_obj;

	if (Buffer::HasInstance(args[1])) {
		write_buffer_obj = args[1]->ToObject();
		write_buffer = Buffer::Data(write_buffer_obj);
		write_length = Buffer::Length(write_buffer_obj);
	}
	if (Buffer::HasInstance(args[2])) {
		read_buffer_obj = args[2]->ToObject();
		read_buffer = Buffer::Data(read_buffer_obj);
		read_length = Buffer::Length(read_buffer_obj);
	}
	if (write_length > 0 && read_length > 0) {
		length = MIN(write_length, read_length);
	}else{
		length = MAX(write_length, read_length);
	}
	WIRING_RETURN(Number::New(wiringPiSPIDataTransfer(args[0]->Uint32Value(), write_buffer, read_buffer, length)))
}
//int wiringPiSPISetup  (int channel, int speed) ;
WIRING_NUMBER_FUNC_INT_INT(wiringPiSPISetup)
*/

static void __wiringPiInit(Handle<Object> target) {
	//Setup Functions
	WIRING_BIND_METHOD(wiringPiSetup);
	WIRING_BIND_METHOD(wiringPiSetupGpio);
	WIRING_BIND_METHOD(wiringPiSetupSys);

	//General wiring functions
	WIRING_BIND_METHOD(pinMode);
	WIRING_BIND_METHOD(digitalWrite);
	WIRING_BIND_METHOD(digitalWriteByte);
	WIRING_BIND_METHOD(pwmWrite);
	WIRING_BIND_METHOD(digitalRead);
	WIRING_BIND_METHOD(pullUpDnControl);

	//PWM Control
	WIRING_BIND_METHOD(pwmSetMode);
	WIRING_BIND_METHOD(pwmSetRange);
	WIRING_BIND_METHOD(pwmSetClock);

	//Timing functions
	WIRING_BIND_METHOD(millis);
	WIRING_BIND_METHOD(micros);
	WIRING_BIND_METHOD(delay);
	WIRING_BIND_METHOD(delayMicroseconds);

	//Program/Thread Priority
	WIRING_BIND_METHOD(piHiPri);

	//Interrupts
	WIRING_BIND_METHOD(waitForInterrupt);
	//WIRING_BIND_METHOD(wiringPiISR);

	//Concurrent Processing (multi-threading)
	//WIRING_BIND_METHOD(piThreadCreate);
	WIRING_BIND_METHOD(piLock);
	WIRING_BIND_METHOD(piUnlock);

	//Misc. Functions	
	WIRING_BIND_METHOD(piBoardRev);
	WIRING_BIND_METHOD(wpiPinToGpio);
	WIRING_BIND_METHOD(setPadDrive);
		
	
	//WIRING_BIND_METHOD(wiringPiSetupPiFace);
	//WIRING_BIND_METHOD(getAlt);
	//WIRING_BIND_METHOD(gpioClockSet);
	
	//WIRING_BIND_METHOD(shiftIn);
	//WIRING_BIND_METHOD(shiftOut);
	
	// Handy defines	
	// Deprecated
	WIRING_DEFINE_CONSTANT(NUM_PINS);
	WIRING_DEFINE_CONSTANT(WPI_MODE_PINS);
	WIRING_DEFINE_CONSTANT(WPI_MODE_GPIO);
	WIRING_DEFINE_CONSTANT(WPI_MODE_GPIO_SYS);
	WIRING_DEFINE_CONSTANT(WPI_MODE_PIFACE);
	WIRING_DEFINE_CONSTANT(WPI_MODE_UNINITIALISED);
	
	// Pin modes
	WIRING_DEFINE_CONSTANT(INPUT);
	WIRING_DEFINE_CONSTANT(OUTPUT);
	WIRING_DEFINE_CONSTANT(PWM_OUTPUT);
	WIRING_DEFINE_CONSTANT(GPIO_CLOCK);

	WIRING_DEFINE_CONSTANT(LOW);
	WIRING_DEFINE_CONSTANT(HIGH);
	
	// Pull up/down/none
	WIRING_DEFINE_CONSTANT(PUD_OFF);
	WIRING_DEFINE_CONSTANT(PUD_DOWN);
	WIRING_DEFINE_CONSTANT(PUD_UP);
	
	// PWM
	WIRING_DEFINE_CONSTANT(PWM_MODE_MS);
	WIRING_DEFINE_CONSTANT(PWM_MODE_BAL);
	
	// Interrupt levels
	WIRING_DEFINE_CONSTANT(INT_EDGE_SETUP);
	WIRING_DEFINE_CONSTANT(INT_EDGE_FALLING);
	WIRING_DEFINE_CONSTANT(INT_EDGE_RISING);
	WIRING_DEFINE_CONSTANT(INT_EDGE_BOTH);
}

NODE_MODULE(wiringPi, __wiringPiInit);
