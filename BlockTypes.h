#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace blockTypes{

void draw();

// "Can Be"-interfaces
struct IsBlock{
	float cachedPosX, cachedPosY;
	float cachedWidth, cachedHeight;

	virtual ~IsBlock();
	
	virtual void recalculateCache(float x, float y) = 0;
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack) = 0;
	bool isInsideMeTest(float x, float y) const;
	float width() const;
	float height() const;
	virtual void draw(float x, float y) = 0;
	virtual void saveAsText(std::string& str) = 0;
};
struct CanBeInExpression : virtual IsBlock{
	virtual ~CanBeInExpression();
};
struct CanBeInOperator : virtual IsBlock{
	virtual ~CanBeInOperator();
};
struct CanBeInFunctionBody : virtual IsBlock{
	virtual ~CanBeInFunctionBody();
};
struct CanBeInClassBody : virtual IsBlock{
	virtual ~CanBeInClassBody();
};

// "Needs"-interfaces
struct NeedSemiColonIfInsideBody{
	virtual ~NeedSemiColonIfInsideBody();
};

/*
struct Iterator{
	struct StackElement{
		IsBlock* block;
		unsigned short metatype;
		unsigned short metadata;
		StackElement(IsBlock* block, unsigned short metatype, unsigned short metadata)
		:	block(block), metatype(metatype), metadata(metadata){
		}
	};

	std::vector<StackElement> stack;

	Iterator(IsBlock* startBlock){
		stack.push_back(StackElement(startBlock,0,0));
	}

	Iterator& operator++(){
		while(stack.size()){
			StackElement se = stack.back().block->getNext();
			if(se.block){
				stack.push_back(se);
				return *this;
			}
			else
				stack.pop_back();
		}
		return *this;
	}

	IsBlock& operator*(){
		return *stack.back().block;
	}
};*/


struct TextLine : virtual IsBlock{
	std::string str;
	
	TextLine();
	TextLine(const std::string& str);
	TextLine(int i);
	TextLine(char c);
	
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);

	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};


struct Comment : virtual IsBlock, CanBeInFunctionBody{
	TextLine* comment;

	Comment();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Integer : virtual CanBeInExpression, CanBeInOperator{
	TextLine* i;

	Integer();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Operator : virtual CanBeInExpression, CanBeInOperator{
	TextLine* op; // +, -, etc
	CanBeInOperator* a, *b;

	Operator();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Expression : virtual IsBlock, CanBeInFunctionBody, NeedSemiColonIfInsideBody{
	CanBeInExpression* exp;

	Expression();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Type : virtual IsBlock{
	TextLine* type;

	Type();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Declaration : virtual IsBlock, CanBeInFunctionBody, CanBeInClassBody, NeedSemiColonIfInsideBody{
	Type* type;
	TextLine* name;
	Expression* exp; // Todo: this should be optional

	Declaration();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	// Render: type name = exp
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Variable : virtual IsBlock, CanBeInExpression{
	Declaration* decl; // The variable class does NOT own this object, it will only point to a declaration, and use it's name!

	Variable(Declaration* decl);
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	// Render: type name = exp
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Function : virtual IsBlock, CanBeInClassBody{
	Type* returnType;
	TextLine* name;
	std::vector<Declaration*> parameters;
	std::vector<CanBeInFunctionBody*> body;

	Function();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct Class : virtual IsBlock, CanBeInClassBody, NeedSemiColonIfInsideBody{
	TextLine* name;
	//std::vector<std::pair<Type*,TextLine*>> templateParameters;
	std::vector<CanBeInClassBody*> body;

	Class();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct For : virtual IsBlock, CanBeInFunctionBody{
	Declaration* decl;
	Expression*  exp1;
	Expression*  exp2;
	std::vector<CanBeInFunctionBody*> body;

	For();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct If : virtual IsBlock, CanBeInFunctionBody{
	Expression*  exp;
	std::vector<CanBeInFunctionBody*> body;

	If();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};

struct While : virtual IsBlock, CanBeInFunctionBody{
	Expression*  exp;
	std::vector<CanBeInFunctionBody*> body;

	While();
	virtual void recalculateCache(float x, float y);
	virtual void isInside(float x, float y, std::vector<IsBlock*>& blockStack);
	virtual void draw(float x, float y);
	virtual void saveAsText(std::string& str);
};



}