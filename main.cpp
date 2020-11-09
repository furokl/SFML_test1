#include <SFML/Graphics.hpp>
#include <iostream>


using namespace sf;
using namespace std;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(400, 400), "Petank!");

	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event))	{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear(Color(250, 220, 100));

		CircleShape triangle(50.f, 3);
		triangle.setPosition(75, 75);
		triangle.setFillColor(Color(0, 125, 155));
		window.draw(triangle);

		CircleShape square(50.f, 4);
		square.setPosition(225, 75);
		square.setFillColor(Color(125, 0, 155));
		window.draw(square);

		CircleShape octagone(50.f, 8);
		octagone.setPosition(75, 225);
		octagone.setFillColor(Color(125, 225, 225));
		window.draw(octagone);
		
		CircleShape circle(50.f);
		circle.setPosition(225, 225);
		circle.setFillColor(Color(125, 25, 25));
		window.draw(circle);

		CircleShape circle1(20.f, 100);
		circle1.setPosition(175, 175);
		circle1.setOutlineThickness(5.f);
		circle1.setOutlineColor(Color(0, 0, 0, 50));
		circle1.setFillColor(Color(125, 225, 155));
		window.draw(circle1);

		ConvexShape convex;
		convex.setPointCount(5);

		convex.setPoint(0, Vector2f(30.f, 20.f));
		convex.setPoint(1, Vector2f(80.f, 50.f));
		convex.setPoint(2, Vector2f(110.f, 90.f));
		convex.setPoint(3, Vector2f(70.f, 150.f));
		convex.setPoint(4, Vector2f(10.f, 170.f));

		convex.setFillColor(Color(50, 100, 25));
		window.draw(convex);

		RectangleShape rectangle(Vector2f(100.f, 100.f));
		rectangle.move(300, 300);
		rectangle.setFillColor(Color(100, 255, 0, 100));
		window.draw(rectangle);

		Vertex line_without_thickness[] =
		{
			Vertex(Vector2f(0.f, 0.f)),
			Vertex(Vector2f(400.f, 400.f))
		};

		line_without_thickness->color = Color::Black;
		window.draw(line_without_thickness, 2, Lines);

		RectangleShape line_with_thickness(Vector2f(600.f, 2.f));
		line_with_thickness.rotate(135.f);
		line_with_thickness.setPosition(400, 0);
		line_with_thickness.setFillColor(Color::Black);
		window.draw(line_with_thickness);

		window.display();
	}

	return 0;
}