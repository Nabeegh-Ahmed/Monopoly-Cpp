#include "GameEngine.h"
#include<SFML/Graphics.hpp>
#include<math.h>
#include<string>
#include <iostream>
using namespace std;

bool GPEngine::MouseClick(sf::RectangleShape target, sf::Event e, sf::RenderWindow& window) {
	sf::Vector2f mouse;
	mouse.x = sf::Mouse::getPosition(window).x;
	mouse.y = sf::Mouse::getPosition(window).y;
	return (
		target.getGlobalBounds().contains(mouse)&&
		sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		sf::Mouse::ButtonCount == 5

	);
}

bool GPEngine::MouseClick(sf::CircleShape target, sf::Event::MouseButtonEvent mouse) {
	sf::Vector2f targetPos = target.getPosition();
	float size = target.getRadius();
	sf::Vector2f origin = target.getOrigin();
	origin.x += size;
	origin.y += size;
	double dist = pow((pow(mouse.y - origin.y, 2) + pow(mouse.x - origin.x, 2)), 0.5);
	if (mouse.x >= targetPos.x && mouse.x <= targetPos.x + 2 * size && mouse.y >= targetPos.y && mouse.y <= targetPos.y + 2 * size) {
		if (dist < size) {
			return true;
		}
		else {
			return false;
		}
	} else {
		return false;
	}
}
CollisionSide GPEngine::areColliding(sf::RectangleShape r1, sf::RectangleShape r2) {
	// r1 is moving and check collision with r2
	CollisionSide collisions;
	collisions.bottom = false;
	collisions.top = false;
	collisions.left = false;
	collisions.right = false;
	sf::Vector2f pos1 = r1.getPosition();
	sf::Vector2f size1 = r1.getSize();
	sf::Vector2f pos2 = r2.getPosition();
	sf::Vector2f size2 = r2.getSize();
	if (pos1.x + size1.x > pos2.x && pos1.y < pos2.y + size2.y && pos1.y+size1.y > pos2.y && pos1.x < pos2.x) {
		collisions.left = true;
	}
	if (pos1.x + size1.x > pos2.x&& pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y && pos1.x == pos2.x+size2.y) {
		collisions.right = true;
	}
	if (pos1.y + size1.y > pos2.y && pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x && pos1.y < pos2.y) {
		collisions.top = true;
	}
	if (pos2.y + size2.y > pos1.y && pos2.x < pos1.x + size1.x && pos2.x + size2.x > pos1.x && pos2.y < pos1.y) {
		collisions.bottom = true;
	}

	return collisions;
}
CollisionSide GPEngine::areColliding(sf::CircleShape r1, sf::RectangleShape r2) {
	CollisionSide c;
	c.left = false;
	c.right = false;
	c.bottom = false;
	c.top = false;
	sf::Vector2f CirclePosition = r1.getPosition();
	double CircleSize = r1.getRadius();
	CirclePosition.x = CirclePosition.x + CircleSize;
	CirclePosition.y = CirclePosition.y + CircleSize;
	sf::Vector2f RectanglePosition = r2.getPosition();
	sf::Vector2f RectangleSize = r2.getSize();
	sf::Vector2f Collisioncoords = CirclePosition;
	Collisioncoords.x = CirclePosition.x + CircleSize;
	Collisioncoords.y = CirclePosition.y;
	float Rec_Pos_Size_y = (RectanglePosition.y + RectangleSize.y);
	
	for (double angle = 0.0,incrementer=0.0; angle <= 90; angle += 3,incrementer+= 0.03333333) {
		if ((((Collisioncoords.x - CircleSize * incrementer) >= (RectanglePosition.x)) && ((Collisioncoords.x - CircleSize * incrementer) <= (RectanglePosition.x + 3.5))) && (((Collisioncoords.y - CircleSize * incrementer) >= (RectanglePosition.y)) && ((Collisioncoords.y - CircleSize * incrementer) <= Rec_Pos_Size_y))) {
			c.left = true;
		}
		if ((((Collisioncoords.x - CircleSize * incrementer) >= (RectanglePosition.x)) && ((Collisioncoords.x - CircleSize * incrementer) <= (RectanglePosition.x + RectangleSize.x))) && (((Collisioncoords.y - CircleSize * incrementer) >= (Rec_Pos_Size_y -3.5)) && ((Collisioncoords.y - CircleSize * incrementer) <= Rec_Pos_Size_y))) {
			c.bottom=true;
		}
	}
	Collisioncoords.x = CirclePosition.x;
	Collisioncoords.y = CirclePosition.y - CircleSize;
	for (double angle = 90, incrementer = 0.0; angle <= 180; angle += 3, incrementer += 0.03333333) {
		if ((((Collisioncoords.x - CircleSize * incrementer) >= (RectanglePosition.x+RectangleSize.x-3.5)) && ((Collisioncoords.x - CircleSize * incrementer) <= (RectanglePosition.x + RectangleSize.x))) && (((Collisioncoords.y + CircleSize * incrementer) >= (RectanglePosition.y)) && ((Collisioncoords.y + CircleSize * incrementer) <= (RectanglePosition.y + RectangleSize.y)))) {
			c.right = true;
		}
		if ((((Collisioncoords.x - CircleSize * incrementer) >= (RectanglePosition.x)) && ((Collisioncoords.x - CircleSize * incrementer) <= (RectanglePosition.x + RectangleSize.x))) && (((Collisioncoords.y + CircleSize * incrementer) >= (RectanglePosition.y + RectangleSize.y - 3.5)) && ((Collisioncoords.y + CircleSize * incrementer) <= (RectanglePosition.y + RectangleSize.y)))) {
			c.bottom = true;
		}
	}
	Collisioncoords.x = CirclePosition.x-CircleSize;
	Collisioncoords.y = CirclePosition.y;
	for (double angle = 180,incrementer=0.0; angle <= 270; angle += 3,incrementer+= 0.03333333) {
		if ((((Collisioncoords.x + CircleSize * incrementer) >= (RectanglePosition.x )) && ((Collisioncoords.x + CircleSize * incrementer) <= (RectanglePosition.x + RectangleSize.x))) && (((Collisioncoords.y + CircleSize * incrementer) >= (RectanglePosition.y)) && ((Collisioncoords.y + CircleSize * incrementer) <= (RectanglePosition.y +3.5)))) {
			c.top = true;
		}
		if ((((Collisioncoords.x + CircleSize * incrementer) >= (RectanglePosition.x + RectangleSize.x - 3.5)) && ((Collisioncoords.x + CircleSize * incrementer) <= (RectanglePosition.x + RectangleSize.x))) && (((Collisioncoords.y + CircleSize * incrementer) >= (RectanglePosition.y )) && ((Collisioncoords.y + CircleSize * incrementer) <= (RectanglePosition.y + RectangleSize.y)))) {
			c.right = true;
		}
	}
	Collisioncoords.x = CirclePosition.x;
	Collisioncoords.y = CirclePosition.y+CircleSize;
	for (double angle = 270,incrementer=0.0; angle <= 360; angle += 3,incrementer+= 0.03333333) {
		if ((((Collisioncoords.x + CircleSize * incrementer) >= (RectanglePosition.x)) && ((Collisioncoords.x + CircleSize * incrementer) <= (RectanglePosition.x + RectangleSize.x))) && (((Collisioncoords.y - CircleSize * incrementer) >= (RectanglePosition.y)) && ((Collisioncoords.y - CircleSize * incrementer) <= (RectanglePosition.y + 3.5)))) {
			c.top = true;
		}
		if ((((Collisioncoords.x + CircleSize * incrementer) >= (RectanglePosition.x)) && ((Collisioncoords.x + CircleSize * incrementer) <= (RectanglePosition.x + 3.5))) && (((Collisioncoords.y - CircleSize * incrementer) >= (RectanglePosition.y)) && ((Collisioncoords.y - CircleSize * incrementer) <= (RectanglePosition.y + RectangleSize.y)))) {
			c.left = true;
		}
	}
	return c;
	
}
void GPEngine::Gravity(sf::RectangleShape &fallingobject, sf::RectangleShape& base) {
	CollisionSide _base = this->areColliding(fallingobject, base);
	static double velocity = 0.01;
	double acceleration = 0.0010;
	if (_base.top == false) {
		sf::Vector2f objectposition = fallingobject.getPosition();
		fallingobject.move(0.0f, velocity);
		if (velocity < 20) {
			velocity += acceleration;
		}
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			fallingobject.move(0.f, -1.f);
		}
		velocity = 0.001;
	}
	if (_base.bottom) {
		fallingobject.move(0.f, 3.f);
	}
}
void GPEngine::Gravity(sf::CircleShape& fallingobject, sf::RectangleShape& base) {
	CollisionSide _base = this->areColliding(fallingobject, base);
	static double velocity = 0.1;
	double acceleration = 0.010;
	if (_base.top == false) {
		sf::Vector2f objectposition = fallingobject.getPosition();
		fallingobject.move(0.0f, velocity);
		if (velocity < 20) {
			velocity += acceleration;
		}
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			fallingobject.move(0.f, -1.f);
		}
		velocity = 0.1;
	}
}
void GPEngine::animation(sf::RectangleShape& shape, sf::Texture* left, sf::Texture* right, sf::Texture* up, sf::Texture* down) {
	static int xvelocity = 0.1;
	static int yvelocity = 0.1;
	int lastyvelocity = yvelocity;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		shape.setTexture(left);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		shape.setTexture(right);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		shape.setTexture(up);
		yvelocity++;
	}
	else {
		yvelocity--;
	}
	if (lastyvelocity > yvelocity || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		shape.setTexture(down);
	}
}
void GPEngine::animation(sf::CircleShape& shape, sf::Texture* left, sf::Texture* right, sf::Texture* up, sf::Texture* down) {
	static int xvelocity = 0.1;
	static int yvelocity = 0.1;
	int lastyvelocity = yvelocity;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		shape.setTexture(left);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		shape.setTexture(right);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		shape.setTexture(up);
		yvelocity++;
	}
	else {
		yvelocity--;
	}
	if (lastyvelocity > yvelocity || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		shape.setTexture(down);
	}
}

void GPEngine::Gravity(sf::RectangleShape& fallingobject, CollisionSide isfalling) {
	static double velocity = 0.1;
	double acceleration = 0.0010;
	if (isfalling.top == false) {
		sf::Vector2f objectposition = fallingobject.getPosition();
		fallingobject.move(0.0f, velocity);
		if (velocity < 20) {
			velocity += acceleration;
		}
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			fallingobject.move(0.f, -1.f);
		}
		velocity = 0.001;
	}
}
void GPEngine::Gravity(sf::CircleShape& fallingobject, CollisionSide isfalling) {
	static double velocity = 0.1;
	double acceleration = 0.0010;
	if (isfalling.top == false) {
		sf::Vector2f objectposition = fallingobject.getPosition();
		fallingobject.move(0.0f, velocity);
		if (velocity < 20) {
			velocity += acceleration;
		}
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			fallingobject.move(0.f, -1.f);
		}
		velocity = 0.001;
	}
}
void GPEngine::enableMovement(sf::RectangleShape& shape) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		shape.move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		shape.move(1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		shape.move(0.f, -3.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		shape.move(0.f, 1.f);
	}
}
void GPEngine::enableMovement(sf::CircleShape& shape) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		shape.move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		shape.move(1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		shape.move(0.f, -3.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		shape.move(0.f, 1.f);
	}
}
void GPEngine::moveView(sf::RectangleShape& shape, sf::View& view) {
	sf::Vector2f pos = shape.getPosition();
	view.setSize(sf::Vector2f(shape.getSize().x * 5, shape.getSize().x * 5));
	view.setCenter(sf::Vector2f(pos.x+shape.getSize().x/2, pos.y));
}
void GPEngine::Light(sf::RectangleShape base, sf::Shader& shader) {
	shader.loadFromFile("assets/vertex_shader.vert", "assets/fragment_shader.frag");
	shader.setUniform("hasTexture", true);
	sf::Vector2f pos;
	pos.x = base.getPosition().x;
	pos.y = base.getPosition().y - base.getSize().y;
	shader.setUniform("lightPos", pos);
}
void GPEngine::FollowLight(sf::RectangleShape shape, sf::Shader& shader) {
	shader.loadFromFile("assets/vertex_shader.vert", "assets/fragment_shader.frag");
	shader.setUniform("hasTexture", true);
	shader.setUniform("lightPos", sf::Vector2f(shape.getPosition().x+shape.getSize().x/2, shape.getPosition().y - shape.getSize().y*2));
}
void GPEngine::Light(sf::CircleShape s1, sf::Shader& shader) {
	shader.loadFromFile("assets/vertex_shader.vert", "assets/fragment_shader.frag");
	shader.setUniform("hasTexture", true);
}
void GPEngine::ShapeRepeater(sf::RectangleShape POV,sf::RectangleShape &ShapeToRepeat, sf::RenderWindow & screen, sf::Vector2f& StartingPos, float DistanceBetweenShape) {
	int i = 0;
	int counter = 0;
	static int numberofshapes=0;
	static int* ptrtoheights=0;
	static bool firsttime = true;
	static bool firstfunctionsturn = true;
	sf::Vector2f generatedposition(StartingPos);
	sf::Vector2f calculationusedposition(StartingPos);
	sf::Vector2f sizeofshape(ShapeToRepeat.getSize());
	sf::Vector2f screendimensions;
	sf::Vector2f originalposition;
	screendimensions.x = screen.getSize().x;
	screendimensions.y = screen.getSize().y;
	if (StartingPos.x+screen.getSize().x <= POV.getPosition().x ||firsttime==true) {
		StartingPos.x = POV.getPosition().x-300;
		numberofshapes = 0;
		i = rand() % (3 - 1 + 1) + 1;
		while (calculationusedposition.x + sizeofshape.x + DistanceBetweenShape < (screendimensions.x) * 2 + StartingPos.x) {
			calculationusedposition.x += sizeofshape.x + DistanceBetweenShape;
			numberofshapes++;
		}
		if (firsttime) {
			ptrtoheights = new int[numberofshapes];
			firsttime = false;
		}
		else {
			delete[]ptrtoheights; 
			ptrtoheights = new int[numberofshapes];

		}
		//for (int index = 0; index < numberofshapes; index++) {
		//	this->HeightRandomizer(ShapeToRepeat, ShapeToRepeat.getSize().y);
		//	ptrtoheights[index] = ShapeToRepeat.getSize().y;
		//}
		switch (i) {
		case 1:
			ShapeToRepeat.setFillColor(sf::Color::Cyan);
			break;
		case 2:
			ShapeToRepeat.setFillColor(sf::Color::Red);
			break;
		case 3:
			ShapeToRepeat.setFillColor(sf::Color::Magenta);
			break;
		}
	}
	if (StartingPos.x -5>= POV.getPosition().x) {
		StartingPos.x= POV.getPosition().x-(screen.getSize().x);
		i = rand() % (3 - 1 + 1) + 1;
		while (calculationusedposition.x + sizeofshape.x + DistanceBetweenShape < (screendimensions.x) * 2 + StartingPos.x) {
			calculationusedposition.x += sizeofshape.x + DistanceBetweenShape;
			numberofshapes++;
		}
		if (firsttime) {
			ptrtoheights = new int[numberofshapes];
			firsttime = false;
		}
		else {
			delete[]ptrtoheights;
			ptrtoheights = new int[numberofshapes];

		}
		//for (int index = 0; index < numberofshapes; index++) {
		//	this->HeightRandomizer(ShapeToRepeat, ShapeToRepeat.getSize().y);
		//	ptrtoheights[index] = ShapeToRepeat.getSize().y;
		//}
		switch (i) {
		case 1:
			ShapeToRepeat.setFillColor(sf::Color::White);
			break;
		case 2:
			ShapeToRepeat.setFillColor(sf::Color::Yellow);
			break;
		case 3:
			ShapeToRepeat.setFillColor(sf::Color::Green);
			break;
		}
	}
	while (generatedposition.x + sizeofshape.x + DistanceBetweenShape < (screendimensions.x) * 2 + StartingPos.x) {
		ShapeToRepeat.setPosition(generatedposition);
		ShapeToRepeat.setSize(sf::Vector2f(ShapeToRepeat.getSize().x,ptrtoheights[counter]));
		if (!firstfunctionsturn) {
			originalposition.x = ShapeToRepeat.getPosition().x;
			originalposition.y = ShapeToRepeat.getPosition().y;
			ShapeToRepeat.setPosition(sf::Vector2f(ShapeToRepeat.getPosition().x, originalposition.y-ShapeToRepeat.getSize().y) );
		}
		else {
			
		}
		screen.draw(ShapeToRepeat);
		generatedposition.x += sizeofshape.x + DistanceBetweenShape;
		counter++;
	}
	if (!firstfunctionsturn) {
		firstfunctionsturn = true;
		ShapeToRepeat.setPosition(originalposition);
	}
	else {
		firstfunctionsturn = false;
	}
}
void GPEngine::HeightRandomizer(sf::RectangleShape& ShapeToAlter, int HeightOfShape) {
	static int originalheight;
	static bool firsttime = true;
	if (firsttime) {
		originalheight = HeightOfShape;
		firsttime = false;
	}
	int percentcutoff = 0;
	percentcutoff = (rand() % (70 - 10 + 1)) + 10;
	ShapeToAlter.setSize(sf::Vector2f(ShapeToAlter.getSize().x, (percentcutoff / 100.0f) * originalheight));
}

UIEngine::UIEngine() {
	GeoSansLight.loadFromFile("assets\\GeosansLight.ttf");
}
uiOptions UIEngine::simpleUI(std::string optionNames[4], sf::RenderWindow& window, sf::Event mouse) {
	sf::Vector2u size = window.getSize();

	sf::Text mainMenu("Main Menu", GeoSansLight);
	mainMenu.setCharacterSize(72);
	mainMenu.setPosition(size.x / 3.6, size.y / 6);
	mainMenu.setOutlineThickness(5);
	mainMenu.setOutlineColor(sf::Color::Black);


	sf::Text option1(optionNames[0], GeoSansLight);
	option1.setFillColor(sf::Color::Black);
	option1.setStyle(sf::Text::Bold);
	option1.setPosition(sf::Vector2f(size.x / 3 + 12, size.y / 3));
	sf::Text option2(optionNames[1], GeoSansLight);
	option2.setFillColor(sf::Color::Black);
	option2.setStyle(sf::Text::Bold);
	option2.setPosition(sf::Vector2f(size.x / 3 + 12, size.y / 3 + 60.));
	sf::Text option3(optionNames[2], GeoSansLight);
	option3.setFillColor(sf::Color::Black);
	option3.setStyle(sf::Text::Bold);
	option3.setPosition(sf::Vector2f(size.x / 3 + 12, size.y / 3 + 120.));
	sf::Text option4(optionNames[3], GeoSansLight);
	option4.setFillColor(sf::Color::Black);
	option4.setStyle(sf::Text::Bold);
	option4.setPosition(sf::Vector2f(size.x / 3 + 12, size.y / 3 + 180.));

	sf::RectangleShape opt1(sf::Vector2f(260.f, 40.f));
	opt1.setFillColor(sf::Color::White);
	opt1.setOutlineThickness(3.f);
	opt1.setOutlineColor(sf::Color::Black);
	opt1.setPosition(sf::Vector2f(size.x / 3, size.y / 3));
	sf::RectangleShape opt2(sf::Vector2f(260.f, 40.f));
	opt2.setFillColor(sf::Color::White);
	opt2.setOutlineThickness(3.f);
	opt2.setOutlineColor(sf::Color::Black);
	opt2.setPosition(sf::Vector2f(size.x / 3, size.y / 3 + 60.));
	sf::RectangleShape opt3(sf::Vector2f(260.f, 40.f));
	opt3.setFillColor(sf::Color::White);
	opt3.setOutlineThickness(3.f);
	opt3.setOutlineColor(sf::Color::Black);
	opt3.setPosition(sf::Vector2f(size.x / 3, size.y / 3 + 120.));
	sf::RectangleShape opt4(sf::Vector2f(260.f, 40.f));
	opt4.setFillColor(sf::Color::White);
	opt4.setOutlineThickness(3.f);
	opt4.setOutlineColor(sf::Color::Black);
	opt4.setPosition(sf::Vector2f(size.x / 3, size.y / 3 + 180.));

	uiOptions boolsi;
	GPEngine G;

	if (G.MouseClick(opt1, mouse, window)) boolsi.option1 = true;
	if (G.MouseClick(opt2, mouse, window)) boolsi.option2 = true;
	if (G.MouseClick(opt3, mouse, window)) boolsi.option3 = true;
	if (G.MouseClick(opt4, mouse, window)) boolsi.option4 = true;

	window.draw(opt1);
	window.draw(option1);
	window.draw(opt2);
	window.draw(option2);
	window.draw(opt3);
	window.draw(option3);
	window.draw(opt4);
	window.draw(option4);
	window.draw(mainMenu);

	return boolsi;
}

Button::Button() {
	block.setSize(sf::Vector2f(80., 80.));
	block.setFillColor(sf::Color::Red);
	block.setOutlineColor(sf::Color::Black);
	block.setOutlineThickness(2.);
	font.loadFromFile("assets/GeosansLight.ttf");
	text.setFont(font);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(block.getPosition().x + 20., block.getPosition().y - 5));
}
void Button::setSize(double height, double width) {
	this->block.setSize(sf::Vector2f(width, height));
}
void Button::setPosition(double x, double y) {
	this->block.setPosition(sf::Vector2f(x, y));
	text.setPosition(sf::Vector2f(block.getPosition().x + 20., block.getPosition().y - 5));
}
void Button::Draw(sf::RenderWindow & window) {
	window.draw(block);
	window.draw(text);
}
std::string Button::getValue() {
	return text.getString();
}
void Button::setColor(sf::Color color) {
	this->block.setFillColor(color);
}
bool Button::Click(sf::Event event, sf::RenderWindow window) {
	return (Engine.MouseClick(this->block, event, window));
}
bool Button::Hover(sf::RenderWindow window) {
	sf::Vector2f mouse;
	mouse.x = sf::Mouse::getPosition(window).x;
	mouse.y = sf::Mouse::getPosition(window).y;
	return (block.getGlobalBounds().contains(mouse));
}