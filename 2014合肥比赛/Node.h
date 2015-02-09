/*
 **********************************************************************
 *	Copyright (C) 2015.
 *	All Rights Reserved.
 **********************************************************************
 *
 *	@file		Node.h
 *	@brief		The class of behavior tree's node.		
 *
 *	Date        Name        Description
 *	06/02/15	ssj			Creation.
 *********************************************************************/

//#pragma once

#if !defined(AFX)
#define AFX


#include "NodeExternalPrecondition.h"
#include "NodeInput.h"
#include "NodeOutput.h"
#include <string>
#include "StaticData.h"

class Node
{

protected:

	/*==========================================================================*/
	/*	protected Variables														*/
	/*==========================================================================*/

	static const int MAX_COUNT = 64; /**< The maximum of child node count. */

	Node* m_pChildNodeList[MAX_COUNT]; /**< The array saving child nodes. */

	Node* m_pParentNode; /**< The pointer saving parent node. */

	Node* m_pActiveNode; /**< The pointer saving active child node now. */

	Node* m_pLastActiveNode; /**< The pointer saving active child node at the previous time. */

	NodeExternalPrecondition* m_pExternalPrecondition; /**< The pointer saving external precondition. */

	std::string m_NodeName; /**< The name of Node. */

	int m_ChildNodeCount; /**< The number of child Nodes. */

	enum NodeRunningSatus /**< The status of child Nodes. */
	{
		RUNNING_EXECUING = 0,
		RUNNING_FINISH = 1,
		RUNNING_ERROR = -1,
		// We can add other conditions.
	};

public:
	/*==========================================================================*/
	/*	public Constructor \ Destructor \ Operator Overloading					*/
	/*==========================================================================*/

	/**
	 *	\brief Constructor
	 *
	 *  \param	p_parentNode	Parent node.
	 *	\param  p_name	Node's name.
	 *  \param	p_externalPrecondition		External precondition.
	 */
	Node(Node* p_parentNode, const char* p_name,
		NodeExternalPrecondition* p_externalPrecondition);

	/**
	 *	\brief Destructor
	 */
	virtual ~Node();

	/*==========================================================================*/
	/*	public Methods, Query													*/
	/*==========================================================================*/

	/**
	 *	\brief Set parent node.
	 *
	 *  \param	p_parentNode	Parent node.
	 */
	void SetParentNode(Node* p_parentNode);

	/**
	 *	\brief Change node's name.
	 *
	 *  \param	p_name	node's new name.
	 */
	void SetNodeName(const char* p_name);

	/**
	 *	\brief	Evaluate whether or not active this node.
	 *
	 *  \param	nodeInput	Node's input.
	 *
	 *	\return	The result of whether or not active this node.
	 */
	bool Evaluate(NodeInput& nodeInput);

	/**
	 *	\brief	Evaluate whether or not m_childNodeIndex is effective.
	 *
	 *  \param	m_index	child Node's index.
	 *
	 *	\return	The result of whether or not m_childNodeIndex is effective.
	 */
	bool Check(int m_childNodeIndex);

	/**
	 *	\brief Get node's name.
	 */
	const char* GetNodeName() const;

	/**
	 *	\brief Set external precondition.
	 *
	 *  \param	p_externalPrecondition	External precondition.
	 */
	void SetExternalPrecondition
		(NodeExternalPrecondition* p_externalPrecondition);

	/**
	 *	\brief add a child node.
	 *
	 *  \param	p_childeNode	One child node needs to add.
	 *
	 *	\return	The pointer of this node.
	 */
	Node& AddChildNode(Node* p_childeNode);
	
	/**
	 *	\brief  Switch to the other child nodes.
	 *
	 *  \param	nodeInput	Node's input.
	 */
	virtual void transition(NodeInput& nodeInput) = 0;

	/**
	 *	\brief  update and running bahavior tree.
	 *
	 *  \param	nodeInput	Node's input.
	 *	\param  nodeOutput  Node's output
	 *
	 *	\return	Behavior trees running state.
	 */
	virtual NodeRunningSatus Tick(NodeInput& nodeInput, NodeOutput& nodeOutput) = 0;

	/**
	 *	\brief	Internal	enaluate.
	 *
	 *  \param	nodeInput	Node's input.
	 *
	 *	\return	The result of whether or not input accord to internal precondition.
	 */
	virtual bool InternalPrecondition(NodeInput& nodeInput);

};


class NodePrioritySelector:public Node
{
public:
	/*==========================================================================*/
	/*	public Constructor \ Destructor \ Operator Overloading					*/
	/*==========================================================================*/

	/**
	 *	\brief Constructor
	 *
	 *  \param	p_parentNode	Parent node.
	 *	\param  p_name	Node's name.
	 *  \param	p_externalPrecondition		External precondition.
	 */
	NodePrioritySelector(Node* p_parentNode, const char* p_name,
		NodeExternalPrecondition* p_externalPrecondition);

	/*==========================================================================*/
	/*	public Methods, Query													*/
	/*==========================================================================*/

	/**
	 *	\brief  Switch to the other child nodes.
	 *
	 *  \param	nodeInput	Node's input.
	 */
	virtual void transition(NodeInput& nodeInput);

	/**
	 *	\brief  update and running bahavior tree.
	 *
	 *  \param	nodeInput	Node's input.
	 *	\param  nodeOutput  Node's output.
	 *
	 *	\return	Behavior priority select trees running state.
	 */
	virtual NodeRunningSatus Tick(NodeInput& nodeInput, NodeOutput& nodeOutput);

	/**
	 *	\brief	Internal	enaluate.
	 *
	 *  \param	nodeInput	Node's input.
	 *
	 *	\return	The result of whether or not input accord to internal precondition.
	 */
	virtual bool InternalPrecondition(NodeInput& nodeInput);


protected:

	int m_CurrentSelectIndex;/**< The child nodes of the evaluate function current selected  */
	int m_LastSelectIndex;/**< The child nodes of the evaluate function last selected  */

};


#endif
