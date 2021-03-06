
#ifndef KIWI_CORE_STATICREADERPORT_DECL_HPP
#define KIWI_CORE_STATICREADERPORT_DECL_HPP

#include "kiwi/mpl/TypeList.hpp"
#include "kiwi/mpl/Tuple.hpp"
#include "kiwi/mpl/MakePointer.hpp"

namespace kiwi{
namespace core{

template<typename TContainerTypeList, typename TCompatibilityPolicy>
class StaticReaderPort{
public:
  typedef kiwi::mpl::AsList<TContainerTypeList>::type ContainerTypeList;
  typedef TCompatibilityPolicy CompatibilityPolicy;
  
  
protected:
  typedef kiwi::mpl::typelist::Transform<ContainerTypeList,kiwi::mpl::MakePointer_2> ContainerPtrTypeList;
  
  kiwi::mpl::tuple::<ContainerPtrTypeList> _toUpdate;
};



}//namespace
}//namespace
s

#endif
