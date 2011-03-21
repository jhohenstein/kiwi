
#pragma once
#ifndef KIWI_CORE_TEMPLATEDATAPORT_HPP
#define KIWI_CORE_TEMPLATEDATAPORT_HPP

#include "kiwi/core/DataPort.hpp"

namespace kiwi{
namespace core{

template<class TContainerType = kiwi::core::Container, int TFlag = 3>
class StaticDataPort : public DataPort
{
public:
  typedef TContainerType ContainerType;
  enum { READ = 1, WRITE = 2, READ_WRITE = READ || WRITE, Flag = TFlag };

  StaticDataPort() : DataPort(0) {}
  
  void setNode( kiwi::core::Node* const node){ _node = node; }

  StaticDataPort(Node* myNode, ContainerType* data = 0)
  : DataPort( myNode ){_container = data;}

  virtual Container* getAbstractContainer() const {
    return _container;
  }

  ContainerType* getContainer() const {
    return _container;
  }

  int flag() const { return TFlag; }

  virtual bool isEmpty() const{
    return _container;
  }

  virtual bool setAbstractContainer( Container* data ){
    _container = dynamic_cast<ContainerType*>(data);
    return _container != 0;
  }
  
  virtual bool setContainer( ContainerType* data ){
    _container = data;
  }

protected:

  ContainerType* _container;
};


}//namespace
}//namespace


#endif
