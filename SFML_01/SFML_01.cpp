#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>


float dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

float length(const sf::Vector2f& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML_01");
	sf::Clock clock;
	sf::Vector2f speed(250.f, 180.f);
	sf::Vector2f speed2(250.f, 180.f);

	sf::Vector2u windowSize = window.getSize();
	float windowWidth = static_cast<float>(windowSize.x);
	float windowHeight = static_cast<float>(windowSize.y);


	sf::CircleShape circle(50.f);
	circle.setFillColor(sf::Color(100, 250, 50));
	circle.setOutlineThickness(10.f);
	circle.setOutlineColor(sf::Color(250, 150, 100));
	circle.setPosition(sf::Vector2f(windowWidth - 100, windowHeight / 2));
	circle.setOrigin({50.f, 50.f});

	sf::CircleShape circle2(30.f);
	circle2.setFillColor(sf::Color(150, 150, 150));
	circle2.setOutlineThickness(10.f);
	circle2.setOutlineColor(sf::Color(50, 50, 50));
	circle2.setPosition(sf::Vector2f(100, windowHeight / 2));
	circle2.setOrigin({30.f, 30.f});


	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		sf::Time dt = clock.restart(); // get elapsed time

		window.draw(circle);
		window.draw(circle2);

		circle.move(speed * dt.asSeconds());
		circle2.move(speed2 * dt.asSeconds());

		
		sf::FloatRect circleBoundingBox = circle.getGlobalBounds();
		sf::FloatRect circleBoundingBox2 = circle2.getGlobalBounds();

		if (circleBoundingBox.position.x + circleBoundingBox.size.x > windowWidth)
		{
			speed *= -1.f;
		}
		if (circleBoundingBox.position.x < 0)
		{
			speed *= -1.f;
		}
		if (circleBoundingBox.position.y < 0)
		{
			speed *= -1.f;
		}
		if (circleBoundingBox.position.y + circleBoundingBox.size.y > windowHeight)
		{
			speed *= -1.f;
		}
	

		if (circleBoundingBox2.position.x + circleBoundingBox2.size.x > windowWidth)
		{
			speed2 *= -1.f;
		}
		if (circleBoundingBox2.position.x < 0)
		{
			speed2 *= -1.f;
		}
		if (circleBoundingBox2.position.y < 0)
		{
			speed2 *= -1.f;
		}
		if (circleBoundingBox2.position.y + circleBoundingBox2.size.y > windowHeight)
		{
			speed2 *= -1.f;
		}

		if (const std::optional intersection = circleBoundingBox.findIntersection(circleBoundingBox2))
		{
			speed *= -1.f;
			speed2 *= -1.f;
		}

		window.display();
	}

	return 0;
}

