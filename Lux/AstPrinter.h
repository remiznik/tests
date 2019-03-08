#pragma once

#include <string>
#include <vector>
#include "IVisitor.h"

class Expr;

class AstPrinter : public IVisitor
{
public:
	AstPrinter();

	std::string print(Expr* expr);

	virtual void visit(class Binary* expr) override;
	virtual void visit(class Grouping* expr) override;
	virtual void visit(class Literal* expr) override;
	virtual void visit(class Unary* expr) override;
private:
	void parenthesize(const std::string& name, const std::vector<Expr*>& exprs);
	void parenthesize(const std::vector<Expr*>& exprs, const std::string& name);

	std::string result_;
};

