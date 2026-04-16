#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include <imgui.h>
#include <imgui-SFML.h>


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
	std::fstream fin("config.txt");
	std::string key;
	int value;

	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;

	while (fin >> key >> value)
	{
		if (key == "windowWidth")
		{
			windowWidth = value;
		}
		if (key == "windowHeight")
		{
			windowHeight = value;
		}
	}


	sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "SFML_01");
	if (!ImGui::SFML::Init(window))
	{
		std::cerr << "Failed to initialize ImGui-SFML\n";
		return 1;
	}

	sf::Clock clock;
	
	float circleRadius = 50.f;
	int circleSegments = 100;
	float circleColor[3] = { (float)204 / 255, (float)204 / 255, (float)204 / 255 };
	sf::CircleShape circle(circleRadius, circleSegments);
	circle.setFillColor(sf::Color(circleColor[0] * 255,circleColor[1] * 255,circleColor[2] * 255 ));
	circle.setOutlineThickness(10.f);
	circle.setOutlineColor(sf::Color(250, 150, 100));
	circle.setPosition(sf::Vector2f(windowWidth - 100, windowHeight / 2));
	circle.setOrigin({circleRadius, circleRadius});
	bool circleExist = true;

	sf::CircleShape circle2(30.f);
	circle2.setFillColor(sf::Color(150, 150, 150));
	circle2.setOutlineThickness(10.f);
	circle2.setOutlineColor(sf::Color(50, 50, 50));
	circle2.setPosition(sf::Vector2f(100, windowHeight / 2));
	circle2.setOrigin({ 30.f, 30.f });

	sf::Vector2f speed(-250.f, -180.f);
	sf::Vector2f speed2(250.f, 180.f);


	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		sf::Time dt = clock.restart(); // get elapsed time

		ImGui::SFML::Update(window, dt);

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
			if (velAlongNormal < 0.f)
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

		ImGui::Begin("Debug");
		ImGui::Checkbox("Circle", &circleExist);
		ImGui::SliderFloat("Circle Radius", &circleRadius, 50.f, 200.f);
		ImGui::SliderInt("Circle Segments", &circleSegments, 3, 150);
		ImGui::ColorEdit3("Circle Color", circleColor);
		ImGui::End();

		circle.setRadius(circleRadius);
		circle.setPointCount(circleSegments);
		circle.setFillColor(sf::Color(circleColor[0] * 255, circleColor[1] * 255, circleColor[2] * 255));
		circle.setOrigin({circleRadius, circleRadius});

		window.clear(sf::Color::Black);
		if (circleExist)
		{
			window.draw(circle);
		}
		window.draw(circle2);

		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}

