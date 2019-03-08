#pragma once
#include "IExpr.h"
#include "IVisitor.h"
#include "Token.h"

class Binary : public Expr
{
public:
	Binary(Expr* left, Token token, Expr* right) : left_(left), token_(token), right_(right)
	{}
	virtual void accept(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	Expr* left_;
	Token token_;
	Expr* right_;
};

class Grouping : public Expr
{
public:
	Grouping(Expr* expression) : expression_(expression) 
	{}

	virtual void accept(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	Expr* expression_;
};

class Literal : public Expr
{
public:
	Literal(const std::string& value) : value_(value)
	{}
	virtual void accept(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	std::string value_;
};

class Unary : public Expr
{
public:
	Unary(Token token, Expr* right) : token_(token), right_(right)
	{}

	virtual void accept(IVisitor* visitor) override
	{
		visitor->visit(this);
	}

	Token token_;
	Expr* right_;
};