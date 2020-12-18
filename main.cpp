#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <random>

std::random_device rd;
std::mt19937 mersenne(rd());

const int H = 18;			// Высота поля
const int L = 10;			// Длина поля

// поле, в котором будут двигаться тетранимо
int field[H][L];

// запись фигур тетранимо
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

// структура тетранимо
struct Point {
	int x, y;
}	a[4], b[4];

bool check() {
	for (int i = 0; i < 4; i++)
		// если вышли за границы
		if (a[i].x < 0 || a[i].x >= L || a[i].y >= H)	return 0;
		// если встретились с полем ненулевого цвета
		else if (field[a[i].y][a[i].x])					return 0;

	return 1;
}

float
	bestresult = 0.f,			// лучший результат
	score = 0.f,				// очки
	x_score = 1.f,				// множитель
	timer = 0.f,				// таймер
	delay = 0.3f;				// задержка
int
	spawn = mersenne() % 7 + 1,	// точка появления
	dx = 0,						// горизонтальное перемещение	
	n = mersenne() % 7,			// тип тетранимо
	color = mersenne() % 8 + 1,	// цвет тетранимо
	size = 20;					// размер текстуры (20x20)
bool
	rotate = false,				// вращение
	beginGame = true,			// начало игры
	pauseGame = false,			// пауза
	looseGame = false,			// поражение
	restartGame = false,		// перезапуск
	muteMusic = false;			// заглушить музыку

int main()
{
	// создаем размер окна
	sf::RenderWindow window(sf::VideoMode(240, 440), "*Tetris*");

	// создаем таймер
	sf::Clock clock;

	// загружаем текстуры
	sf::Texture texture, backgroung;
	texture.loadFromFile("C:\\Tetris\\Redist\\square160x20.png");
	backgroung.loadFromFile("C:\\Tetris\\Redist\\fon.png");

	sf::Sprite square(texture), sprite_background(backgroung);

	// загружаем шрифты
	sf::Font font;
	font.loadFromFile("C:\\Tetris\\Redist\\SingleDayRegular.ttf");

	sf::Text text_score, text_loose, text_restart;
	text_score.setFont(font);
	text_loose.setFont(font);
	text_restart.setFont(font);

	// загружаем аудио
		// звук
	sf::SoundBuffer sample;
	sample.loadFromFile("C:\\Tetris\\Redist\\pong.wav");

	sf::Sound pong;
	pong.setBuffer(sample);
	pong.setVolume(20);

		// основная музыка
	sf::Music music;
	music.openFromFile("C:\\Tetris\\Redist\\lofi.ogg");
	music.play();
	music.setLoop(true);

		// музыка при поражении
	sf::Music looseMusic;
	looseMusic.openFromFile("C:\\Tetris\\Redist\\Deadmau5.ogg");

	// пока окно открыто
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// создаем событие
		sf::Event event;
		while (window.pollEvent(event)) {

			// если пользователь закрыл окно
			if (event.type == sf::Event::Closed)
				window.close();

			// если нажали клавишу
			if (event.type == sf::Event::KeyPressed)
				switch (event.key.code) {
				case sf::Keyboard::Escape:	
					window.close();	
					break;
				case sf::Keyboard::Left:
					dx = -1;		
					break;
				case sf::Keyboard::Right:
					dx = 1;			
					break;
				case sf::Keyboard::Up:
					rotate = true;		
					break;
				case sf::Keyboard::Down:
					delay = 0.05f;		
					break;
				case sf::Keyboard::A:
					dx = -1;			
					break;
				case sf::Keyboard::D:	
					dx = 1;				
					break;
				case sf::Keyboard::W:	
					rotate = true;		
					break;
				case sf::Keyboard::S:	
					delay = 0.05f;		
					break;
				case sf::Keyboard::R:	
					restartGame = true;	
					break;
				case sf::Keyboard::M:
					(muteMusic) ? 
						muteMusic = false: 
						muteMusic = true; 	
					break;
				}

			// если клавиша отжата
			if (event.type == sf::Event::KeyReleased)
				switch (event.key.code) {
				case sf::Keyboard::Down:	
					delay = 0.3f;
					break;
				case sf::Keyboard::S:
					delay = 0.3f;
					break;
				}

			// если пользователь свернул окно
			if (event.type == sf::Event::LostFocus) {
				pauseGame = true;
				if (!looseGame)
					if (!muteMusic)
						music.setVolume(3);
			}

			// если пользователь взаимодействует с окном
			if (event.type == sf::Event::GainedFocus) {
				pauseGame = false;
				if (!looseGame)
					if (!muteMusic)
						music.setVolume(15);
			}
		}

		// отрисовка текста
		window.clear(sf::Color::White);
		window.draw(sprite_background);
		window.draw(text_score);
		window.draw(text_loose);
		window.draw(text_restart);

		// перезапустить игру
		if (restartGame) {
			restartGame = false;
			if (looseGame) {
				// убираем текст рестарта
				text_restart.setString("");

				// фиксируем не поражение
				looseGame = false;

				// возвращаем полю нулевое значение цвета
				for (int i = 0; i < H; i++)
					for (int j = 0; j < L; j++)
						field[i][j] = 0;

				// музыка заглушена?
				if (!muteMusic) {
					music.setVolume(15);
					looseMusic.stop();
				}

				// перезаписываем надпись "рестарт" на лучший результат
				text_loose.setFillColor(sf::Color::White);
				text_loose.setString("" + std::to_string(bestresult));

				// сброс счетчика очков
				x_score = 1.f;
				score = 0;
			}
		}

		// отрисовка поля
		for (int i = 0; i < H; i++)
			for (int j = 0; j < L; j++) {
				if (field[i][j] == 0) continue;
				square.setTextureRect(sf::IntRect((field[i][j] - 1) * size, 0, size, size));
				square.setPosition(j * 20, i * 20);
				square.move(20, 20);
				window.draw(square);
			}

		// отрисовка фигуры
		for (int i = 0; i < 4; i++) {
			square.setTextureRect(sf::IntRect((color - 1) * size, 0, size, size));
			square.setPosition(a[i].x * 20, a[i].y * 20);
			square.move(20, 20);
			window.draw(square);
		}

		// если не поражение
		if (!looseGame) {
			// если не пауза
			if (!pauseGame) {
				// устанавливаем громкость музыки
				(!muteMusic) ?
					music.setVolume(15):
					music.setVolume(0);

				// генерируем первую фигуру
				if (beginGame) {
					beginGame = false;
					n = mersenne() % 7;
					for (int i = 0; i < 4; i++) {
						a[i].x = figures[n][i] % 2;
						a[i].y = figures[n][i] / 2;
						a[i].x += spawn;
					}
				}

				// горизонтальное перемещение
				for (int i = 0; i < 4; i++) {
					b[i] = a[i];
					a[i].x += dx;
				}

				// если противоречит условию, возвращаемся
				if (!check()) {
					for (int i = 0; i < 4; i++)
						a[i] = b[i];
				}

				// движение вниз
				if (timer > delay) {	// если таймер превысил значение задержки
					// сохраняем предыдущее значение и спускаемся вниз
					for (int i = 0; i < 4; i++) {
						b[i] = a[i];
						a[i].y += 1;
					}

					// если противоречит условию
					if (!check()) {
						// фиксируем фигуру в поле
						for (int i = 0; i < 4; i++) {
							field[b[i].y][b[i].x] = color;

							// если фигуры достигли границы (проигрыш)
							if (b[i].y <= 3) {		
								text_restart.setPosition(50, -1);
								text_restart.setFillColor(sf::Color::Black);
								text_restart.setCharacterSize(16);
								text_restart.setString("press 'R' for restart...");
								text_loose.setPosition(20, 5);
								text_loose.setString("YOU LOSE :(");
								text_loose.setFillColor(sf::Color::Red);
								text_loose.setCharacterSize(42);

								// музыка, при поражении
								if (!muteMusic) {
									looseMusic.play();
									looseMusic.setVolume(15);
								}

								// глушим основную музыку
								music.setVolume(0);

								// фиксируем поражение
								looseGame = true;

								// фиксируем лучший результат
								if (bestresult < score)
									bestresult = score;
							}
						}

						// генерируем новую фигуру
						spawn = mersenne() % 7 + 1;
						color = mersenne() % 8 + 1;
						n = mersenne() % 7;
						for (int i = 0; i < 4; i++) {
							a[i].x = figures[n][i] % 2;
							a[i].y = figures[n][i] / 2;
							a[i].x += spawn;
						}
					}

					// сброс таймера
					timer = 0;
				}

				// вращение фигуры		
				if (n == 6) rotate = false;	// блокируем вращение для квадрата
				if (rotate) {
					Point p = a[1];			// создаем центр вращения

					/*	Вращение по формуле:
					*	X = x0 + (x − x0) * cos⁡(a) − (y − y0) * sin⁡(a);
					*	Y = y0 + (y − y0) * cos⁡(a) + (x − x0) * sin⁡(a);
					*
					*	при a = 90 -> sin(90) = 1, cos(90) = 0.
					*	X = x0 − (y − y0)
					*	Y = y0 + (x − x0)
					*/
					for (int i = 0; i < 4; i++) {
						int x = a[i].y - p.y;	// y - y0
						int y = a[i].x - p.x;	// x - x0
						a[i].x = p.x - x;		// x0 − (y − y0)
						a[i].y = p.y + y;		// y0 + (x − x0)
					}

					// если противоречит условию, возвращаем как было
					if (!check()) {
						for (int i = 0; i < 4; i++)
							a[i] = b[i];
					}
				}

				// ломаем линию
				int k = H - 1;
				int temp1 = 0; // счетчик сломанных линий
				for (int i = k; i > 0; i--) {
					int temp = 0;
					for (int j = 0; j < L; j++) {
						if (field[i][j])
							temp++;
						field[k][j] = field[i][j];
					}
					if (temp < L)
						k--;
					if (temp == L)
						temp1++;
				}

				// присваивание очков
				switch (temp1) {
				case 1:
					pong.play();
					x_score += 0.1111f;
					score += x_score;	
					break;
				case 2:
					pong.play();
					x_score += 0.3333f;
					score += x_score * 2;
					break;
				case 3:
					pong.play();
					x_score += 0.5555f;
					score += x_score * 3;
					break;
				case 4:
					pong.play();
					x_score += 0.7777f;
					score += x_score * 4;
					break;
				}

				// сброс значений
				temp1 = 0;
				dx = 0;
				rotate = 0;
			}// конец если не пауза
		}// конец если не поражение
		
		// отображение очков
		text_score.setPosition(55, 392);
		text_score.setString("Score: " + std::to_string(score));
		text_score.setCharacterSize(24);

		// вывод на экран
		window.display();
	}// конец пока окно открыто
	return 0;
}