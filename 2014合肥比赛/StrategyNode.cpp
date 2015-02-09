#include "StdAfx.h"
#include "StrategyNode.h"

StageStopNode::StageStopNode(Node* p_parentNode,
	const char* p_name,
	NodeExternalPrecondition* p_externalPrecondition)
	:NodePrioritySelector(p_parentNode,
	"Stage Stop", p_externalPrecondition)
{

}

StageStopNode::NodeRunningSatus 
StageStopNode::Tick(NodeInput& nodeInput, NodeOutput& nodeOutput)
{
	for (int i = 0; i < PLAYER_NUMBER; i++)
	{
		nodeInput.m_DicisionalPlayers[i]->ctrl = STAGE_STOP;
	}
	//ActionToPlayer
}

StageAuto::StageAuto(Node* p_parentNode,
	const char* p_name,
	NodeExternalPrecondition* p_externalPrecondition)
	:NodePrioritySelector(p_parentNode,
	"Stage Auto", p_externalPrecondition)
{

}

StageAuto::NodeRunningSatus 
StageAuto::Tick(NodeInput& nodeInput, NodeOutput& nodeOutput)
{
	//ActionToPlayer
}



