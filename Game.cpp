#include "Game.h"
#include "player.h"
Texture tEmpty, tShip, tHit, tMiss, tSink;
Texture blackBgr;

bool AIPlay(player &p, player &opponent, Sprite** tab_own, Sprite** tab_spr, ComputerAlgorithm &AI, RenderWindow& window, Text& com)
{
	ship *tmp;
	tmp = AI.ShootOpponent();
	window.clear(Color::Blue);
	DrawFields(p, tab_own, true, window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
	DrawFields(opponent, tab_spr, SHIP_VISIBILITY, window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, true);
	com.setString("Computer's turn");
	window.draw(com);
	window.display();
	while (tmp != nullptr)
	{
		if (tmp->RefShipLength() == 0)
		{


			if (p.CountShips() == 0)
			{
				return true;
			}
			
		}
		tmp = AI.ShootOpponent();
		window.clear(Color::Blue);
		DrawFields(p, tab_own, true, window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
		DrawFields(opponent, tab_spr, SHIP_VISIBILITY, window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, true);
		com.setString("Computer's turn");
		window.draw(com);
		window.display();
	}
	return false;
}






void ClearArea(ship& s, field** tab)
{

	int x, y,length;
	DIRECTION dir;
	x = s.getX();
	y = s.getY();
	length = s.getFirstLength();
	dir = (DIRECTION)s.tmpdir;

	switch (dir)
	{
	case UP:
		for (int i = 0; i < length; i++)
		{
			tab[y - i][x].refState() = SINK;
			if (x - 1 >= 0)
				tab[y - i][x - 1].refState() = MISS;
			if (x + 1 < 10)
				tab[y - i][x + 1].refState() = MISS;

		}
		if (y - length >= 0)
			tab[y - length][x] = MISS;
		if (y + 1 < 10)
			tab[y + 1][x] = MISS;
		break;
	case LEFT:
		for (int i = 0; i < length; i++)
		{
			tab[y][x-i].refState() = SINK;
			if (y - 1 >= 0)
				tab[y - 1][x - i].refState() = MISS;
			if (y + 1 < 10)
				tab[y + 1][x - i].refState() = MISS;

		}
		if (x - length >= 0)
			tab[y][x - length] = MISS;
		if (x + 1 < 10)
			tab[y][x + 1] = MISS;
		break;
	case DOWN:
		for (int i = 0; i < length; i++)
		{
			tab[y + i][x].refState() = SINK;
			if (x - 1 >= 0)
				tab[y + i][x - 1].refState() = MISS;
			if (x + 1 < 10)
				tab[y + i][x + 1].refState() = MISS;

		}
		if (y + length < 10)
			tab[y + length][x] = MISS;
		if (y - 1 >= 0)
			tab[y - 1][x] = MISS;
		break;
	case RIGHT:
		for (int i = 0; i < length; i++)
		{
			tab[y][x + i].refState() = SINK;
			if (y - 1 >= 0)
				tab[y - 1][x + i].refState() = MISS;
			if (y + 1 < 10)
				tab[y + 1][x + i].refState() = MISS;

		}
		if (x + length < 10)
			tab[y][x + length] = MISS;
		if (x - 1 >= 0)
			tab[y][x - 1] = MISS;
		break;
	case NONE:
		cout << "SOMETHING WENT WRONG";
		break;
	default:
		break;
	}
}



void LoadTexture(Texture& t, string filename)
{
	if (!t.loadFromFile(filename))
	{
		MessageBox(NULL, "Cannot load a texture!", "ERROR", NULL);
	}
}

void DrawFields(player& p, Sprite** tab, bool visibility, RenderWindow& window, int x, int y, double scale, bool canSelect, bool online, field** tab_enemy)
{
	Vector2f mouse(Mouse::getPosition(window));
	STATUS s;
	for (int i = 0; i < SIZE_OF_PG; i++)
		for (int j = 0; j < SIZE_OF_PG; j++)
		{
			if (!online)
				s = p.WhatState(j, i);
			else
				s = tab_enemy[i][j].refState();
			Sprite& tmp = tab[i][j];
			if (tmp.getGlobalBounds().contains(mouse) && canSelect && p.NotShot(j,i) && !online)
				tmp.setTexture(blackBgr);
			else if(online && tmp.getGlobalBounds().contains(mouse) && canSelect && s == EMPTY)
				tmp.setTexture(blackBgr);
			else
			{
				switch (s)
				{
				case EMPTY:
					tmp.setTexture(tEmpty);
					break;
				case SHIP:
					if (visibility)
						tmp.setTexture(tShip);
					else
						tmp.setTexture(tEmpty);
					break;
				case MISS:
					tmp.setTexture(tMiss);
					break;
				case HIT:
					tmp.setTexture(tHit);
					break;
				case SINK:
					tmp.setTexture(tSink);
					break;
				default:
					MessageBox(NULL, "Wrong field!", "Error", NULL);
					break;
				}
			}
			tmp.setScale(scale, scale);
			tmp.setPosition(x + scale*100*j, y + scale*100*i);
			window.draw(tmp);
		}
}

void ColorField(RenderWindow& window, Sprite** tab)
{
	Vector2f mouse(Mouse::getPosition(window));

	for (int i = 0; i < SIZE_OF_PG; i++)
		for (int j = 0; j < SIZE_OF_PG; j++)
			if (tab[i][j].getGlobalBounds().contains(mouse))
				tab[i][j].setColor(Color(0, 0, 0, 10));
}

Game::Game()
{
	state = END;

	if (!font.loadFromFile("font.ttf"))
	{
		MessageBox(NULL, "Cannot find font file!", "ERROR", NULL);
		return;
	}
	state = MENU;
}

bool Game::Shoot(player& p, RenderWindow& window, Sprite** tab, Event& event, Text& t)
{
	Vector2f mouse(Mouse::getPosition(window));
	ship* tmp;
	for (int i = 0; i < SIZE_OF_PG; i++)
		for (int j = 0; j < SIZE_OF_PG; j++)
			if (tab[i][j].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && p.NotShot(j, i))
			{
				tmp = p.Shoot(i, j);
				if (tmp == nullptr)
					return false;
				if (tmp->RefShipLength() == 0)
				{
					if (p.CountShips() == 0)
					{
						MessageBox(NULL, "Congratulations! You won!", "Fantastic", NULL);
						state = GAME_OVER;
						t.setString("You won!");
						return true;
					}
					t.setString("You've sunk enemy ship! Still your turn!");
				}
				else
					t.setString("You've hit enemy ship! Can you sank it?");
				return true;
			}

	return true;

}

void Game::menu()
{
	RenderWindow window(VideoMode(cfg::WINDOW_WIDTH, cfg::WINDOW_HEIGHT, 32), "BATTLESHIP", Style::None);
	Text title("Battleship", font, 100);
	title.setStyle(Text::Bold);
	title.setPosition(cfg::WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 30);

	const int nOptions = 4;
	Text options[nOptions];
	string str[] = { "Single Player","Hot Seat Multiplayer", "Multiplayer","Exit" };

	/*Music pirateMusic;
	pirateMusic.setLoop(true);
	if (!pirateMusic.openFromFile("pirate.wav"))
		MessageBox(NULL, "Audio File not found!", "Error", MB_OK);
	pirateMusic.play();*/
	for (int i = 0; i < nOptions; i++)
	{
		options[i].setFont(font);
		options[i].setCharacterSize(50);
		options[i].setString(str[i]);
		options[i].setPosition(cfg::WINDOW_WIDTH / 2 - options[i].getLocalBounds().width / 2, 200 + i * 100);
		options[i].setColor(Color::White);
	}
	Clock clock;
	while (state == MENU)
	{
		Event event;
		Vector2f mouse(Mouse::getPosition(window));
		Time elapsed = clock.restart();
		if (elapsed < milliseconds(1000 / cfg::fps))
			sleep(milliseconds(1000 / cfg::fps) - elapsed);

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				state = END;


			// MENU OPTIONS
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
				&& options[nOptions - 1].getGlobalBounds().contains(mouse))
				state = END;
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
				&& options[0].getGlobalBounds().contains(mouse))
			{

				state = GAME;

			}
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
				&& options[1].getGlobalBounds().contains(mouse))
				state = GAME_MULTI;
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
				&& options[2].getGlobalBounds().contains(mouse))
				state = GAME_ONLINE;
		}

		window.clear(Color::Blue);
		window.draw(title);

		for (int i = 0; i < nOptions; i++)
		{
			if (options[i].getGlobalBounds().contains(mouse))
				options[i].setColor(Color::Black);
			else
				options[i].setColor(Color::White);
			window.draw(options[i]);
		}
		window.display();
	}
	//pirateMusic.stop();
}

void Game::Play(player& p, player& opponent, string s)
{
	RenderWindow window(VideoMode(cfg::game_window_width, cfg::game_window_height, 32), "BATTLESHIP", Style::None);
	ComputerAlgorithm ai(&p);
	LoadTexture(tEmpty, "image_empty.png");
	LoadTexture(tShip, "image_ship.png");
	LoadTexture(tHit, "image_hit.png");
	LoadTexture(tMiss, "image_miss.png");
	LoadTexture(tSink, "image_sink.png");
	LoadTexture(blackBgr, "image_select.png");
	Text title_me("My ships",font, 50);
	Text title_opp("Enemy ships", font, 50);
	Text back_button("Back to menu", font, 30);
	Text communicate("Your turn", font, 50);
	title_me.setStyle(Text::Bold);
	title_opp.setStyle(Text::Bold);
	back_button.setStyle(Text::Bold);
	title_me.setPosition((cfg::first_pg_x + 500*cfg::scale) - title_me.getGlobalBounds().width / 2, cfg::first_pg_y + 1000*cfg::scale);
	title_opp.setPosition((cfg::second_pg_x + 500 * cfg::scale) - title_opp.getGlobalBounds().width / 2, cfg::second_pg_y + 1000 * cfg::scale);
	back_button.setPosition(cfg::game_window_width - cfg::scale * 100 - back_button.getGlobalBounds().width, cfg::scale * 50);
	communicate.setPosition(cfg::scale * 100, cfg::scale * 50);
	Sprite** tab_spr = new Sprite*[SIZE_OF_PG];
	Sprite** tab_own = new Sprite*[SIZE_OF_PG];
	for (int i = 0; i < SIZE_OF_PG; i++)
	{
		tab_spr[i] = new Sprite[SIZE_OF_PG];
		tab_own[i] = new Sprite[SIZE_OF_PG];
	}




	Clock clock;
	while (window.isOpen())
	{
		Time elapsed = clock.restart();
		if (elapsed < milliseconds(1000 / cfg::fps))
			sleep(milliseconds(1000 / cfg::fps) - elapsed);
		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		if (back_button.getGlobalBounds().contains(mouse))
			back_button.setColor(Color::Black);
		else
			back_button.setColor(Color::White);

		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && back_button.getGlobalBounds().contains(mouse))
			{
				if(state == GAME_OVER || MessageBox(NULL,"If you go back to menu, you'll lose all your game progress!\nDo you really want to go back to menu?","Confirm",MB_YESNO) == IDYES)
				{
					state = MENU;
					window.close();
				}
				continue;
			}
			if (event.type == Event::Closed)
			{
				state = MENU;
				window.close();
			}
			if (state == GAME && !Shoot(opponent, window, tab_spr, event, communicate))
			{
				MessageBox(NULL, "You've missed.\nComputer is playing now.", "Computer's turn", NULL);
				communicate.setString("Computer's turn");
				window.clear(Color::Blue);
				window.draw(back_button);
				window.draw(title_me);
				window.draw(title_opp);
				window.draw(communicate);
				DrawFields(p, tab_own, true, window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
				DrawFields(opponent, tab_spr, (state == GAME_OVER), window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, state != GAME_OVER);
				window.display();
				if (AIPlay(p, opponent, tab_own, tab_spr, ai, window, communicate))
				{
					MessageBox(NULL, "Sadly, computer won. Maybe you'll win next time", "You lose", NULL);
					state = GAME_OVER;
					communicate.setString("Computer won");
					
				}
				else
				communicate.setString("Your turn");
			}

			if (state == GAME_MULTI && !Shoot(opponent, window, tab_spr, event, communicate))
			{
				MessageBox(NULL, "You've missed.\nEnemy is playing now.", "Enemy's turn.", NULL);
				return;
			}
			
		}
		window.clear(Color::Blue);
		window.draw(back_button);
		window.draw(title_me);
		window.draw(title_opp);
		window.draw(communicate);

		DrawFields(p, tab_own, (state != GAME_MULTI || cfg::my_ship_visibility) , window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
		DrawFields(opponent, tab_spr,(state == GAME_OVER || SHIP_VISIBILITY), window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, state != GAME_OVER);
		window.display();
	}


	
}



// GAME CLASS' METHODS







void Game::hotSeat()
{
	player p1, p2;
	p1.SetShips();
	p2.SetShips();
	while(1)
	{
		MessageBox(NULL, "Player 1 is playing now.", "Player 1 turn", NULL);
		Play(p1, p2, "Player 1");
		if (state != GAME_MULTI)
			break;
		MessageBox(NULL, "Player 2 is playing now.", "Player 2 turn", NULL);
		Play(p2, p1, "Player 2");
		if (state != GAME_MULTI)
			break;
	}
}

void Game::multiPlayerMenu()
{
	int window_width = 800;
	int window_height = 600;
	int charsize = 50;
	double ip_x = 25;
	double ip_y = window_height/4 - charsize - ip_x/2;
			
	RenderWindow window(VideoMode(window_width, window_height, 32), "BATTLESHIP", Style::Default);
	
	IpAddress myIp;
	const int nOptions = 3;
	string labels[nOptions] = {"Start server", "Connect to server", "Back to menu" };
	Text options[nOptions];
	Text labelMyIp("My ip: ", font, charsize - 20);
	Text labelEnemyIp("",font,charsize);
	labelEnemyIp.setFont(font);
	Text textMyIp(IpAddress::getLocalAddress().toString(), font, charsize - 20);
	Text textEnemyIp("",font,charsize);
	Text backButton("Back", font, charsize);
	Text loadButton("Load ip", font, charsize - 20);
	Text saveButton("Save ip", font, charsize - 20);
	Text connectButton("Connect", font, charsize + 10);
	Text message("", font, charsize);

	message.setPosition(100, 500);
	labelMyIp.setPosition(ip_x, ip_y);
	labelEnemyIp.setPosition(ip_x, ip_y + charsize + 25);
	textMyIp.setPosition(ip_x + labelMyIp.getGlobalBounds().width + 25, ip_y);
	backButton.setPosition(window_width - backButton.getGlobalBounds().width - 20, window_height - 40 - backButton.getGlobalBounds().height);
	loadButton.setPosition(ip_x + 25, labelEnemyIp.getGlobalBounds().top + labelEnemyIp.getGlobalBounds().height + 75);
	saveButton.setPosition(loadButton.getGlobalBounds().left + loadButton.getGlobalBounds().width +  25, labelEnemyIp.getGlobalBounds().top + labelEnemyIp.getGlobalBounds().height + 75);
	connectButton.setPosition(window_width / 2 - connectButton.getGlobalBounds().width / 2, labelEnemyIp.getGlobalBounds().top + labelEnemyIp.getGlobalBounds().height + 150);
	bool isServer;
	bool selected = false;
	Connector connector(isServer, labelEnemyIp.getString(), message);
	Thread connecting(&Connector::Connect, &connector);

	for (int i = 0; i < nOptions; i++)
	{
		options[i].setFont(font);
		options[i].setColor(Color::White);
		options[i].setString(labels[i]);
		options[i].setCharacterSize(60);
		options[i].setPosition(window_width / 2 - options[i].getGlobalBounds().width / 2, window_height / 3 + i*80);
	}
	Clock clock;
	while (window.isOpen())
	{

		Time elapsed = clock.restart();
		if (elapsed < milliseconds(1000 / cfg::fps))
			sleep(milliseconds(1000 / cfg::fps) - elapsed);

		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		if (!selected)
		{
			for (int i = 0; i < nOptions; i++)
			{
				if (options[i].getGlobalBounds().contains(mouse))
					options[i].setColor(Color::Black);
				else
					options[i].setColor(Color::White);
			}
		}
		else
		{
			if (backButton.getGlobalBounds().contains(mouse))
				backButton.setColor(Color::Black);
			else
				backButton.setColor(Color::White);

			if (connectButton.getGlobalBounds().contains(mouse) && !connector.connecting)
				connectButton.setColor(Color::Black);
			else
				connectButton.setColor(Color::White);
			if (loadButton.getGlobalBounds().contains(mouse))
				loadButton.setColor(Color::Black);
			else
				loadButton.setColor(Color::White);
			if (saveButton.getGlobalBounds().contains(mouse))
				saveButton.setColor(Color::Black);
			else
				saveButton.setColor(Color::White);
		}
		while (window.pollEvent(event))
		{
			if (selected)
			{
				GetIp(event, textEnemyIp);
				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && saveButton.getGlobalBounds().contains(mouse))
				{
					if (!SaveIp(textEnemyIp))
						MessageBox(NULL, "Can't save ip.", "ERROR", MB_ICONWARNING + MB_OK);
				}

				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && loadButton.getGlobalBounds().contains(mouse))
				{
					if (!LoadIpFromFile(textEnemyIp))
						MessageBox(NULL, "Can't load ip. Maybe file doesn't exist...", "ERROR", MB_ICONWARNING + MB_OK);
				}
	


				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && backButton.getGlobalBounds().contains(mouse))
				{
					selected = false;
					connecting.terminate();
					message.setString("");
				}
				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && connectButton.getGlobalBounds().contains(mouse) && !connector.connecting)
				{
					connector.isServer = isServer;
					connector.str_ip = textEnemyIp.getString();
					if (isServer)
						message.setString("Waiting for client");
					else
					{
						message.setString("Connecting with server");
					}
					connector.text = message;
					//connector.Connect();
					connecting.launch();
					//connecting.terminate();
				}
				if (connector.isConnected)
				{
					window.close();
					multiPlayer(connector);
				}
			}
			else
			{
				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && options[nOptions - 1].getGlobalBounds().contains(mouse))
				{
					state = MENU;
					window.close();
					return;
				}
				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && options[0].getGlobalBounds().contains(mouse))
				{
					selected = true;
					isServer = true;
					labelEnemyIp.setString("Type client's ip: ");
					textEnemyIp.setPosition(ip_x + labelEnemyIp.getGlobalBounds().width + 25, ip_y + charsize + 25);
				}
				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && options[1].getGlobalBounds().contains(mouse))
				{
					selected = true;
					isServer = false;
					labelEnemyIp.setString("Type server's ip: ");
					textEnemyIp.setPosition(ip_x + labelEnemyIp.getGlobalBounds().width + 25, ip_y + charsize + 25);
				}
			}
		}
		window.clear(Color::Blue);
		if (selected)
		{
			window.draw(textEnemyIp);
			window.draw(textMyIp);
			window.draw(labelEnemyIp);
			window.draw(labelMyIp);
			window.draw(saveButton);
			window.draw(loadButton);
			window.draw(backButton);
			window.draw(connectButton);
			window.draw(message);
		}
		else
			for (int i = 0; i < nOptions; i++)
				window.draw(options[i]);
		window.display();
	}
}


void GetIp(Event& event, Text& text)
{
	string str = text.getString();
	if (event.type == Event::TextEntered)
	{
		if (event.text.unicode == 8)
		{
			if (str.size() > 0)
				str.erase(str.size() - 1, 1);
		}
		else if (event.text.unicode < 128 && event.text.unicode != 13 && str.size() < 20)
			str += event.text.unicode;

		text.setString(str);
	}
}

bool LoadIpFromFile(Text& text)
{
	ifstream in("ip.txt");
	string s;
	if (!(in >> s))
	{
		return false;
		in.close();
	}
	text.setString(s);
	in.close();
	return true;


}

bool SaveIp(Text& text)
{
	ofstream out("ip.txt");
	string s = text.getString();
	if (!(out << s))
	{
		return false;
		out.close();
	}
	out.close();
	return true;
}

bool receiveWithTimeout(UdpSocket& s, Packet& p, Time& t, unsigned short port, IpAddress ip)
{
	SocketSelector SS;
	SS.add(s);
	if (SS.wait(t))
	{
		if (SS.isReady(s))
		{
			return s.receive(p, ip, port) == Socket::Done;
		}
		return false;
	}
	else
		return false;

}

void Connector::Connect()
{
	connecting = true;
	//UdpSocket socket;
	//socket.setBlocking(false);
	//SocketSelector selector;
	//selector.add(socket);
	Time time = sf::seconds(5);
	Time sleeptime = sf::seconds(2);
	Clock clock;
	IpAddress ip(str_ip);
	string str_c = "Connected with server!";
	string str_s = "Connected with client!";
	unsigned short port_s = 50001;
	unsigned short port_c = 50002;
	//ip = str_ip;
	if (isServer)
		socket.bind(port_s);
	else
		socket.bind(port_c);

	if (isServer)
	{
		string tmp;
		Packet packet;
		if (!receiveWithTimeout(socket, packet, seconds(10), port_s, ip))
		{
			text.setString("Can't connect");
		}
		else
		{
			packet >> tmp;
			packet.clear();
			text.setString(tmp);
			tmp = str_c;
			packet << tmp;
			socket.send(packet, ip, port_c);
			sleep(sleeptime);
			isConnected = text.getString() == str_s;
		}

	}
	else
	{
		string tmp;
		Packet packet;

		tmp = str_s;
		packet << tmp;
		socket.send(packet, ip, port_s);
		packet.clear();
		if (!receiveWithTimeout(socket, packet, seconds(10), port_c, ip))
		{
			text.setString("Can't connect");
		}
		else
		{
			packet >> tmp;
			text.setString(tmp);
			sleep(sleeptime);
			isConnected = tmp == str_c;
		}
	}

	connecting = false;
}
void Connector::SendData()
{
	unsigned short port;
	if (isServer)
		port = 50002;
	else
		port = 50001;
	Packet packet;
	if (myTurn)
		packet << myTurn << x << y;
	else
		packet << myTurn << shipHit << shipSunk << won << enemy_ship;
	socket.send(packet, (IpAddress)str_ip, port);
}

void Connector::ReceiveData()
{
	unsigned short port;
	bool tmp;
	if (isServer)
		port = 50001;
	else
		port = 50002;
	Packet packet;
	socket.receive(packet, (IpAddress)str_ip, port);
	if (myTurn)
		packet >> tmp >> shipHit >> shipSunk >> won >> enemy_ship;
	else
	{
		packet >> tmp >> x >> y;
	}

	dataReceived = true;

}


void handleReceivedData(Connector& connector, field** tab_enemy, player& p, bool& turnFlag)
{
	connector.dataReceived = false;
	connector.receiving = false;
	if (connector.myTurn) // handle info about shot
	{
		if (connector.shipHit)
		{
			tab_enemy[connector.y][connector.x] = HIT;
			turnFlag = true;
		}
		else
		{
			tab_enemy[connector.y][connector.x] = MISS;
			turnFlag = false;
			return;
		}

	}
	else // check my ships
	{
		ship* tmp = p.Shoot(connector.y, connector.x);
		connector.shipHit = tmp != nullptr;
		if (connector.shipHit)
		{
			connector.shipSunk = tmp->RefShipLength() == 0;
			connector.won = p.CountShips() == 0;
		}
		else
		{
			connector.shipSunk = false;
		}
		if (connector.shipSunk)
		{
			connector.enemy_ship = *tmp;
		}
		turnFlag = !connector.shipHit;
	}
}
void Game::multiPlayer(Connector& connector)
{
	bool turnFlag;
	player p;
	p.SetShips();
	field** tab_enemy = new field*[SIZE_OF_PG];
	for (int i = 0; i < SIZE_OF_PG; i++)
		tab_enemy[i] = new field[SIZE_OF_PG];
	connector.dataReceived = false;
	connector.receiving = false;
	connector.myTurn = connector.isServer;
	Thread T_receiving(&Connector::ReceiveData, &connector);
	Thread T_sending(&Connector::SendData, &connector);

	RenderWindow window(VideoMode(cfg::game_window_width, cfg::game_window_height,32),"Battleship", Style::Default);
	LoadTexture(tEmpty, "image_empty.png");
	LoadTexture(tShip, "image_ship.png");
	LoadTexture(tHit, "image_hit.png");
	LoadTexture(tMiss, "image_miss.png");
	LoadTexture(tSink, "image_sink.png");
	LoadTexture(blackBgr, "image_select.png");
	Text title_me("My ships", font, 50);
	Text title_opp("Enemy ships", font, 50);
	Text back_button("Back to menu", font, 30);
	Text communicate("", font, 50);
	title_me.setStyle(Text::Bold);
	title_opp.setStyle(Text::Bold);
	back_button.setStyle(Text::Bold);
	title_me.setPosition((cfg::first_pg_x + 500 * cfg::scale) - title_me.getGlobalBounds().width / 2, cfg::first_pg_y + 1000 * cfg::scale);
	title_opp.setPosition((cfg::second_pg_x + 500 * cfg::scale) - title_opp.getGlobalBounds().width / 2, cfg::second_pg_y + 1000 * cfg::scale);
	back_button.setPosition(cfg::game_window_width - cfg::scale * 100 - back_button.getGlobalBounds().width, cfg::scale * 50);
	communicate.setPosition(cfg::scale * 100, cfg::scale * 50);
	Sprite** tab_spr = new Sprite*[SIZE_OF_PG];
	Sprite** tab_own = new Sprite*[SIZE_OF_PG];
	for (int i = 0; i < SIZE_OF_PG; i++)
	{
		tab_spr[i] = new Sprite[SIZE_OF_PG];
		tab_own[i] = new Sprite[SIZE_OF_PG];
	}
	Clock clock;
	while (window.isOpen())
	{
		Time elapsed = clock.restart();
		if (elapsed < milliseconds(1000 / cfg::fps))
			sleep(milliseconds(1000 / cfg::fps) - elapsed);
		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		if (back_button.getGlobalBounds().contains(mouse))
			back_button.setColor(Color::Black);
		else
			back_button.setColor(Color::White);

		// RECEIVING
		if (!connector.receiving)
		{
			connector.receiving = true;
			T_receiving.launch();

		}
		else if (connector.dataReceived)
		{
			turnFlag = connector.myTurn;
			handleReceivedData(connector, tab_enemy,p,turnFlag);
			if (!connector.myTurn)
				T_sending.launch();
			else if (connector.shipSunk)
			{
				ClearArea(connector.enemy_ship, tab_enemy);
				connector.shipSunk;
			}
			connector.myTurn = turnFlag;


		}


		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && back_button.getGlobalBounds().contains(mouse))
			{
				
				if (state == GAME_OVER || MessageBox(NULL, "If you go back to menu, you'll lose all your game progress!\nDo you really want to go back to menu?", "Confirm", MB_YESNO) == IDYES)
				{
					state = MENU;
					window.close();
					T_receiving.terminate();
					T_sending.terminate();
					break;

				}
				continue;
			}

			// SENDING
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && connector.myTurn && state == GAME_ONLINE)
			{
				for (int i = 0; i < SIZE_OF_PG; i++)
					for (int j = 0; j < SIZE_OF_PG; j++)
						if (tab_spr[i][j].getGlobalBounds().contains(mouse) && tab_enemy[i][j].refState() == EMPTY)
						{
							connector.x = j;
							connector.y = i;
							T_sending.launch();
						}
			}
			if (event.type == Event::Closed)
			{
				state = MENU;
				window.close();
				break;
			}

		}
		// Checking game conditions
		if (state == GAME_ONLINE)
		{
			if (connector.myTurn)
				communicate.setString("Your turn");
			else
				communicate.setString("Enemy's turn");
		}

		if (connector.won && state != GAME_OVER && state != MENU)
		{
			if (connector.myTurn)
				communicate.setString("Congratulations! You won!");
			else
				communicate.setString("Sadly, enemy won");
			state = GAME_OVER;
			//T_receiving.terminate();
			//T_sending.terminate();
		}

		window.clear(Color::Blue);
		window.draw(back_button);
		window.draw(title_me);
		window.draw(title_opp);
		window.draw(communicate);
		DrawFields(p, tab_own, true, window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
		DrawFields(p, tab_spr, SHIP_VISIBILITY, window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, connector.myTurn && state != GAME_OVER, true,tab_enemy);
		window.display();
	}



}