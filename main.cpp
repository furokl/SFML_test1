#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

using namespace sf;
using namespace std;

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
	random_device rd;
	mt19937 mersenne(rd());

	// window
	RenderWindow window(VideoMode(240, 440), "*Tetris*");

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
	Clock clock;

	// load texture
	Texture texture, backgroung;
	texture.loadFromFile("D:\\FuroK\\Visual Studio\\Texture\\square160x20.png");
	backgroung.loadFromFile("D:\\FuroK\\Visual Studio\\Texture\\fon.png");

	Sprite square(texture), sprite_background(backgroung);

	// loaf font
	Font font;
	font.loadFromFile("D:\\FuroK\\Visual Studio\\Font\\SingleDayRegular.ttf");

	Text text, text2;
	text.setFont(font);
	text2.setFont(font);

	// load sound fx
	SoundBuffer sample;
	sample.loadFromFile("D:\\FuroK\\Visual Studio\\Sound\\pong.wav");

	Sound pong;
	pong.setBuffer(sample);
	pong.setVolume(20);

	Music music;
	music.openFromFile("D:\\FuroK\\Visual Studio\\Sound\\sosnin.ogg");
	music.play();
	music.setVolume(20);
	music.setLoop(true);

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				switch (event.key.code) {
				case Keyboard::Escape:	window.close();	break;
				case Keyboard::Left:	dx = -1;		break;
				case Keyboard::Right:	dx = 1;			break;
				case Keyboard::Up:		rotate = true;	break;
				case Keyboard::Down:	delay = 0.05f;	break;
				case Keyboard::A:		dx = -1;		break;
				case Keyboard::D:		dx = 1;			break;
				case Keyboard::W:		rotate = true;	break;
				case Keyboard::S:		delay = 0.05f;	break;
				}
			if (event.type == Event::KeyReleased)
				switch (event.key.code) {
				case Keyboard::Down:	delay = 0.3f;	break;
				case Keyboard::S:		delay = 0.3f;	break;
			
				}
			if (event.type == Event::LostFocus)
				pauseGame = false;
			if (event.type == Event::GainedFocus)
				pauseGame = true;
		}
		window.clear(Color::White);
		window.draw(sprite_background);
		window.draw(text);
		window.draw(text2);
		// field render
		for (int i = 0; i < H; i++)
			for (int j = 0; j < L; j++) {
				if (field[i][j] == 0) continue;
				square.setTextureRect(IntRect((field[i][j] - 1) * size, 0, size, size));
				square.setPosition(j * 20, i * 20);
				square.move(20, 20);
				window.draw(square);
			}
		// detail render
		for (int i = 0; i < 4; i++) {
			square.setTextureRect(IntRect((color - 1) * size, 0, size, size));
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
								text2.setFillColor(Color::Red);
								text2.setCharacterSize(42);
								loseGame = true;
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
		text.setString("Score: " + to_string(score));
		text.setCharacterSize(24);
		window.display();
	}
	return 0;
}