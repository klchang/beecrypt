/*
 * Copyright (c) 2004, 2005 X-Way Rights BV
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

/*!\file Properties.h
 * \ingroup CXX_UTIL_m
 */

#ifndef _CLASS_PROPERTIES_H
#define _CLASS_PROPERTIES_H

#ifdef __cplusplus

#include "beecrypt/c++/io/InputStream.h"
using beecrypt::io::InputStream;
#include "beecrypt/c++/io/OutputStream.h"
using beecrypt::io::OutputStream;
#include "beecrypt/c++/lang/String.h"
using beecrypt::lang::String;
#include "beecrypt/c++/util/Enumeration.h"
using beecrypt::util::Enumeration;
#include "beecrypt/c++/util/Hashtable.h"
using beecrypt::util::Hashtable;

namespace beecrypt {
	namespace util {
		/*!\ingroup CXX_UTIL_m
		 */
		class BEECRYPTCXXAPI Properties : public Hashtable<Object,Object>
		{
		private:
			class Names : public Object, public virtual Enumeration<const String>
			{
			private:
				array<String*> _list;
				jint _pos;

			public:
				Names(Hashtable<Object,Object>& h);
				virtual ~Names();

				virtual bool hasMoreElements() throw ();
				virtual const String* nextElement() throw (NoSuchElementException);
			};

			void enumerate(Hashtable<Object,Object>& h) const;

		protected:
			const Properties* defaults;

		public:
			Properties();
			Properties(const Properties& copy);
			Properties(const Properties* defaults);
			virtual ~Properties() {}

			const String* getProperty(const String& key) const throw ();
			const String* getProperty(const String& key, const String& defaultValue) const throw ();

			/*!\warning If this method returns a non-null value, make sure
			 *  you pass it to beecrypt::lang::collection_rcheck(), which will delete
			 *  it if it's no longer attached to any other collection.
			 */
			Object* setProperty(const String& key, const String& value);

			Enumeration<const String>* propertyNames() const;

			void load(InputStream& in) throw (IOException);
			/*!\todo rewrite this method using an OutputWriter after obtaining all
			 *  keys through the enumerate method
			 */
			void store(OutputStream& out, const String& header) throw (IOException);
		};
	}
}

#endif

#endif
