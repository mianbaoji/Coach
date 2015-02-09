#include "stdafx.h"
#include "Node.h"

Node::Node(Node* p_parentNode, const char* p_name,
	NodeExternalPrecondition* p_externalPrecondition):
	m_pParentNode(nullptr),
	m_pActiveNode(nullptr),
	m_pLastActiveNode(nullptr),
	m_pExternalPrecondition(nullptr),
	m_NodeName("UNNAMED"),
	m_ChildNodeCount(0)
{
	for (int i = 0; i < MAX_COUNT; i++)
	{
		m_pChildNodeList[i] = nullptr;
	}
	if (p_parentNode)
	{
		SetParentNode(p_parentNode);
	}
	if (p_externalPrecondition)
	{
		SetExternalPrecondition(p_externalPrecondition);
	}
	SetNodeName(p_name);
}

Node::~Node()
{
	for (int i = 0; i < MAX_COUNT; i++)
	{
		if (m_pChildNodeList[i] != nullptr)
		{
			delete(m_pChildNodeList[i]);
		}
	}
	if (m_pExternalPrecondition != nullptr)
	{
		delete(m_pExternalPrecondition);
	}
}

inline void Node::SetParentNode(Node* p_parentNode)
{
	m_pParentNode = p_parentNode;
	return;
}

inline void Node::SetNodeName(const char* p_name)
{
	m_NodeName = p_name;
	return ;
}

bool Node::Evaluate(NodeInput& nodeInput)
{
	if ((m_pExternalPrecondition == NULL ||
		m_pExternalPrecondition->Evaluate(nodeInput))
		&& InternalPrecondition(nodeInput))
	{
		return true;
	}
	else return false;
}

inline bool Node::Check(int m_childNodeIndex)
{
	if (m_childNodeIndex >= 0 && m_childNodeIndex < m_ChildNodeCount)
	{
		return true;
	}
	else return false;
}

inline const char* Node::GetNodeName() const
{
	return m_NodeName.c_str();
}

inline void Node::SetExternalPrecondition
	(NodeExternalPrecondition* p_externalPrecondition)
{
	m_pExternalPrecondition = p_externalPrecondition;
	return;
}

Node& Node::AddChildNode(Node* p_childNode)
{
	if (m_ChildNodeCount == MAX_COUNT)
	{
		printf("The child nodes are too many!\n");
		return *this;
	}
	m_pChildNodeList[m_ChildNodeCount++] = p_childNode;
	return *this;
}

bool Node::InternalPrecondition(NodeInput& nodeInput)
{
	return true;
}

NodePrioritySelector::NodePrioritySelector
(Node* p_parentNode, const char* p_name, 
	NodeExternalPrecondition* p_externalPrecondition):
	Node(p_parentNode, p_name, p_externalPrecondition),
	m_CurrentSelectIndex(MAX_COUNT),
	m_LastSelectIndex(MAX_COUNT)
{

}


void NodePrioritySelector::transition(NodeInput& nodeInput)
{
	if (Check(m_LastSelectIndex))
	{
		Node* tempChildNode = m_pChildNodeList[m_LastSelectIndex];
		tempChildNode->transition(nodeInput);
	}
	m_LastSelectIndex = MAX_COUNT;
	return;
}

NodePrioritySelector::NodeRunningSatus 
NodePrioritySelector::Tick
(NodeInput& nodeInput, NodeOutput& nodeOutput)
{
	NodeRunningSatus tempRunningStatus = RUNNING_FINISH;
	if (Check(m_CurrentSelectIndex))
	{
		if (m_LastSelectIndex != m_CurrentSelectIndex)  
		{
			if (Check(m_LastSelectIndex))
			{
				Node* tempChildNode = m_pChildNodeList[m_LastSelectIndex];
				tempChildNode->transition(nodeInput);
			}
			m_LastSelectIndex = m_CurrentSelectIndex;
		}
	}
	if (Check(m_LastSelectIndex))
	{
		Node* tempChildNode = m_pChildNodeList[m_LastSelectIndex];
		tempRunningStatus = tempChildNode->Tick(nodeInput, nodeOutput);
		if (tempRunningStatus)
			m_LastSelectIndex = MAX_COUNT;
	}
	return tempRunningStatus;
}

bool NodePrioritySelector::InternalPrecondition(NodeInput& nodeInput)
{
	m_CurrentSelectIndex = MAX_COUNT;
		for(int i = 0; i < m_ChildNodeCount; i++)
		{
			Node* tempChildNode = m_pChildNodeList[m_LastSelectIndex];
			if(tempChildNode->Evaluate(nodeInput))
			{
				m_CurrentSelectIndex = i;
				return true;
			}
		}
		return false;
}

