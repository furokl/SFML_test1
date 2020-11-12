#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <random>

const int H = 18;			// High for work field
const int L = 10;			// Length for work field

int field[H][L] = { 0 };

int figures[7][4] =
{
	1, 3, 5, 7,		// |	
	2, 4, 5, 7,		// S
	3, 4, 5, 6,		// S'
	3, 4, 5, 7,		// T
	2, 3, 5, 7,		// L
	3, 5, 6, 7,		// L'
	2, 3, 4, 5,		// []
};

struct Point {
	int x, y;
}	a[4], b[4];

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= L || a[i].y >= H)	return 0;
		else if (field[a[i].y][a[i].x])					return 0;

	return 1;
}

int main()
{
	// random
	std::random_device rd;
	std::mt19937 mersenne(rd());

	// window 
	sf::RenderWindow window(sf::VideoMode(240, 440), "*Tetris*");

	// for move square
	float score = 0.f;				// score
	float x_score = 1.f;			// multiplier
	int spawn = mersenne() % 7 + 1;	// spawn
	int dx = 0;						// move		
	int n = mersenne() % 7;			// type
	int color = mersenne() % 8 + 1;	// color
	int size = 20;					// size texture (20x20)
	bool rotate = false;			// rotate
	bool beginGame = true;			// for start
	bool pauseGame = true;			// pause
	bool loseGame = false;


	float timer = 0.f;
	float delay = 0.3f;				// delay
	sf::Clock clock;

	// load texture
	sf::Texture texture, backgroung;
	texture.loadFromFile("C:\\Tetris\\Redist\\square160x20.png");
	backgroung.loadFromFile("C:\\Tetris\\Redist\\fon.png");

	sf::Sprite square(texture), sprite_background(backgroung);

	// loaf font
	sf::Font font;
	font.loadFromFile("C:\\Tetris\\Redist\\SingleDayRegular.ttf");

	sf::Text text, text2;
	text.setFont(font);
	text2.setFont(font);

	// load sound fx
	sf::SoundBuffer sample;
	sample.loadFromFile("C:\\Tetris\\Redist\\pong.wav");

	sf::Sound pong;
	pong.setBuffer(sample);
	pong.setVolume(20);

	sf::Music music;
	music.openFromFile("Ñ:\\Tetris\\Redist\\sosnin.ogg");
	music.play();
	music.setVolume(20);
	music.setLoop(true);

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				switch (event.key.code) {
				case sf::Keyboard::Escape:	window.close();	break;
				case sf::Keyboard::Left:	dx = -1;		break;
				case sf::Keyboard::Right:	dx = 1;			break;
				case sf::Keyboard::Up:		rotate = true;	break;
				case sf::Keyboard::Down:	delay = 0.05f;	break;
				case sf::Keyboard::A:		dx = -1;		break;
				case sf::Keyboard::D:		dx = 1;			break;
				case sf::Keyboard::W:		rotate = true;	break;
				case sf::Keyboard::S:		delay = 0.05f;	break;
				}
			if (event.type == sf::Event::KeyReleased)
				switch (event.key.code) {
				case sf::Keyboard::Down:	delay = 0.3f;	break;
				case sf::Keyboard::S:		delay = 0.3f;	break;	
				}
			if (event.type == sf::Event::LostFocus)
				pauseGame = false;
			if (event.type == sf::Event::GainedFocus)
				pauseGame = true;
		}
		window.clear(sf::Color::White);
		window.draw(sprite_background);
		window.draw(text);
		window.draw(text2);
		// field render
		for (int i = 0; i < H; i++)
			for (int j = 0; j < L; j++) {
				if (field[i][j] == 0) continue;
				square.setTextureRect(sf::IntRect((field[i][j] - 1) * size, 0, size, size));
				square.setPosition(j * 20, i * 20);
				square.move(20, 20);
				window.draw(square);
			}
		// detail render
		for (int i = 0; i < 4; i++) {
			square.setTextureRect(sf::IntRect((color - 1) * size, 0, size, size));
			square.setPosition(a[i].x * 20, a[i].y * 20);
			square.move(20, 20);
			window.draw(square);
		}

		// when you dont lose
		if (!loseGame) {
			// when window is only open
			if (pauseGame) {
				// first figure
				if (beginGame) {
					beginGame = false;
					n = mersenne() % 7;
					for (int i = 0; i < 4; i++) {
						a[i].x = figures[n][i] % 2;
						a[i].y = figures[n][i] / 2;
						a[i].x += spawn;
					}
				}

				// move tetranimo
					// right + left
				for (int i = 0; i < 4; i++) {
					b[i] = a[i];
					a[i].x += dx;
				}
				if (!check()) {
					for (int i = 0; i < 4; i++)
						a[i] = b[i];
				}
				// down
				if (timer > delay) {
					for (int i = 0; i < 4; i++) {
						b[i] = a[i];
						a[i].y += 1;
					}
					if (!check()) {
						for (int i = 0; i < 4; i++) {
							field[b[i].y][b[i].x] = color;
							if (b[i].y <= 3) {
								text2.setPosition(15, 5);
								text2.setString("YOU LOSE :(");
								text2.setFillColor(sf::Color::Red);
								text2.setCharacterSize(42);
								loseGame = true;
								music.stop();
							}
						}
						spawn = mersenne() % 7 + 1;
						color = mersenne() % 8 + 1;
						n = mersenne() % 7;
						for (int i = 0; i < 4; i++) {
							a[i].x = figures[n][i] % 2;
							a[i].y = figures[n][i] / 2;
							a[i].x += spawn;
						}
					}
					timer = 0;
				}

				// rotate tetranimo			
				if (n == 6) rotate = false;			// block for type n = 6 []
				if (rotate) {
					Point p = a[1];
					for (int i = 0; i < 4; i++) {
						int x = a[i].y - p.y;		// y - y0
						int y = a[i].x - p.x;		// x - x0
						a[i].x = p.x - x;
						a[i].y = p.y + y;
					}
					if (!check()) {
						for (int i = 0; i < 4; i++)
							a[i] = b[i];
					}
				}

				// delete tetranimo
				int k = H - 1;
				for (int i = k; i > 0; i--) {
					int temp = 0;
					for (int j = 0; j < L; j++) {
						if (field[i][j])
							temp++;
						field[k][j] = field[i][j];
					}
					if (temp < L)
						k--;
					else {
						pong.play();
						x_score += 0.1111f;
						score += 1 * x_score;
					}
				}

				dx = 0;
				rotate = 0;
			}
			// end if pause
		}
		// end if lose
		text.setPosition(55, 392);
		text.setString("Score: " + std::to_string(score));
		text.setCharacterSize(24);
		window.display();
	}
	return 0;
}