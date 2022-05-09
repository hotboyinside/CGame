#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>



int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));


	const float pi = 3.14159f;
	const int gameWidth = 800;
	const int gameHeight = 600;
	sf::Vector2f paddleSize(25, 100);
	float ballRadius = 10.f;

	sf::Texture t1;
	t1.loadFromFile("C:/Users/Acer/Downloads/cifr.png");

	
	sf::Sprite score[2];
	int sc[2] = { 0, 0 };
	for (int i = 0; i < 2; i++) {
		score[i].setTexture(t1);
		score[i].setPosition(749 * i, 10);
	}


	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), L"Пинг-понг",
		sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);


	sf::SoundBuffer ballSoundBuffer;
	if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
		return EXIT_FAILURE;
	sf::Sound ballSound(ballSoundBuffer);


	sf::RectangleShape leftPaddle;
	leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
	leftPaddle.setOutlineThickness(3);
	leftPaddle.setOutlineColor(sf::Color::Black);
	leftPaddle.setFillColor(sf::Color(100, 100, 200));
	leftPaddle.setOrigin(paddleSize / 2.f);


	sf::RectangleShape rightPaddle;
	rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
	rightPaddle.setOutlineThickness(3);
	rightPaddle.setOutlineColor(sf::Color::Black);
	rightPaddle.setFillColor(sf::Color(200, 100, 100));
	rightPaddle.setOrigin(paddleSize / 2.f);


	sf::CircleShape ball;
	ball.setRadius(ballRadius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(sf::Color::Black);
	ball.setFillColor(sf::Color::White);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);


	sf::Font font;
	if (!font.loadFromFile("C:/Users/Acer/Desktop/игра/BoncegroFF4F.otf"))
		return EXIT_FAILURE;


	sf::Text pauseMessage;
	pauseMessage.setFont(font);
	pauseMessage.setCharacterSize(40);
	pauseMessage.setPosition(170.f, 150.f);
	pauseMessage.setFillColor(sf::Color::White);
	pauseMessage.setString(L"Добро пожаловать в Пинг-понг!\nНажмите space чтобы начать игру!");


	sf::Clock AITimer;
	const sf::Time AITime = sf::seconds(0.1f);
	const float paddleSpeed = 400.f;
	float rightPaddleSpeed = 0.f;
	const float ballSpeed = 400.f;
	float ballAngle = 0.f; 

	sf::Clock clock;
	bool isPlaying = false;
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{

			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				break;
			}


			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				if (!isPlaying)
				{

					isPlaying = true;
					clock.restart();

	
					leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
					rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
					ball.setPosition(gameWidth / 2, gameHeight / 2);

	
					do
					{

						ballAngle = (std::rand() % 360) * 2 * pi / 360;
					} while (std::abs(std::cos(ballAngle)) < 0.7f);
				}
			}
		}

		if (isPlaying)
		{
			float deltaTime = clock.restart().asSeconds();


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
				(leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
			{
				leftPaddle.move(0.f, -paddleSpeed * deltaTime);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
				(leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
			{
				leftPaddle.move(0.f, paddleSpeed * deltaTime);
			}


			if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
				((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
			{
				rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
			}


			if (AITimer.getElapsedTime() > AITime)
			{
				AITimer.restart();
				if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
					rightPaddleSpeed = paddleSpeed;
				else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
					rightPaddleSpeed = -paddleSpeed;
				else
					rightPaddleSpeed = 0.f;
			}


			float factor = ballSpeed * deltaTime;
			ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);


			if (ball.getPosition().x - ballRadius < 0.f)
			{
				sc[1]++;
				isPlaying = false;
				pauseMessage.setString(L"Ты проиграл!\nНажми space чтобы начать заново\nили escape чтобы выйти");
			}
			if (ball.getPosition().x + ballRadius > gameWidth)
			{
				sc[0]++;
				isPlaying = false;
				pauseMessage.setString(L"Ты выиграл!\nНажми space чтобы начать заново\nили escape чтобы выйти");
			}
			if (ball.getPosition().y - ballRadius < 0.f)
			{
				ballSound.play();
				ballAngle = -ballAngle;
				ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
			}
			if (ball.getPosition().y + ballRadius > gameHeight)
			{
				ballSound.play();
				ballAngle = -ballAngle;
				ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
			}

			if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
				ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
				ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
			{
				if (ball.getPosition().y > leftPaddle.getPosition().y)
					ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
				else
					ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

				ballSound.play();
				ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
			}

			if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
				ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
				ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
			{
				if (ball.getPosition().y > rightPaddle.getPosition().y)
					ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
				else
					ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

				ballSound.play();
				ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
			}
		}
		for (int i = 0; i < 2; i++) {
			if (sc[i] < 10)
				score[i].setTextureRect(sf::IntRect(50 * sc[i], 0, 50, 75));
			else if (sc[i] == 10)
				score[i].setTextureRect(sf::IntRect(500, 0, 75, 75));
			else if (sc[i] == 11)
				score[i].setTextureRect(sf::IntRect(580, 0, 75, 75));
		}


		window.clear(sf::Color(50, 200, 50));

		if (isPlaying)
		{
			window.draw(leftPaddle);
			window.draw(rightPaddle);
			window.draw(ball);
			for (int i = 0; i < 2; i++)
				window.draw(score[i]);
		}
		else
		{
			window.draw(pauseMessage);
		}

		window.display();
	}

	return EXIT_SUCCESS;
}
