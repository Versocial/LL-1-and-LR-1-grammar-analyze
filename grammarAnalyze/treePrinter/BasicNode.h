/*
 * MathNode.h
 *
 *  Created on: 23 ago. 2019
 *      Author: asens
 */

#ifndef BASICNODE_H_
#define BASICNODE_H_

#include <iostream>
#include <list>
#include "../wordsAnalyze/word.h"

class BasicNode {
public:
	BasicNode();
	BasicNode(word w,int canBeLeaf);

	virtual ~BasicNode();

	void addChildren(BasicNode * nd);
	std::list<BasicNode *> getChildren();

	std::string getData();
	void setData(std::string data);
	int isLeaf()const;
	static const int mustLeaf = 1;
	static const int canNotLeaf = -1;
	static const int maybeLeaf = 0;
protected:
	void setParent(BasicNode *nd);
	word w;
	int canBeLeaf;
	std::string _data;

	std::list<BasicNode *> _children;
	BasicNode * _parent;
};

#endif /* BASICNODE_H_ */
