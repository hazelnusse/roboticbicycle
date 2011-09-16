/*
 * myclasses.h
 *
 *  Created on: 07.12.2009
 *      Author: mthomas
 */

#ifndef MYCLASSES_H_
#define MYCLASSES_H_

class Root
{
protected:
	int cnt;
public:
	Root(int c) : cnt(c) { }
	void inc_cnt() { cnt++; }
	virtual void say_hello();
};

class Child1 : public Root
{
public:
	Child1(int c) : Root(c) { }
	void say_hello();
};

class Child2 : public Root
{
public:
	Child2(int c) : Root(c) { }
	void say_hello();
};

#endif /* MYCLASSES_H_ */
