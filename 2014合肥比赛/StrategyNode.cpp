#include "StdAfx.h"
#include "StrategyNode.h"

StageStopNode::StageStopNode(Node* p_parentNode,
	const char* p_name,
	NodeExternalPrecondition* p_externalPrecondition)
	:Node(p_parentNode, "Stage Stop",
		 p_externalPrecondition)
{

}