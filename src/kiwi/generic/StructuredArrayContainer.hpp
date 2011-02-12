// Copyright (c) 2010 Nicolas Silva
// All rights reserved.
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

#ifndef KIWI_COMPOSITEARRAYCONTAINER_HPP
#define KIWI_COMPOSITEARRAYCONTAINER_HPP

#include "kiwi/generic/ArrayContainer.hpp"
#include <vector>

namespace kiwi{
namespace generic{


template<typename TValueType, kiwi::uint32_t TDimension>
class StructuredArrayContainer 
	: public ArrayContainer<TValueType, TDimension>
{
public:
	typedef TValueType ValueType;
	static const kiwi::uint32_t Dimension = TDimension;
	typedef ArrayContainer<TValueType, TDimension> MotherClass;
	typedef typename MotherClass::CoordinateVector CoordinateVector;
	typedef typename MotherClass::StrideVector StrideVector;




	/**
	 * @brief Constructor.
	 */ 
	StructuredArrayContainer(
			const CoordinateVector& perArraySize
			, const kiwi::string& description )
	{
		init(0, perArraySize, description );
	}

	/**
	 *	@brief Destructor. 
	 */ 
	~StructuredArrayContainer()
	{
		for(kiwi::uint32_t i = 0; i < _subContainers.size(); ++i){
			delete _subContainers[i];
		}
	}
	
	virtual bool isComposite() const {return true;}
	
	virtual kiwi::uint32_t nbSubContainers() const {
		return _subContainers.size();
	}

	virtual core::Container* subContainer(kiwi::uint32_t index){
		if(index < nbSubContainers() ){
			return _subContainers[index];
		}else{ return 0; }
	}

protected:
	bool init(ValueType* dataPtr
			, CoordinateVector perArraySize
			, const kiwi::string& description )
	{
	ScopedBlockMacro(___, "StructuredArrayContainer::init")

	// setup the main container's size
	

	kiwi::uint32_t nbArrays = 1;
	for(kiwi::uint32_t i = 0; i < description.size(); ++i){
			if((description[i] == '|') || (description[i] == '%')){
				++nbArrays;
			}
		}

	
	kiwi::uint32_t perArrayTotalSize = 1;
	for(kiwi::uint32_t i = 0; i < Dimension; ++i)
		perArrayTotalSize *= perArraySize(i);
	MotherClass::_totalSize = perArrayTotalSize * nbArrays;	
	// if we have to allocate the data
	if(!dataPtr){
		MotherClass::_data = new ValueType[nbArrays*MotherClass::_totalSize];
		MotherClass::_deleteDataDestructor = true;
	}else{
		MotherClass::_deleteDataDestructor = false;
	}

	// Note: this may change. The thing is that having noninterleaved and
	// interleaved arrays in the same block makes it hard to find a regular way
	// to browse through the block using increments and taking dimensions into
	// account.
	// The actual implementation works best when the structure is all "interleaved"
	MotherClass::_stride = size2stride(perArraySize, nbArrays);
	MotherClass::_stride(0) = 1;

	MotherClass::_spanSize = perArraySize;
	MotherClass::_spanSize(0) = perArraySize(0)*nbArrays;
	
	kiwi::uint32_t count = 0;
	kiwi::uint32_t pos = 0;
	kiwi::uint32_t nthArray = 0;
	kiwi::char_t lastSymbol = '|';
		while( pos < description.size() ){
			Debug::print() << "while...\n";
			if(description[pos] == '|'){
				Debug::print() << "found '|'\n";
				if(lastSymbol == '|'){
					Debug::print() << "adding classic array\n";
					_subContainers.push_back(
						new ArrayContainer<ValueType,Dimension>(
							MotherClass::_data + nthArray * perArrayTotalSize
							, perArraySize
							, size2stride(perArraySize, 1) )
					);
					++nthArray;
				}
				lastSymbol = '|';
			}else if(description[pos] == '%'){
				Debug::print() << "found '%'\n";
				lastSymbol = '%';
				kiwi::uint32_t interleavedCount = 1;
				kiwi::uint32_t pos2 = 0;
				do{
					++pos2;
					if(description[pos+pos2] == '%'){
						Debug::print() << "found '%'\n";
						++interleavedCount;
					}
				}while( (description[pos+pos2] != '|')
					&&(pos+pos2 < description.size()) );

				for(kiwi::uint32_t i = 0; i <= interleavedCount; ++i){
					Debug::print() << "adding interleaved array\n";
					_subContainers.push_back(
						new ArrayContainer<ValueType,Dimension>(
							MotherClass::_data + nthArray * perArrayTotalSize + i
							, perArraySize
							, size2stride( perArraySize, interleavedCount+1))
					);
				}
				pos += pos2-1;
				nthArray += interleavedCount+1;
			}
			++pos;
		}
		if(lastSymbol == '|'){
			Debug::print() << "adding classic array\n";
			_subContainers.push_back(
				new ArrayContainer<ValueType,Dimension>(
					MotherClass::_data + nthArray * perArrayTotalSize
					, perArraySize
					, size2stride( perArraySize, 1) )
			);
		}else if(lastSymbol == '%'){

		}
	}

	StrideVector size2stride(const CoordinateVector& size, kiwi::uint32_t nbInteleavedArrays = 1)
	{
		ScopedBlockMacro(____, "StructuredArrayContainer::size2stride")
		StrideVector result(nbInteleavedArrays);
		for(kiwi::uint32_t i = 1; i < Dimension; ++i)
		{
			result(i) = result(i-1) * size(i-1);
		}
		return result;
	}

private:
	std::vector<ArrayContainer<ValueType,Dimension>*> _subContainers;
};



}//namespace
}//namespace


#endif