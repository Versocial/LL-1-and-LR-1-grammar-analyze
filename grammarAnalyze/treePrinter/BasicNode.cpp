/*
 * MathNode.cpp
 *
 *  Created on: 23 ago. 2019
 *      Author: asens
 */

#include "BasicNode.h"

BasicNode::BasicNode() {
	_parent = nullptr;

}

BasicNode::BasicNode(word w, int canItBeLeaf)
{
	_parent = nullptr;
	this->w = w;
	_data = w.value;
	canBeLeaf = canItBeLeaf;
}
//
//BasicNode::BasicNode(std::string data) {
//	_parent = nullptr;
//	_data   = data;
//}

BasicNode::~BasicNode() {
	for(auto it = _children.begin(); it != _children.end(); ++it)
	{
		delete (*it);
		//(*it) = nullptr;
	}
}

void BasicNode::addChildren(BasicNode *nd) {
	_children.push_back(nd);
	nd->setParent(this);
}

std::list<BasicNode*> BasicNode::getChildren() {
	return _children;
}

std::string BasicNode::getData() {
	return _data;
}
//
//void BasicNode::setData(std::string data) {
//	_data = data;
//}

void BasicNode::setWord(word w)
{
	this->w = w;
	this->_data = w.value;
}

int BasicNode::isLeaf()const
{
	return canBeLeaf;
}

word BasicNode::Word() const
{
	return w;
}

void BasicNode::setParent(BasicNode *nd) {
	_parent = nd;
}
