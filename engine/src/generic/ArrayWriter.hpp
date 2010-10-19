//      Redistribution and use in source and binary forms, with or without
//      modification, are permitted provided that the following conditions are
//      met:
//      
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following disclaimer
//        in the documentation and/or other materials provided with the
//        distribution.
//      * Neither the name of the  nor the names of its
//        contributors may be used to endorse or promote products derived from
//        this software without specific prior written permission.
//      
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#pragma once

#ifndef KIWI_ARRAYWRITER_HPP
#define KIWI_ARRAYWRITER_HPP

#include "core/Resource.hpp"
#include "generic/ArrayContainer.hpp"


namespace kiwi
{
namespace generic
{
	
	
template < typename TValueType, unsigned int TDimension>
class ArrayWriter : public core::Writer
{
public:
	typedef TValueType ValueType;
	typedef Point<unsigned int, TDimension+1> IncsType;
	typedef Point<unsigned int, TDimension> Coordinates;
	
	// -----------------------------------------------------------------
	/**
	 * @brief Constructor.
	 */ 
	ArrayWriter(const core::Resource::WriterInputPort& port);
	
	/**
	 * @brief Basic access method.
	 */ 
	ValueType get(const Point<int, TDimension>& coords) const;	
	/**
	 * @brief Basic access method.
	 */ 
	ValueType get(unsigned int i) const;
	
	/**
	 * @brief Basic access method.
	 */ 
	void set(const Point<int, TDimension>& coords, ValueType value);
	/**
	 * @brief Basic access method.
	 */ 
	void set(unsigned int i, ValueType value);
	
	/**
	 * @brief Unsafe yet faster access method.
	 */ 
	inline const ValueType* getDataPointer() const { return _data; } 	// TODO: const stuff
	
	/**
	 * @brief Returns an iterator to the beguinning of the data.
	 */ 
	ArrayIterator<ValueType> getIterator() const
	{
		unsigned size = 1;
		for(unsigned i = 0; i < TDimension; ++i) size *= _span(i);
		debug.print() << "Writer::chan size :  " << size*_incs(0)-1 << endl();
		return ArrayIterator<ValueType>( _data, _data + size*_incs(0)-1, _incs(0) );
	}
	
protected:
 ValueType* _data;
 ValueType* _end;
 IncsType _incs;
 portIndex_t _port;
 Coordinates _span;
};


}	
} // namespace


#include "generic/ArrayWriter.ih"


#endif