#include <SFML/Graphics.hpp>
#include <random>

using namespace sf;
using namespace std;

const int H = 25;			// High for window
const int L = 15;			// Length for window

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
		else if (field[a[i].x][a[i].y])					return 0;

	return 1;
}

int main()
{
	// random
	random_device rd;
	mt19937 mersenne(rd());

	// window
	RenderWindow window(VideoMode(L * 20, H * 20), "*Tetris*");

	// for move square
	int dx = 0;						// move
	bool rotate = 0;				// rotate			
	int n = mersenne() % 7;			// type
	int color = 1;					// color
	bool beginGame = true;			// for start
	int size = 20;					// size texture (20x20)

	float timer = 0.f;
	float delay = 0.3f;				// delay
	Clock clock;

	// load texture
	Texture texture;
	texture.loadFromFile("D:\\FuroK\\Visual Studio\\Texture\\square160x20.png");

	// make square
	Sprite square(texture);
	square.setTextureRect(IntRect(0, 0, size, size));

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
				case Keyboard::S:		delay = 0.1f;	break;
				}
			if (event.type == Event::KeyReleased)
				switch (event.key.code) {
				case Keyboard::Down:	delay = 0.3f;	break;
				case Keyboard::S:		delay = 0.3f;	break;
			
				}
		}
		// render
		window.clear(Color::White);
		for (int i = 0; i < H; i++)
			for (int j = 0; j < L; j++) {
				if (field[i][j] == 0) continue;
				square.setTextureRect(IntRect(field[i][j] * size, 0, size, size));
				square.setPosition(static_cast<float>(j * 20), static_cast<float>(i * 20));
				window.draw(square);
			}

		// first render
		for (int i = 0; i < 4; i++) {
			square.setTextureRect(IntRect(color * size, 0, size, size));
			square.setPosition(static_cast<float>(a[i].x * 20), static_cast<float>(a[i].y * 20));
			window.draw(square);
		}

		if (beginGame) {
			beginGame = false;
			n = mersenne() % 7;
			for (int i = 0; i < 4; i++) {
				a[i].x = figures[n][i] % 2;
				a[i].y = figures[n][i] / 2;
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
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = color;
				color = 1 + mersenne() % 7;
				n = mersenne() % 7;
				for (int i = 0; i < 4; i++) {
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}
			timer = 0;
		}

		// rotate tetranimo [x = x0 + (x-x0)cosa - (y-y0)sina]
		if (n == 6) rotate = false;
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
		for (int i = H - 1; i > 0; i--) {
			int temp = 0;
			for (int j = 0; j < L; j++) {
				if (field[i][j])
					temp++;
				field[k][j] = field[i][j];
			}
			if (temp < L)
				k--;
		}

		dx = 0;
		rotate = 0;

		window.display();
	}

	return 0;
}