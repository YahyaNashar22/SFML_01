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

sf::Vector2f normalize(const sf::Vector2f& v)
{
	float len = length(v);
	if (len == 0.f)
	{
		return { 0.f, 0.f };
	}

	return v / len;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML_01");
	sf::Clock clock;

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

	sf::Vector2f speed(-250.f, -180.f);
	sf::Vector2f speed2(250.f, 180.f);



	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		sf::Time dt = clock.restart(); // get elapsed time

		circle.move(speed * dt.asSeconds());
		circle2.move(speed2 * dt.asSeconds());

		
		sf::FloatRect circleBoundingBox = circle.getGlobalBounds();
		sf::FloatRect circleBoundingBox2 = circle2.getGlobalBounds();

		// collision with wall for circle 1
		if (circleBoundingBox.position.x + circleBoundingBox.size.x > windowWidth)
		{
			speed.x = -std::abs(speed.x);
		}
		if (circleBoundingBox.position.x < 0.f)
		{
			speed.x = std::abs(speed.x);
		}
		if (circleBoundingBox.position.y < 0.f)
		{
			speed.y = std::abs(speed.y);
		}
		if (circleBoundingBox.position.y + circleBoundingBox.size.y > windowHeight)
		{
			speed.y = -std::abs(speed.y);
		}
	
		// collision with wall for circle 2
		if (circleBoundingBox2.position.x + circleBoundingBox2.size.x > windowWidth)
		{
			speed2.x = -std::abs(speed2.x);
		}
		if (circleBoundingBox2.position.x < 0.f)
		{
			speed2.x = std::abs(speed2.x);
		}
		if (circleBoundingBox2.position.y < 0.f)
		{
			speed2.y = std::abs(speed2.y);
		}
		if (circleBoundingBox2.position.y + circleBoundingBox2.size.y > windowHeight)
		{
			speed2.y = -std::abs(speed2.y);
		}

		// Circle-Circle Collision
		if (const std::optional intersection = circleBoundingBox.findIntersection(circleBoundingBox2))
		{
			sf::Vector2f pos1 = circle.getPosition();
			sf::Vector2f pos2 = circle2.getPosition();

			sf::Vector2f normal = normalize(pos1 - pos2);
			sf::Vector2f relativeVelocity = speed - speed2;

			float velAlongNormal = dot(relativeVelocity, normal);

			// only resolve if moving toward each other
			if (velAlongNormal)
			{
				// Equal mass, elastic collision
				speed -= velAlongNormal * normal;
				speed2 += velAlongNormal * normal;
			}

			// Separate them to reduce sticking
			float overlapX = intersection->size.x;
			float overlapY = intersection->size.y;
			float overlap = std::min(overlapX, overlapY);

			circle.move(normal * (overlap * 0.5f));
			circle2.move(-normal * (overlap * 0.5f));
		}


		window.clear(sf::Color::Black);
		window.draw(circle);
		window.draw(circle2);
		window.display();
	}

	return 0;
}

