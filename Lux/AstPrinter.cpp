#include "pch.h"
#include "AstPrinter.h"

#include "Exprs.h"

AstPrinter::AstPrinter()
{
}

std::string AstPrinter::print(Expr* expr)
{
	expr->accept(this);
	return result_;
}

void AstPrinter::visit(Binary* expr)
{
	parenthesize(expr->token_.lexeme_, { expr->left_, expr->right_ });
	//parenthesize({ expr->left_, expr->right_ }, expr->token_.lexeme_);
}

void AstPrinter::parenthesize(const std::string & name, const std::vector<Expr*>& exprs)
{
	result_.append("(");
	result_.append(name);
	for (auto& expr : exprs)
	{
		result_.append(" ");
		expr->accept(this);
	}
	result_.append(")");
}

void AstPrinter::parenthesize(const std::vector<Expr*>& exprs, const std::string & name)
{

	for (auto& expr : exprs)
	{
		result_.append(" ");
		expr->accept(this);
	}
	result_.append(name);
	
}

void AstPrinter::visit(Grouping * expr)
{
	parenthesize("group", { expr->expression_ });
}

void AstPrinter::visit(Literal * expr)
{
	result_.append(expr->value_);
}

void AstPrinter::visit(Unary * expr)
{
	parenthesize(expr->token_.lexeme_, { expr->right_ });
}



