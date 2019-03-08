#pragma once

class IVisitor;
class Expr
{
public:
	virtual void accept(IVisitor* visitor) = 0;
};
