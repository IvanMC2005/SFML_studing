#include <iostream>
#include <fstream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <cmath>
#include <string>
#include <locale>
using namespace sf;	
using namespace std;
int main()
{
	setlocale(0, "rus");
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "New");

	Vector2u size = window.getSize();
	VertexArray graph(LineStrip, size.x + 1);
	int count_joints = 0;
	float  x0 = size.x / 2, x, y, x1, y1;

	float y0 = size.y / 2, i = 0;

	float scale = 50; // начальный масштаб графика
	float scale_speed = 0.2; // скорость масштабирования
	float e = 2.7182818;
	float move_speed = 0.3; // скорость перемещения графика
	
	int low_border; // нижняя граница диапазона
	char ch;
	cout << "Введите левую границу графика\n";
	cin >> low_border;
	cout << "\n";
	while (cin.fail())
	{
		cin.clear();
		while ((ch = getchar()) != '\n');
		cout << "Введите левую границу графика\n";
		cin >> low_border;
		cout << "\n";
	}
	int high_border; // верхняя граница диапазона
	string low_text = to_string(low_border);
	cout << "Введите правую границу графика\n";
	cin >> high_border;
	cout << "\n";
	while (cin.fail())
	{
		cin.clear();
		while ((ch = getchar()) != '\n');
		cout << "Введите правую границу графика\n";
		cin >> high_border;
		cout << "\n";
	}
	string high_text = to_string(high_border);
	int mass = max(abs(high_border), abs(low_border)) * 4, infinity_point = 1;
	// рисуем координатные оси
	RectangleShape OoX(Vector2f(12 * scale, 1));
	RectangleShape asimpt(Vector2f(1, 12 * scale));
	asimpt.setFillColor(Color::Black);
	OoX.setFillColor(Color::Black);
	OoX.setPosition(x0 - 6 * scale, y0);
	RectangleShape OoY(Vector2f(1, 12 * scale));
	OoY.setFillColor(Color::Black);
	OoY.setPosition(x0, y0 - 6 * scale);
	// рисуем стрелки, задающие направление осям
	RectangleShape arrows[4];
	for (int i = 0; i < 4; i++)
	{
		arrows[i].setSize(Vector2f(1, 20));
		arrows[i].setFillColor(Color::Black);
	}
	arrows[0].setRotation(30);
	arrows[1].setRotation(-30);
	arrows[2].setRotation(120);
	arrows[3].setRotation(60);
	CircleShape centre(3);
	centre.setFillColor(Color::Black);
	// задаём шрифт для текста и сам текст (подписи осей, едичный отрезок)
	Font font;
	font.loadFromFile("x64//Debug//aflfont.TTF");
	Text X("X", font, 20);
	Text Y("Y", font, 20);
	Text single("1", font, 20);
	Text zero("0", font, 20);
	Text right_degree(high_text, font, 20);
	Text left_degree(low_text, font, 20);
	right_degree.setFillColor(Color::Black);
	left_degree.setFillColor(Color::Black);
	zero.setFillColor(Color::Black);
	X.setFillColor(Color::Black);
	Y.setFillColor(Color::Black);
	single.setFillColor(Color::Black);
	// задаём деления на координатной плоскости
	RectangleShape* grades = new RectangleShape[mass + 1];
	for (int i = 0; i < mass; i++)
	{
		grades[i].setSize(Vector2f(1, 20));
		grades[i].setFillColor(Color::Black);
	}
	while (window.isOpen())
	{
		// задаём положение и размер всем объектам, зависящим от масштаба
		OoX.setSize(Vector2f((mass / 2 + 2) * scale, 1));
		OoY.setSize(Vector2f(1, (mass / 2 + 2) * scale));
		asimpt.setSize(Vector2f(3, (mass / 2 + 2) * scale));
		asimpt.setPosition(x0 + scale * infinity_point - 1, y0 - (mass / 4 + 1) * scale);
		OoX.setPosition(x0 - (mass / 4 + 1) * scale, y0);
		OoY.setPosition(x0, y0 - (mass / 4 + 1) * scale);
		arrows[0].setPosition(x0, y0 - (mass / 4 + 1) * scale);
		arrows[1].setPosition(x0, y0 - (mass / 4 + 1) * scale);
		arrows[2].setPosition(x0 + (mass / 4 + 1) * scale, y0);
		arrows[3].setPosition(x0 + (mass / 4 + 1)  * scale, y0);
		X.setPosition(x0 + (mass / 4 + 1) * scale + 2, y0 + 2);
		zero.setPosition(x0 + 3, y0 + 3);
		centre.setPosition(x0 - 1, y0 - 1);
		Y.setPosition(x0 - 30, y0 - (mass / 4 + 1) * scale);
		single.setPosition(x0 + scale + 2, y0 + 3);
		
		for (int i = 0; i < mass; i++)
		{
			if (i < mass / 2)
				if (i < mass / 4)
					grades[i].setPosition(Vector2f(x0 - (i % (mass / 4) + 1) * scale, y0 - 10));
				else
					grades[i].setPosition(Vector2f(x0 + (i % (mass / 4) + 1) * scale, y0 - 10));
			else
			{
				grades[i].setRotation(90);
				if (i < 3*mass/4)
					grades[i].setPosition(Vector2f(x0 + 10, y0 - (i % (mass / 4) + 1) * scale));
				else
					grades[i].setPosition(Vector2f(x0 + 10, y0 + (i % (mass / 4) + 1) * scale));
			}
		}
		left_degree.setPosition(x0 + low_border * scale, y0 + 3);
		right_degree.setPosition(x0 + high_border * scale, y0 + 3);
		graph.clear(); // очищаем очередь точек перед переотрисовкой графика
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Subtract))
			if (scale < 15)
			{
				scale = 15;
			}
			else
				scale -= scale_speed;
		if (Keyboard::isKeyPressed(Keyboard::Add))
			if (scale < 5000)
				scale += scale_speed;
		if (Keyboard::isKeyPressed(Keyboard::D))
			x0 -= move_speed;
		if (Keyboard::isKeyPressed(Keyboard::A))
			x0 += move_speed;
		if (Keyboard::isKeyPressed(Keyboard::S))
			y0 -= move_speed;
		if (Keyboard::isKeyPressed(Keyboard::W))
			y0 += move_speed;
		window.clear(Color::White);
		// задаём точки графика функции и помещаем в VertexArray
		for (i = low_border * scale; i < high_border * scale; i++)
		{
			x = i;
			y = (5 * x / scale) / pow((1 - x / scale), 2);
			x1 = x0 + x; /* вычисляем для конкретного x значение графика по оси Ox, 
													умножая x на масштаб, сдвигая относительно центра с помощью прибавления x0*/
			y1 = y0 - y * scale;/* вычисляем для конкретного x значение y, с учётом расчёта в декартовой системе координат(x растёт вправо,
								а y вверх) нормализуем расчёт значения по Oy, вычитая от центра полученное y, помноженное на масштаб */
			if (y1 > y0 - (mass / 4 + 1) * scale)
				graph.append(Vertex(Vector2f(x1, y1), Color::Green));
			else
				graph.append(Vertex(Vector2f(x1, y1), Color::White));

		}
		window.draw(single);
		window.draw(left_degree);
		window.draw(right_degree);
		window.draw(asimpt);
		window.draw(zero);
		window.draw(centre);
		window.draw(graph);
		window.draw(OoX);
		window.draw(OoY);
		window.draw(X);
		window.draw(Y);
		for (int i = 0; i < 4; i++)
			window.draw(arrows[i]);
		for (int i = 0; i < mass; i++)
			window.draw(grades[i]);
		window.display();
	}
	delete[] grades;
	return 0;
}