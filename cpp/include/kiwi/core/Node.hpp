#pragma once
#ifndef KIWI_CORE_NODE_HPP
#define KIWI_CORE_NODE_HPP

#include <vector>

#include "kiwi/core/Pipeline.hpp"
#include "kiwi/core/Commons.hpp"
#include "kiwi/extern/log/DebugStream.hpp"
#include "kiwi/core/Data.hpp"
#include "kiwi/core/Connect.hpp"
#include "kiwi/core/Blob.hpp"

namespace kiwi{ namespace view{ class NodeView; }}

namespace kiwi{
namespace core{

class InputPort;
class OutputPort;
class NodeUpdater;
class NodeTypeInfo;

/**
 * Composant of a scripting graph (or pipeline).
 *
 * Nodes are connected to one another through input and output ports.
 * To create a node its type must first be registered.
 * Use NodeTypeManager to create nodes.
 */ 
class Node
{
friend class kiwi::core::Pipeline;
friend bool kiwi::core::protocol::Connect(OutputPort&,InputPort&);
friend bool kiwi::core::protocol::Disconnect(OutputPort&,InputPort&);
public:
    typedef std::vector<InputPort*> InputArray;
    typedef std::vector<OutputPort*> OutputArray;
    typedef uint32 ID;
    typedef std::vector<Node*> NodeArray;
    typedef Blob Storage;

    /**
     * Constructor.
     */ 
    Node(Pipeline* pipeline, const NodeTypeInfo* type);

    /**
     * Returns the input ports as a vector.
     */ 
    const InputArray& inputs() const
    {
        return _inputs;
    }
    
    /**
     * Returns the output ports as a vector.
     */ 
    const OutputArray& outputs() const
    {
        return _outputs;
    }

    /**
     * Returns the ith input port.
     */ 
    InputPort& input(uint32 i = 0)
    {
        return *_inputs[i];
    }

    /**
     * Returns the ith input port (const).
     */ 
    const InputPort& input(uint32 i = 0) const
    {
        return *_inputs[i];
    }

    /**
     * Returns the ith output port.
     */ 
    OutputPort& output(uint32 i = 0)
    {
        return *_outputs[i];
    }

    /**
     * Returns the ith output port (const).
     */ 
    const OutputPort& output(uint32 i = 0) const
    {
        return *_outputs[i];
    }

    const InputPort& input( string portName ) const;
    const OutputPort& output( string portName ) const;
    string inputName( uint32 i ) const;
    string outputName( uint32 i ) const;

    uint32 indexOf( const InputPort* p );
    uint32 indexOf( const OutputPort* p );

    /**
     * Returns the set of nodes connected to this ones' outputs as a vector.
     */ 
    const NodeArray& previousNodes() const
    {
        return _previousNodes;
    }

    /**
     * Returns the set of nodes connected to this ones' inputs as a vector.
     */ 
    const NodeArray& nextNodes() const
    {
        return _nextNodes;
    }

    /**
     * Returns this node's runtime type info.
     */ 
    const NodeTypeInfo * type() const
    {
        return _type;
    }

    /**
     * Returns a pointer to the pipeline this node belongs to.
     */ 
    Pipeline* pipeline() const
    {
        return _pipeline;
    }

    /**
     * Returns this node's unique id.
     */ 
    ID id() const
    {
        return _id;
    }

    Storage& storage()
    {
        return _storage;
    }

    const Storage& storage() const
    {
        return _storage;
    }

    bool hasStorage() const
    {
        return _storage.isAllocated();
    }

    view::NodeView * view() const
    {
        return _view;
    }

    void setView( view::NodeView* v );

    /**
     * Updates the node if its type has a NodeUpdater component.
     *
     * This is when the algorithm carried by the node (if any) is executed.
     */ 
    bool update();
    void init();
    
private:
    static ID _newId()
    {
        static ID nextId = 0;
        return ++nextId;
    }
protected:
    void inputConnected(InputPort* port, OutputPort* to); // TODO: remove these?
    void inputDisconnected(InputPort* port, OutputPort* to);
    void outputConnected(OutputPort* port, InputPort* from);
    void outputDisconnected(OutputPort* port, InputPort* from);
    
    Pipeline* _pipeline;
private:
    InputArray  _inputs;
    OutputArray _outputs;
    NodeArray   _previousNodes;
    NodeArray   _nextNodes;
    Storage     _storage;
    view::NodeView*   _view;
    const NodeTypeInfo* _type;
    ID _id;
};



} //namespace
} //namespace


#endif
