#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <wiringPi.h>
#include <wiringShift.h>

using namespace v8;
using namespace node;

#define WIRING_FUNC(NAME) _wiring_##NAME
#define WIRING_DEFINE_CONSTANT(NAME, VALUE) (target)->Set( \
		v8::String::NewSymbol(NAME), \
		v8::Integer::New(VALUE), \
		static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete) \
);
#define WIRING_BIND_METHOD(NAME) NODE_SET_METHOD(target, #NAME, WIRING_FUNC(NAME));
#define WIRING_WRAP_FUNC(NAME) static Handle<Value> WIRING_FUNC(NAME) (const Arguments& args)
#define WIRING_RETURN(RET) return scope.Close(RET);
#define WIRING_RETURN_UNDEFINED WIRING_RETURN(Undefined())
#define WIRING_ARGCHK_LEN(N) if (args.Length() < N) { \
	ThrowException(Exception::TypeError(String::New("Need "#N" arguments."))); \
	WIRING_RETURN_UNDEFINED \
}
#define WIRING_ARGCHK_INT(N,BIT) if (!args[N]->IsNumber() || !args[N]->IsInt##BIT()) { \
	ThrowException(Exception::TypeError(String::Concat(String::New("Argument "#N" type error. need int"#BIT", get "), args[N]->ToString()))); \
	WIRING_RETURN_UNDEFINED \
}
#define WIRING_ARGCHK_UINT(N,BIT) if (!args[N]->IsNumber() || !args[N]->IsUint##BIT()) { \
	ThrowException(Exception::TypeError(String::Concat(String::New("Argument "#N" type error. need uint"#BIT", get "), args[N]->ToString()))); \
	WIRING_RETURN_UNDEFINED \
}
#define WIRING_NUMBER_FUNC_VOID(NAME) WIRING_WRAP_FUNC(NAME) { \
	HandleScope scope; \
	WIRING_RETURN(Number::New(NAME())) \
}
#define WIRING_NUMBER_FUNC_INT(NAME) WIRING_WRAP_FUNC(NAME) { \
	HandleScope scope; \
	WIRING_ARGCHK_LEN(1) \
	WIRING_ARGCHK_INT(0, 32) \
	WIRING_RETURN(Number::New(NAME(args[0]->Int32Value()))) \
}
#define WIRING_UNDEFINED_FUNC_INT(NAME) WIRING_WRAP_FUNC(NAME) { \
	HandleScope scope; \
	WIRING_ARGCHK_LEN(1) \
	WIRING_ARGCHK_INT(0, 32) \
	NAME(args[0]->Int32Value()); \
	WIRING_RETURN_UNDEFINED \
}
#define WIRING_UNDEFINED_FUNC_UINT(NAME) WIRING_WRAP_FUNC(NAME) { \
	HandleScope scope; \
	WIRING_ARGCHK_LEN(1) \
	WIRING_ARGCHK_UINT(0, 32) \
	NAME(args[0]->Uint32Value()); \
	WIRING_RETURN_UNDEFINED \
}
#define WIRING_UNDEFINED_FUNC_INT_INT(NAME) WIRING_WRAP_FUNC(NAME) { \
	HandleScope scope; \
	WIRING_ARGCHK_LEN(2) \
	WIRING_ARGCHK_INT(0, 32) \
	WIRING_ARGCHK_INT(1, 32) \
	NAME(args[0]->Int32Value(), args[1]->Int32Value()); \
	WIRING_RETURN_UNDEFINED \
}
#define WIRING_NUMBER_FUNC_INT_INT(NAME) WIRING_WRAP_FUNC(NAME) { \
	HandleScope scope; \
	WIRING_ARGCHK_LEN(2) \
	WIRING_ARGCHK_INT(0, 32) \
	WIRING_ARGCHK_INT(1, 32) \
	WIRING_RETURN(Number::New(NAME(args[0]->Int32Value(), args[1]->Int32Value()))) \
}

// wiringPi.h:
// Basic wiringPi functions

//extern int  wiringPiSetup       (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetup)
//extern int  wiringPiSetupSys    (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetupSys)
//extern int  wiringPiSetupGpio   (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetupGpio)
//extern int  wiringPiSetupPiFace (void) ;
WIRING_NUMBER_FUNC_VOID(wiringPiSetupPiFace)

//extern int  wiringPiSetupPiFaceForGpioProg (void) ;	// Don't use this - for gpio program only

//extern void (*pinMode)           (int pin, int mode) ;
WIRING_UNDEFINED_FUNC_INT_INT(pinMode)
//extern void (*pullUpDnControl)   (int pin, int pud) ;
WIRING_UNDEFINED_FUNC_INT_INT(pullUpDnControl)
//extern void (*digitalWrite)      (int pin, int value) ;
WIRING_UNDEFINED_FUNC_INT_INT(digitalWrite)
//extern void (*pwmWrite)          (int pin, int value) ;
WIRING_UNDEFINED_FUNC_INT_INT(pwmWrite)
//extern void (*setPadDrive)       (int group, int value) ;
WIRING_UNDEFINED_FUNC_INT_INT(setPadDrive)
//extern int  (*digitalRead)       (int pin) ;
WIRING_NUMBER_FUNC_INT(digitalRead)
//extern void (*delayMicroseconds) (unsigned int howLong) ;
WIRING_UNDEFINED_FUNC_UINT(delayMicroseconds)
//extern void (*pwmSetMode)        (int mode) ;
WIRING_UNDEFINED_FUNC_INT(pwmSetMode)
//extern void (*pwmSetRange)       (unsigned int range) ;
WIRING_UNDEFINED_FUNC_UINT(pwmSetRange)
//
//// Interrupts
//
//extern int  (*waitForInterrupt) (int pin, int mS) ;
WIRING_UNDEFINED_FUNC_INT_INT(waitForInterrupt)
//
//// Threads
//
//#define	PI_THREAD(X)	void *X (void *dummy)
//
//extern int  piThreadCreate (void *(*fn)(void *)) ;
//extern void piLock         (int key) ;
WIRING_UNDEFINED_FUNC_INT(piLock)
//extern void piUnlock       (int key) ;
WIRING_UNDEFINED_FUNC_INT(piUnlock)
//
//// Schedulling priority
//
//extern int piHiPri (int pri) ;
WIRING_NUMBER_FUNC_INT(piHiPri)
//
//
//// Extras from arduino land
//
//extern void         delay             (unsigned int howLong) ;
WIRING_UNDEFINED_FUNC_UINT(delay)
//extern unsigned int millis            (void) ;
WIRING_NUMBER_FUNC_VOID(millis)

// wiringShift.h:

//extern uint8_t shiftIn  (uint8_t dPin, uint8_t cPin, uint8_t order) ;
WIRING_WRAP_FUNC(shiftIn) {
	HandleScope scope;
	WIRING_ARGCHK_LEN(3)
	WIRING_ARGCHK_UINT(0, 32)
	WIRING_ARGCHK_UINT(1, 32)
	WIRING_ARGCHK_UINT(2, 32)
	WIRING_RETURN(Number::New(shiftIn(args[0]->Uint32Value(), args[1]->Uint32Value(), args[2]->Uint32Value())))
}

//extern void    shiftOut (uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val) ;
WIRING_WRAP_FUNC(shiftOut) {
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

WIRING_WRAP_FUNC(wiringPiSPIDataTransfer) {
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

void wiringPiInit(Handle<Object> target) {
	WIRING_BIND_METHOD(wiringPiSetup)
	WIRING_BIND_METHOD(wiringPiSetupSys)
	WIRING_BIND_METHOD(wiringPiSetupGpio)
	WIRING_BIND_METHOD(wiringPiSetupPiFace)
	WIRING_BIND_METHOD(pinMode)
	WIRING_BIND_METHOD(pullUpDnControl)
	WIRING_BIND_METHOD(digitalWrite)
	WIRING_BIND_METHOD(pwmWrite)
	WIRING_BIND_METHOD(setPadDrive)
	WIRING_BIND_METHOD(digitalRead)
	WIRING_BIND_METHOD(delayMicroseconds)
	WIRING_BIND_METHOD(pwmSetMode)
	WIRING_BIND_METHOD(pwmSetRange)
	WIRING_BIND_METHOD(waitForInterrupt)
	WIRING_BIND_METHOD(piLock)
	WIRING_BIND_METHOD(piUnlock)
	WIRING_BIND_METHOD(piHiPri)
	WIRING_BIND_METHOD(delay)
	WIRING_BIND_METHOD(millis)
	WIRING_BIND_METHOD(shiftIn)
	WIRING_BIND_METHOD(shiftOut)
	WIRING_DEFINE_CONSTANT("INPUT", INPUT)
	WIRING_DEFINE_CONSTANT("OUTPUT", OUTPUT)
	WIRING_DEFINE_CONSTANT("PWM_OUTPUT", PWM_OUTPUT)
	WIRING_DEFINE_CONSTANT("LOW", LOW)
	WIRING_DEFINE_CONSTANT("HIGH", HIGH)
	WIRING_DEFINE_CONSTANT("PUD_OFF", PUD_OFF)
	WIRING_DEFINE_CONSTANT("PUD_DOWN", PUD_DOWN)
	WIRING_DEFINE_CONSTANT("PUD_UP", PUD_UP)
	WIRING_DEFINE_CONSTANT("LSBFIRST", LSBFIRST)
	WIRING_DEFINE_CONSTANT("MSBFIRST", MSBFIRST)
}

NODE_MODULE(wiringpi, wiringPiInit);
