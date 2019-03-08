#pragma once

class Expr;

class IVisitor
{
public:
	virtual void visit(class Binary* expr) = 0;
	virtual void visit(class Grouping* expr) = 0;
	virtual void visit(class Literal* expr) = 0;
	virtual void visit(class Unary* expr) = 0;
};