#include "BlockTypes.h"

void bolls_draw_text(const std::string& text, float x, float y);
extern float font_char_width;
extern float font_char_height;


Function* f = NULL;

namespace blockTypes{
	void draw(){
		if(f==NULL)
			f = new Function();
		f->draw(0,0);
	}
}



IsBlock::~IsBlock(){}
bool IsBlock::isInsideMeTest(float x, float y) const{
	return x > cachedPosX && x < cachedPosX + cachedWidth && y > cachedPosY && y < cachedPosY + cachedHeight;
}

float IsBlock::width() const{
	return cachedWidth;
}
float IsBlock::height() const{
	return cachedHeight;
}



// "Can Be"-interfaces
CanBeInExpression::~CanBeInExpression(){}
CanBeInOperator::~CanBeInOperator(){}
CanBeInFunctionBody::~CanBeInFunctionBody(){}
CanBeInClassBody::~CanBeInClassBody(){}

// "Needs"-interfaces
NeedSemiColonIfInsideBody::~NeedSemiColonIfInsideBody(){}




TextLine::TextLine(){
	recalculateCache(0,0);
}
TextLine::TextLine(const std::string& str):str(str){
	recalculateCache(0,0);
}
TextLine::TextLine(int i){
	char temp[16];
	sprintf_s(temp,16,"%i",i);
	str.assign(temp);
	recalculateCache(0,0);
}
TextLine::TextLine(char c){
	str.push_back(c);
	recalculateCache(0,0);
}

void TextLine::recalculateCache(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	cachedWidth  = font_char_width * str.length();
	cachedHeight = font_char_height;
}
void TextLine::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
	}
}

void TextLine::draw(float x, float y){
	//drawText(x,y,str);
	bolls_draw_text(str,x,y);
}
void TextLine::saveAsText(std::string& str){
	str.append(this->str);
}




Comment::Comment(){
	comment = new TextLine("Comment?");
	recalculateCache(0,0);
}
void Comment::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	cachedWidth  = comment->width();
	cachedHeight = comment->height();
}
void Comment::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		comment->isInside(x,y,blockStack);
	}
}
void Comment::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	TextLine("// ").draw(x,y);
	comment->draw(x + TextLine("// ").width() ,y);
}
void Comment::saveAsText(std::string& str){
	str.append("/*");
	comment->saveAsText(str);
	str.append("*/");
}



Integer::Integer(){
	i = new TextLine(0);
	recalculateCache(0,0);
}
void Integer::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	cachedWidth  = i->width();
	cachedHeight = i->height();
}
void Integer::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
	}
}
void Integer::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	i->draw(x,y);
}
void Integer::saveAsText(std::string& str){
	i->saveAsText(str);
}



Operator::Operator(){
	op = new TextLine('+');
	a = new Integer();
	b = new Integer();
	recalculateCache(0,0);
}
void Operator::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	cachedWidth  = a->width() + op->width() + b->width();
	cachedHeight = op->height();
}
void Operator::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		op->isInside(x,y,blockStack);
		a->isInside(x,y,blockStack);
		b->isInside(x,y,blockStack);
	}
}
void Operator::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	a->draw(x,y);
	op->draw(x + a->width(),y);
	b->draw(x+a->width()+op->width(), y);
}
void Operator::saveAsText(std::string& str){
	a->saveAsText(str);
	op->saveAsText(str);
	b->saveAsText(str);
}




Expression::Expression(){
	exp = new Integer();
	recalculateCache(0,0);
}
void Expression::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	cachedWidth  = exp->width();
	cachedHeight = exp->height();
}
void Expression::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		exp->isInside(x,y,blockStack);
	}
}
void Expression::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	exp->draw(x,y);
}
void Expression::saveAsText(std::string& str){
	exp->saveAsText(str);
}




Type::Type(){
	type = new TextLine("Type?");
	recalculateCache(0,0);
}
void Type::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	cachedWidth  = type->width();
	cachedHeight = type->height(); // All have the same height
}
void Type::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		type->isInside(x,y,blockStack);
	}
}
void Type::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	type->draw(x,y);
}
void Type::saveAsText(std::string& str){
	type->saveAsText(str);
}



Declaration::Declaration(){
	type = new Type();
	name = new TextLine("Name?");
	exp = new Expression();
	recalculateCache(0,0);
}
void Declaration::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	cachedWidth  = type->width() + TextLine(' ').width() + name->width() + TextLine(' ').width() + TextLine('=').width() + TextLine(' ').width() + exp->width();
	cachedHeight = type->height(); // All have the same height
}
void Declaration::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		type->isInside(x,y,blockStack);
		name->isInside(x,y,blockStack);
		exp->isInside(x,y,blockStack);
	}
}
void Declaration::draw(float x, float y){
	// Render: type name = exp
	cachedPosX = x;
	cachedPosY = y;
	type->draw(x,y);
	name->draw(x + type->width() + TextLine(' ').width(), y);
	TextLine('=').draw(x + type->width() + TextLine(' ').width() + name->width() + TextLine(' ').width(), y);
	exp->draw(x + type->width() + TextLine(' ').width() + name->width() + TextLine(' ').width() + TextLine(' ').width() + TextLine('=').width(), y);
}
void Declaration::saveAsText(std::string& str){
	type->saveAsText(str);
	TextLine(' ').saveAsText(str);
	name->saveAsText(str);
	TextLine(" = ").saveAsText(str);
	exp->saveAsText(str);
}


Variable::Variable(Declaration* decl){
	this->decl = decl;
}
void Variable::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	cachedWidth  = decl->name->width();
	cachedHeight = decl->name->height();
}
void Variable::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		// Dont ask decl, cause we don't contain it, we only point at it...
	}
}
void Variable::draw(float x, float y){
	// Todo... how should i fix this... This draw function will update the cache for the declarations name... Which will create bugs for picking etc...
	// So... lets cache the cache... A stupid solution on a stupid problem...
	float px = decl->name->cachedPosX;
	float py = decl->name->cachedPosY;
	decl->name->draw(x,y);
	decl->name->cachedPosX = px;
	decl->name->cachedPosY = py;
}
void Variable::saveAsText(std::string& str){
	decl->name->saveAsText(str);
}


Function::Function(){
	returnType = new Type();
	name = new TextLine("Name?");
	parameters.push_back(new Declaration());
	body.push_back(new Expression());
	body.push_back(new Declaration());
	body.push_back(new Expression());
	recalculateCache(0,0);
}
void Function::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	float header_width = returnType->width() + TextLine(' ').width() + name->width() + TextLine('(').width();

	for(size_t i=0; i<parameters.size(); i++){
		header_width += parameters[i]->width();
		if(i!=parameters.size()-1)
			header_width += TextLine(", ").width();
	}

	header_width += TextLine(')').width();

	float body_max_width = 0.0;
	for(size_t i=0; i<body.size(); i++){
		float body_width = body[i]->width();
		if( body_max_width < body_width )
			body_max_width = body_width;
	}

	TextLine functionTab("    ");
	cachedWidth = std::max(header_width, functionTab.width() + body_max_width);
	

	float body_height = 0.0;
	for(size_t i=0; i<body.size(); i++)
		body_height += body[i]->height();
	cachedHeight = returnType->height() + body_height;
}
void Function::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		returnType->isInside(x,y,blockStack);
		name->isInside(x,y,blockStack);
		for(size_t i=0; i<parameters.size(); ++i)
			parameters[i]->isInside(x,y,blockStack);
		for(size_t i=0; i<body.size(); ++i)
			body[i]->isInside(x,y,blockStack);
	}
}
void Function::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	returnType->draw(x,y);
	name->draw(x + returnType->width() + TextLine(' ').width(), y);
	TextLine('(').draw(x + returnType->width() + TextLine(' ').width() + name->width(), y);

	float xi = x + returnType->width() + TextLine(' ').width() + name->width() + TextLine('(').width();
	for(size_t i=0; i<parameters.size(); i++){
		parameters[i]->draw(xi, y);
		xi += parameters[i]->width();
		if(i!=parameters.size()-1){
			TextLine(", ").draw(xi, y);
			xi += TextLine(", ").width();
		}
	}
	TextLine(')').draw(xi, y);

	y += TextLine(' ').height();

	TextLine functionTab("    ");

	for(size_t i=0; i<body.size(); i++){
		body[i]->draw(x + functionTab.width(), y);
		y += body[i]->height();
	}
}
void Function::saveAsText(std::string& str){
	returnType->saveAsText(str);
	TextLine(' ').saveAsText(str);
	name->saveAsText(str);
	TextLine("( ").saveAsText(str);
	for(size_t i=0; i<parameters.size(); i++){
		parameters[i]->saveAsText(str);
		if(i!=parameters.size()-1)
			TextLine(", ").saveAsText(str);
	}
	TextLine(" )").saveAsText(str);
	TextLine("\n").saveAsText(str);

	TextLine("{").saveAsText(str);
	TextLine("\n").saveAsText(str);

	for(size_t i=0; i<body.size(); i++){
		body[i]->saveAsText(str);

		if(dynamic_cast<NeedSemiColonIfInsideBody*>(body[i]))
			str.append(";");

		str.append("\n");
	}

	TextLine("}").saveAsText(str);
	TextLine("\n").saveAsText(str);
}




Class::Class(){
	name = new TextLine("Name?");
	body.push_back(new Declaration());
	body.push_back(new Function());
	recalculateCache(0,0);
}
void Class::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	float header_width = TextLine("class ").width() + name->width();

	float body_max_width = 0.0;
	for(size_t i=0; i<body.size(); i++){
		float body_width = body[i]->width();
		if( body_max_width < body_width )
			body_max_width = body_width;
	}

	TextLine functionTab("    ");
	cachedWidth = std::max(header_width, functionTab.width() + body_max_width);
	

	float body_height = 0.0;
	for(size_t i=0; i<body.size(); i++)
		body_height += body[i]->height();
	cachedHeight = name->height() + body_height;
}
void Class::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		name->isInside(x,y,blockStack);
		for(size_t i=0; i<body.size(); ++i)
			body[i]->isInside(x,y,blockStack);
	}
}
void Class::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	TextLine("class ").draw(x,y);
	name->draw(x + TextLine("class ").width(), y);

	y += TextLine(' ').height();

	TextLine classTab("    ");

	for(size_t i=0; i<body.size(); i++){
		body[i]->draw(x + classTab.width(), y);
		y += body[i]->height();
	}
}
void Class::saveAsText(std::string& str){
	TextLine("class ").saveAsText(str);
	name->saveAsText(str);

	TextLine("{").saveAsText(str);
	TextLine("\n").saveAsText(str);

	for(size_t i=0; i<body.size(); i++){
		body[i]->saveAsText(str);

		if(dynamic_cast<NeedSemiColonIfInsideBody*>(body[i]))
			str.append(";");

		str.append("\n");
	}

	TextLine("}").saveAsText(str);
	TextLine("\n").saveAsText(str);
}



For::For(){
	decl = new Declaration();
	exp1 = new Expression();
	exp2 = new Expression();
	recalculateCache(0,0);
}
void For::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	float header_width = TextLine("for( ").width() + decl->width() + TextLine("; ").width() + exp1->width() + TextLine("; ").width() + exp2->width() + TextLine(" )").width();

	float body_max_width = 0.0;
	for(size_t i=0; i<body.size(); i++){
		float body_width = body[i]->width();
		if( body_max_width < body_width )
			body_max_width = body_width;
	}

	TextLine functionTab("    ");
	cachedWidth = std::max(header_width, functionTab.width() + body_max_width);
	

	float body_height = 0.0;
	for(size_t i=0; i<body.size(); i++)
		body_height += body[i]->height();
	cachedHeight = decl->height() + body_height;
}
void For::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		decl->isInside(x,y,blockStack);
		exp1->isInside(x,y,blockStack);
		exp2->isInside(x,y,blockStack);
		for(size_t i=0; i<body.size(); ++i)
			body[i]->isInside(x,y,blockStack);
	}
}
void For::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	TextLine("for( ").draw(x,y);
	decl->draw(x + TextLine("for( ").width(), y);
	TextLine("; ").draw(x + TextLine("for( ").width() + decl->width(), y);
	exp1->draw(x + TextLine("for( ").width() + decl->width() + TextLine("; ").width(), y);
	TextLine("; ").draw(x + TextLine("for( ").width() + decl->width() + TextLine("; ").width() + exp1->width(), y);
	exp2->draw(x + TextLine("for( ").width() + decl->width() + TextLine("; ").width() + exp1->width() + TextLine("; ").width(), y);
	TextLine(" )").draw(x + TextLine("for( ").width() + decl->width() + TextLine("; ").width() + exp1->width() + TextLine("; ").width() + exp2->width(), y);

	y += TextLine(' ').height();

	TextLine forTab("    ");

	for(size_t i=0; i<body.size(); i++){
		body[i]->draw(x + forTab.width(), y);
		y += body[i]->height();
	}
}
void For::saveAsText(std::string& str){
	TextLine("for( ").saveAsText(str);
	decl->saveAsText(str);
	TextLine("; ").saveAsText(str);
	exp1->saveAsText(str);
	TextLine("; ").saveAsText(str);
	exp2->saveAsText(str);
	TextLine(" )\n").saveAsText(str);

	TextLine("{").saveAsText(str);
	TextLine("\n").saveAsText(str);

	for(size_t i=0; i<body.size(); i++){
		body[i]->saveAsText(str);

		if(dynamic_cast<NeedSemiColonIfInsideBody*>(body[i]))
			str.append(";");

		str.append("\n");
	}

	TextLine("}").saveAsText(str);
	TextLine("\n").saveAsText(str);
}



If::If(){
	exp = new Expression();
	recalculateCache(0,0);
}
void If::recalculateCache(float x, float y){
	cachedPosX = x;
	cachedPosY = y;

	float header_width = TextLine("if( ").width() + exp->width() + TextLine(" )").width();

	float body_max_width = 0.0;
	for(size_t i=0; i<body.size(); i++){
		float body_width = body[i]->width();
		if( body_max_width < body_width )
			body_max_width = body_width;
	}

	TextLine functionTab("    ");
	cachedWidth = std::max(header_width, functionTab.width() + body_max_width);
	

	float body_height = 0.0;
	for(size_t i=0; i<body.size(); i++)
		body_height += body[i]->height();
	cachedHeight = exp->height() + body_height;
}
void If::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		exp->isInside(x,y,blockStack);
		for(size_t i=0; i<body.size(); ++i)
			body[i]->isInside(x,y,blockStack);
	}
}
void If::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	TextLine("if( ").draw(x,y);
	exp->draw(x + TextLine("if( ").width(), y);
	TextLine(" )").draw(x + TextLine("if( ").width() + exp->width(), y);

	y += TextLine(' ').height();

	TextLine ifTab("    ");

	for(size_t i=0; i<body.size(); i++){
		body[i]->draw(x + ifTab.width(), y);
		y += body[i]->height();
	}
}
void If::saveAsText(std::string& str){
	TextLine("if( ").saveAsText(str);
	exp->saveAsText(str);
	TextLine(" )\n").saveAsText(str);

	TextLine("{").saveAsText(str);
	TextLine("\n").saveAsText(str);

	for(size_t i=0; i<body.size(); i++){
		body[i]->saveAsText(str);

		if(dynamic_cast<NeedSemiColonIfInsideBody*>(body[i]))
			str.append(";");

		str.append("\n");
	}

	TextLine("}").saveAsText(str);
	TextLine("\n").saveAsText(str);
}



While::While(){
	exp = new Expression();
	recalculateCache(0,0);
}
void While::recalculateCache(float x, float y){
	cachedPosX  = x;
	cachedPosY  = y;

	float header_width = TextLine("while( ").width() + exp->width() + TextLine(" )").width();

	float body_max_width = 0.0;
	for(size_t i=0; i<body.size(); i++){
		float body_width = body[i]->width();
		if( body_max_width < body_width )
			body_max_width = body_width;
	}

	TextLine functionTab("    ");
	cachedWidth = std::max(header_width, functionTab.width() + body_max_width);
	
	
	float body_height = 0.0;
	for(size_t i=0; i<body.size(); i++)
		body_height += body[i]->height();
	cachedHeight = exp->height() + body_height;
}
void While::isInside(float x, float y, std::vector<IsBlock*>& blockStack){
	if(isInsideMeTest(x,y)){
		blockStack.push_back(this);
		exp->isInside(x,y,blockStack);
		for(size_t i=0; i<body.size(); ++i)
			body[i]->isInside(x,y,blockStack);
	}
}
void While::draw(float x, float y){
	cachedPosX = x;
	cachedPosY = y;
	TextLine("while( ").draw(x,y);
	exp->draw(x + TextLine("while( ").width(), y);
	TextLine(" )").draw(x + TextLine("while( ").width() + exp->width(), y);

	y += TextLine(' ').height();

	TextLine ifTab("    ");

	for(size_t i=0; i<body.size(); i++){
		body[i]->draw(x + ifTab.width(), y);
		y += body[i]->height();
	}
}
void While::saveAsText(std::string& str){
	TextLine("while( ").saveAsText(str);
	exp->saveAsText(str);
	TextLine(" )\n").saveAsText(str);

	TextLine("{").saveAsText(str);
	TextLine("\n").saveAsText(str);

	for(size_t i=0; i<body.size(); i++){
		body[i]->saveAsText(str);

		if(dynamic_cast<NeedSemiColonIfInsideBody*>(body[i]))
			str.append(";");

		str.append("\n");
	}

	TextLine("}").saveAsText(str);
	TextLine("\n").saveAsText(str);
}