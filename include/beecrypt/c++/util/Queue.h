/*
 * Copyright (c) 2005 X-Way Rights BV
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

/*!\file Queue.h
 * \ingroup CXX_UTIL_m
 */

#ifndef _INTERFACE_BEE_UTIL_QUEUE_H
#define _INTERFACE_BEE_UTIL_QUEUE_H

#ifdef __cplusplus

#include "beecrypt/c++/util/Collection.h"
using beecrypt::util::Collection;

namespace beecrypt {
	namespace util {
		/*!\ingroup CXX_UTIL_m
		 */
		template<class E> class Queue : public virtual Collection<E>
		{
		public:
			virtual E* element() = 0;
			virtual bool offer(E*) = 0;
			virtual E* peek() = 0;
			virtual E* poll() = 0;
			virtual E* remove() = 0;
		};
	}
}

#endif

#endif
