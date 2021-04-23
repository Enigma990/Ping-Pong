#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<sstream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	std::cout << "Game started" << std::endl;

	// Font file
	sf::Font font;
	if (font.loadFromFile("Assets/Beon-Regular.otf") == false)
	{
		std::cout << "Font Loading Failed" << std::endl;
	}

	//Text
	sf::Text score;
	score.setFont(font);
	score.setCharacterSize(30);
	score.setPosition(380, 10);
	score.setFillColor(sf::Color::Red);
	score.setString("0 : 0");

	//Texture
	sf::Texture texPad;
	sf::Texture texBall;
	sf::Texture texBackground;

	if (texPad.loadFromFile("Assets/pad.png") == false)
		return 1;
	if (texBall.loadFromFile("Assets/ball.png") == false)
		return 1;
	if (texBackground.loadFromFile("Assets/background.png") == false)
		return 1;


	//Sounds
	sf::SoundBuffer buffHit;
	if (buffHit.loadFromFile("Assets/hit.wav") == false)
		return 1;

	sf::Sound hit;
	hit.setBuffer(buffHit);

	//States
	bool up = false;
	bool down = false;


	//Variables
	int yVelocityPad1 = 0;
	int xVelocityBall = -3;
	int yVelocityBall = -3;
	int yVelocityPad2 = 0;

	int pad1Score = 0;
	int pad2Score = 0;

	//Shapes 
	//Background
	sf::RectangleShape background(sf::Vector2f(800, 600));
	background.setPosition(0, 0);
	background.setTexture(&texBackground);

	//Pad1
	sf::RectangleShape pad1(sf::Vector2f(50, 100));
	pad1.setPosition(50, 200);
	pad1.setTexture(&texPad);

	//Pad2
	sf::RectangleShape pad2(sf::Vector2f(50, 100));
	pad2.setPosition(700, 200);
	pad2.setTexture(&texPad);

	//Ball
	sf::RectangleShape ball(sf::Vector2f(50, 50));
	ball.setPosition(400, 200);
	ball.setTexture(&texBall);


	while (window.isOpen())
	{
		// Event part
		sf::Event event;

		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			//Key Pressed
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					up = true;
				}

				if (event.key.code == sf::Keyboard::Down)
				{
					down = true;
				}
			}

			//Key Released
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					up = false;
				}

				if (event.key.code == sf::Keyboard::Down)
				{
					down = false;
				}
			}
		}

		// Logic handling
		//Pad1 Logic
		if (up == true)
		{
			yVelocityPad1 = -5;
		}
		if (down == true)
		{
			yVelocityPad1 = 5;
		}
		if (up == true && down == true)
		{
			yVelocityPad1 = 0;
		}
		if (up == false && down == false)
		{
			yVelocityPad1 = 0;
		}

		//Movement
		pad1.move(0, yVelocityPad1);

		//Pad Out of bound check
		if (pad1.getPosition().y < 0 || pad1.getPosition().y > 500)
		{
			pad1.move(0, -yVelocityPad1);
		}

		//Pad2 Logic
		if (ball.getPosition().y < pad2.getPosition().y)
		{
			yVelocityPad2 = -3;
		}
		if (ball.getPosition().y > pad2.getPosition().y)
		{
			yVelocityPad2 = 3;
		}

		pad2.move(0, yVelocityPad2);


		//Ball Logic
		ball.move(xVelocityBall, yVelocityBall);

		//Ball out of bound check
		if (ball.getPosition().y < 0 || ball.getPosition().y > 550)
		{
			yVelocityBall = -yVelocityBall;
		}
		
		if (ball.getPosition().x < -50)
		{
			pad2Score++;
			ball.setPosition(300, 200);
			xVelocityBall = 3;
			yVelocityBall = 3;
		}
		if (ball.getPosition().x > 800)
		{
			pad1Score++;
			ball.setPosition(300, 200);
			xVelocityBall = -3;
			yVelocityBall = -3;
		}

		//Ball Collision
		if (ball.getGlobalBounds().intersects(pad1.getGlobalBounds()) == true || ball.getGlobalBounds().intersects(pad2.getGlobalBounds()) == true)
		{
			xVelocityBall = -xVelocityBall;
			hit.play();

			if (xVelocityBall < 0)
				xVelocityBall -= 1;
			else
				xVelocityBall += 1;
			if (yVelocityBall < 0)
				yVelocityBall -= 1;
			else
				yVelocityBall += 1;
		}

		// Rendering part
		window.clear();

		window.draw(background);
		window.draw(pad1);
		window.draw(pad2);
		window.draw(ball);
		
		std::stringstream text;
		text << pad1Score << " : " << pad2Score;
		score.setString(text.str());
		window.draw(score);

		window.display();

	}

	return 0;
}