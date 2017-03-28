/**
 * The following code is modified from Zed A. Shaw's "Awesome Debug Macros"
 * and can be found at http://c.learncodethehardway.org/book/ex20.html.
 *
 * By default, both logs and debug outputs are disabled. To enable them, place
 *
 *		#define DOLOG
 *			and/or
 *		#define DEBUG
 *
 * before the #include "dbg.h" in the necessary files.
 */
#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

/*
 * If DOLOG is defined, print log and debug messages.
 *
 * The log_err, log_warn, and log_info macros each provide meaningful output
 * including the filename and linenumber they were called from, as well as the
 * provided message.
 *
 * For log_err and log_warn, the message will also include a clean
 * representation of errno, since these would be used in situations following
 * a failed function call or other error-provoking event.
 *
 * Note that these macros are only meaningful if the DOLOG flag macro is
 * defined. This can be useful when log messages clutter the output, and should
 * be suppressed in production.
 */
#ifdef DOLOG
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define log_err(M, ...)
#define log_warn(M, ...)
#define log_info(M, ...)
#endif

/*
 * If TEST is defined, print test messages.
 */
#ifdef TEST
#define test_pass(M, ...) fprintf(stderr, "[TEST PASS] (%s:%d) " M "\n", __FILE__, __LINE__)
#define test_fail(M, ...) fprintf(stderr, "[TEST FAIL] (%s:%d) " M "\n", __FILE__, __LINE__)
#else
#define test_pass(M, ...) (1)
#define test_fail(M, ...) (0)
#endif

/*
 * If DEBUG is defined, print debug messages.
 *
 * Similar to the log macros, but toggles on a different #define switch
 * (DEBUG). The debug macro should be used for frequent, verbose reports about
 * the status of the program, and should only be enabled to fix a bug or gain
 * insight to the inner workings of the progam.
 */
#ifdef DEBUG
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

/*
 * The check macros are for placing safeguards and test-cases inline with
 * functional code. These are useful for identifying when a change in code
 * in one place can cause errors in code that previously worked just fine.
 *
 * check(A, M, ...)
 *		Ensure that condition A is true. If not, print error message M
 *		and jump to the "error:" tag.
 *
 * sentinel(M, ...)
 *		Sentinels should be placed in blocks of code that the flow should never
 *		reach or is never expected to reach. If sentinel does get called, it
 *		immediately prints error message M and jumps to the "error:" tag.
 *
 * check_mem(A)
 *		Use after initializing a pointer with malloc(). If the value of pointer
 *		A is 0, check_mem prints an error message and jumps to the "error:" tag.
 *
 * check_debug(A, M, ...)
 *		Behaves the same way as check, but prints on the debug macro instead
 *		of log_err and does not jump to the "error:" tag.
 *
 * assert(A, M, ...)
 *		Evaluates that condition A is true. If it is, print [TEST PASS] plus
 *		the test name M. If condition A is false, print [TEST FAIL] and the
 *		test name M. Upon a test pass, assert() evaluates to 1, upon a fail
 *		it evaluates to 0.
 *
 * Note that the use of check, sentinel, and check_mem all require that an
 * "error:" jump tag be defined within the scope of each function in which
 * they're used. This can be used to return an error code or NULL, as necessary
 * per function.
 *
 *		int my_function() {
 *			//Function code
 *			check(some_status == 0, "That's not right!");
 *			//More function code
 *			return 0; //The function successfully exited!
 *		error:
 *			return 1; //A check function failed. Return an error status.
 *		}
 */
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
#define check_mem(A) check((A), "Out of memory.")
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }
#define assert(A, M, ...) (A ? ((test_pass(M, ##__VA_ARGS__) & 0) | 1) : (test_fail(M, ##__VA_ARGS__) & 0))

#endif
