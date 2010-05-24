
//metadoc Common copyright Steve Dekorte 2002
//metadoc Common license BSD revised
/*metadoc Common description
You may need to add an entry for your C compiler.
*/

/*
Trick to get inlining to work with various compilers
Kudos to Daniel A. Koepke
*/

#undef IO_DECLARE_INLINES
#undef IOINLINE

/*
#if defined(__cplusplus)
	#ifdef IO_IN_C_FILE
	#else
		#define IO_DECLARE_INLINES
		#define IOINLINE extern inline
	#endif
#else
*/

#if defined(__APPLE__)
	#include "TargetConditionals.h"
#endif

#if defined __XCODE__ && (TARGET_ASPEN_SIMULATOR || TARGET_OS_ASPEN)
	#define NON_EXTERN_INLINES
#else
	#if defined __GNUC__ && __GNUC__ >= 4
		//#define NON_EXTERN_INLINES
	#endif
#endif

#ifdef NON_EXTERN_INLINES

#ifdef IO_IN_C_FILE
	// in .c 
	#define IO_DECLARE_INLINES
	#define IOINLINE 
#else
	// in .h 
	#define IO_DECLARE_INLINES
	#define IOINLINE inline
#endif 

#else

#ifdef IO_IN_C_FILE
	// in .c 
	#define IO_DECLARE_INLINES
	#define IOINLINE inline
#else

	// in .h 
	#define IO_DECLARE_INLINES
	#define IOINLINE extern inline
	
#endif 

#endif

/*
#endif
*/

/*  
	The following code is needed on Windows with mingw to avoid a duplicate function definition error on inline functions.
	
	It happens when including the Io headers in a 3rd party program and statically linking libiovmall.a.
	(This problem does not occur when linking the Io DLL, only when static linking.  However, considering
	that Io's default way of registering protos is also broken on mingw if using a DLL, static linking is desirable.)
	
	The problem is that GCC creates a function body for the inline functions in the static library libiovmall.a,
	even though the function is normally used inline, and it also independently creates a function body in the object files
	of any 3rd party program include the Io headers.  When you link the 3rd party program with the Io library, GCC
	finds the two function bodies (that it created without your consent and that you never wanted)
	and then it complains of a duplicate function definition, even though they were supposed to be inline.
	See: http://stackoverflow.com/questions/2217628/multiple-definition-of-inline-functions-when-linking-static-libs
	
	It does this to support some crap C standard change - that is, it used to work one way, and in the next C
	standard they changed extern inline to work completely the opposite of the way it worked before.
		
	Strangely Io's use of inline SHOULD FAIL ON LINUX TOO according to this same standard, but GCC makes 
	special allowances because the Linux kernel abuses inline in the same way, and so GCC has a hack that allows this 
	on Linux, but not on Windows.  Reference:  http://lkml.indiana.edu/hypermail/linux/kernel/0408.0/1787.html
		
	Despite nearly a year of off and on experimentation and investigation, I have not been able to figure 
	out any single definition of IOINLINE that works on mingw for both Io code proper and for a third party program
	using the Io headers, even when using IO_IN_C_FILE to disable the change.  The only solution I've been able to come up with is
	to provide a mechanism to allow a 3rd party program to blow away the existing IOINLINE definition and replace it
	with one that will work for including the Io headers.  To use this, simply "#define REPLACE_IOINLINE" before including
	and Io headers in your 3rd party program.
	
	--Dennis Ferron
*/
#ifdef REPLACE_IOINLINE
#undef IOINLINE
#define IOINLINE __attribute__((always_inline)) inline
#endif
