#pragma warning(disable:4099)	// class/struct mixups
#pragma warning(disable:4251)	// needs to have dll-interface
#pragma warning(disable:4291)	// no matching op. delete (there are)
#pragma warning(disable:4355)	// 'this' used in base member initializer list
#pragma warning(disable:4786)	// debug identifiers are truncated
#pragma warning(disable:4800)	// forcing value to bool 'true' or 'false' (performance warning)

// New warnings at level W4

#pragma warning(disable:4018)  // signed/unsigned mismatch
#pragma warning(disable:4100)  // unreferenced formal parameter
#pragma warning(disable:4127)  // conditional expression is constant
#pragma warning(disable:4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable:4189)  // local variable is initialized but not referenced
#pragma warning(disable:4211)  // nonstandard extension used : redefined extern to static
#pragma warning(disable:4214)  // nonstandard extension used : bit field types other than int
#pragma warning(disable:4244)  // conversion from '.......' to '......', possible loss of data
#pragma warning(disable:4245)  // conversion from '.......' to '......', signed/unsigned mismatch
#pragma warning(disable:4291)  // no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning(disable:4309)  // truncation of constant value
#pragma warning(disable:4310)  // cast truncates constant value
#pragma warning(disable:4355)  // '....' used in base member initializer list
#pragma warning(disable:4505)  // unreferenced local function has been removed
#pragma warning(disable:4511)  // copy constructor could not be generated
#pragma warning(disable:4512)  // assignment operator could not be generated
#pragma warning(disable:4663)  // to explicitly specialize class template '.....' use the following syntax
#pragma warning(disable:4701)  // local variable '......' may be used without having been initialized
#pragma warning(disable:4702)  // unreachable code
#pragma warning(disable:4706)  // assignment within conditional expression TO BE REMOVED
#pragma warning(disable:4709)  // comma operator within array index expression
#pragma warning(disable:4710)  // function '.....' not inlined
#pragma warning(disable:4711)  // function '.....' selected for automatic inline expansion

// Warning from Microsoft Visual Studio\VC98\include\fstream

#pragma warning(disable:4097)  // typedef-name '......' used as synonym for class-name '.....'