/*
 * Copyright (c) 2004 X-Way Rights BV
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*!\file IOException.h
 * \ingroup CXX_IO_m
 */

#ifndef _CLASS_BEE_IO_IOEXCEPTION_H
#define _CLASS_BEE_IO_IOEXCEPTION_H

#ifdef __cplusplus

#include "beecrypt/c++/lang/Exception.h"
using beecrypt::lang::Exception;
using beecrypt::lang::String;

namespace beecrypt {
	namespace io {
		/*!\ingroup CXX_IO_m
		 */
		class IOException : public Exception
		{
		public:
			inline IOException() {}
			inline IOException(const char* message) : Exception(message) {}
			inline IOException(const String& message) : Exception(message) {}
			inline ~IOException() {}
		};
	}
}

#endif

#endif
