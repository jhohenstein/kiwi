
#include "kiwi/core/Node.hpp"
#include "kiwi/utils/DebugStream.hpp"
#include "kiwi/processing/ProcessingPipeline.hpp"
#include "kiwi/mock/MockNodeUpdater.hpp"

using namespace kiwi;
using namespace kiwi::core;
using namespace kiwi::processing;

int main()
{
	SCOPEDBLOCK("Kiwi::core::Node Test");

    ProcessingPipeline p;

    Node::InputArray ins;
    Node::OutputArray outs;
    Node n1(&p, ins, outs, new mock::MockNodeUpdater );
    
	kiwi::log << "hej\n";
	
	return 0;
}