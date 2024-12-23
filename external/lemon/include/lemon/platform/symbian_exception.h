// Exception Handling support header for -*- C++ -*-

// Copyright (C) 1995, 1996, 1997, 1998, 2000, 2001, 2002
// Free Software Foundation
//
// This file is part of GNU CC.
//
// GNU CC is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GNU CC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU CC; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

/** @file exception
 *  This header defines several types and functions relating to the
 *  handling of exceptions in a C++ program.
 */

#ifndef __EXCEPTION__
#define __EXCEPTION__

/*  Copyright (C) 2004 Garrett A. Kajmowicz
	This file is part of the uClibc++ Library.
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __BASIC_DEFINITIONS
#define __BASIC_DEFINITIONS 1

#ifdef __GCC__
#pragma GCC visibility push(default)
#endif

//The following is used to support GCC symbol visibility patch

#ifdef GCC_HASCLASSVISIBILITY
	#define _UCXXEXPORT __attribute__ ((visibility("default")))
	#define _UCXXLOCAL __attribute__ ((visibility("hidden")))
#else
	#define _UCXXEXPORT
	#define _UCXXLOCAL

#endif

#ifdef __GCC__
#define __UCLIBCXX_NORETURN __attribute__ ((__noreturn__))
#else
#define __UCLIBCXX_NORETURN
#endif

#ifdef __UCLIBCXX_HAS_TLS__
	#define __UCLIBCXX_TLS __thread
#else
	#define __UCLIBCXX_TLS
#endif



//Testing purposes
#define __STRING_MAX_UNITS 65535

namespace std{
	typedef signed long int streamsize;
}

#ifdef __GCC__
#pragma GCC visibility pop
#endif

#endif


#ifdef __DODEBUG__
	#define UCLIBCXX_DEBUG 1
#else
	#define UCLIBCXX_DEBUG 0
#endif
extern "C++" {

namespace std
{
  /**
   *  @brief Base class for all library exceptions.
   *
   *  This is the base class for all exceptions thrown by the standard
   *  library, and by certain language expressions.  You are free to derive
   *  your own %exception classes, or use a different hierarchy, or to
   *  throw non-class data (e.g., fundamental types).
   */
  class exception
  {
  public:
    exception() throw() { }
    virtual ~exception() throw();
    /** Returns a C-style character string describing the general cause
     *  of the current error.  */
    virtual const char* what() const throw();
  };

  /** If an %exception is thrown which is not listed in a function's
   *  %exception specification, one of these may be thrown.  */
  class bad_exception : public exception
  {
  public:
    bad_exception() throw() { }
    // This declaration is not useless:
    // http://gcc.gnu.org/onlinedocs/gcc-3.0.2/gcc_6.html#SEC118
    virtual ~bad_exception() throw();
  };

  /// If you write a replacement %terminate handler, it must be of this type.
  typedef void (*terminate_handler) ();
  /// If you write a replacement %unexpected handler, it must be of this type.
  typedef void (*unexpected_handler) ();

  /// Takes a new handler function as an argument, returns the old function.
  terminate_handler set_terminate(terminate_handler) throw();
  /** The runtime will call this function if %exception handling must be
   *  abandoned for any reason.  */
  void terminate() __UCLIBCXX_NORETURN;

  /// Takes a new handler function as an argument, returns the old function.
  unexpected_handler set_unexpected(unexpected_handler) throw();
  /** The runtime will call this function if an %exception is thrown which
   *  violates the function's %exception specification.  */
  void unexpected() __UCLIBCXX_NORETURN;

  /** [18.6.4]/1:  "Returns true after completing evaluation of a
   *  throw-expression until either completing initialization of the
   *  exception-declaration in the matching handler or entering @c unexpected()
   *  due to the throw; or after entering @c terminate() for any reason
   *  other than an explicit call to @c terminate().  [Note: This includes
   *  stack unwinding [15.2].  end note]"
   *
   *  2:  "When @c uncaught_exception() is true, throwing an %exception can
   *  result in a call of @c terminate() (15.5.1)."
   */
//  bool uncaught_exception() throw();
} // namespace std

namespace __gnu_cxx
{
  /** A replacement for the standard terminate_handler which prints more
      information about the terminating exception (if any) on stderr.  Call
      @code
        std::set_terminate (__gnu_cxx::__verbose_terminate_handler)
      @endcode
      to use.  For more info, see
      http://gcc.gnu.org/onlinedocs/libstdc++/19_diagnostics/howto.html#4
  */
  void __verbose_terminate_handler ();
} // namespace __gnu_cxx

} // extern "C++"

#endif

