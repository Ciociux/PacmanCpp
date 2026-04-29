#include <SFML/Graphics.hpp>
#include <vector>

#define SP 1

void drawPacman(int dir, float x, float y, int& ciclo);
void pc0(int dir, float x, float y);
void pc1(int dir, float x, float y);
void pc2(int dir, float x, float y);
void pc3(int dir, float x, float y);
void map();
void spawnFrutti();
char fruit();
void input(int& dirT, bool& run);
void direction(int& dir, int dirT, float& x, float& y, float raggio);
int whatIs(float x, float y);
bool centratoX(float x);
bool centratoY(float x);
void punteggio(float x, float y);
void muro(int riga, int col);
void pallino(int riga, int col);
void pera(int riga, int col);
void mela(int riga, int col);
void arancia(int riga, int col);
void schermataIniziale();
void schermataVittoria();
void pausa();
bool win();

//Global
sf::RenderWindow finestra(
    sf::VideoMode({ 760, 600 }), //grandezza finestra
    "Pac-Man" //titolo finestra
);

// texture
sf::Texture texPacman0, texPacman1, texPacman2, texPacman3, texMela, texPera, texArancia;
sf::Font arial("arial.ttf");
sf::Text text(arial);

int punti = 0;

//Campo
float dimCel = 40;
const int maxX = 760;
const int maxY = 600;
const int RIGHE = 15;
const int COLONNE = 19;

char labirinto[RIGHE][COLONNE] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,2,1,2,1,2,1,2,1,1,1,2,1,2,1},
    {1,2,2,2,2,2,1,2,2,2,1,2,2,2,2,2,1,2,1},
    {1,1,1,1,1,2,1,1,1,0,1,1,1,2,1,1,1,2,1},
    {0,0,0,0,1,2,1,0,0,0,0,0,1,2,1,0,0,0,0},
    {1,1,1,1,1,2,1,0,0,0,0,0,1,2,1,1,1,1,1},
    {1,2,2,2,2,2,2,0,0,0,0,0,2,2,2,2,2,2,1},
    {1,1,1,1,1,2,1,0,0,0,0,0,1,2,1,1,1,1,1},
    {0,0,0,0,1,2,1,0,0,0,0,0,1,2,1,0,0,0,0},
    {1,1,1,1,1,2,1,1,1,0,1,1,1,2,1,1,1,2,1},
    {1,2,2,2,2,2,1,2,2,2,1,2,2,2,2,2,1,2,1},
    {1,2,1,1,1,2,1,2,1,2,1,2,1,1,1,2,1,2,1},
    {1,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main() {
    srand(time(NULL));

    //texture
    texPacman0.loadFromFile("texture/pacman0.png");
    texPacman1.loadFromFile("texture/pacman1.png");
    texPacman2.loadFromFile("texture/pacman2.png");
    texPacman3.loadFromFile("texture/pacman3.png");
    texMela.loadFromFile("texture/mela.png");
    texPera.loadFromFile("texture/pera.png");
    texArancia.loadFromFile("texture/arancia.png");

    float x = 380;
    float y = 300;

    int dir = 0;
    int dirT = 0;

    int ciclo = 0;

    sf::Clock clock;
    float timestep = 0.5f / 60.f;

    bool run = false;
    bool start = true;
    bool vinto = false;

    while (finestra.isOpen())
    {
        float tTrascorso = clock.getElapsedTime().asSeconds();

        if (tTrascorso < timestep) 
            continue; 

        clock.restart();

        input(dirT, run);

        finestra.clear(sf::Color::Blue);

        if (run) {
            if (!vinto) {
                map();
                spawnFrutti();
                direction(dir, dirT, x, y, 15);
                punteggio(x, y);
                drawPacman(dir, x, y, ciclo);

                if (win()) vinto = true;
            }
            else {
                schermataVittoria(); // SOLO DISEGNO
            }
        }
        else {
            if (start) schermataIniziale();
            else pausa();
        }

        finestra.display(); // mostra la finestra
    }

    return 0;
}

void schermataIniziale() {

    // Pannello centrale nero
    sf::RectangleShape panel;
    panel.setSize({ 480.f, 280.f });
    panel.setFillColor(sf::Color(30, 30, 30, 250));
    panel.setOrigin({ 240.f, 140.f });
    panel.setPosition({ 380.f, 300.f });
    finestra.draw(panel);

    // Titolo PAC-MAN
    sf::Text titolo(arial);
    titolo.setString("PAC-MAN");
    titolo.setCharacterSize(56);
    titolo.setFillColor(sf::Color::Yellow);
    titolo.setStyle(sf::Text::Bold);
    sf::FloatRect r = titolo.getLocalBounds();
    titolo.setOrigin({
        r.position.x + r.size.x / 2.f,
        r.position.y + r.size.y / 2.f
        });
    titolo.setPosition({ 380.f, 245.f });
    finestra.draw(titolo);

    // Linea decorativa
    sf::RectangleShape line;
    line.setSize({ 300.f, 3.f });
    line.setFillColor(sf::Color::Yellow);
    line.setOrigin({ 150.f, 1.5f });
    line.setPosition({ 380.f, 290.f });
    finestra.draw(line);

    // Istruzioni stile arcade
    sf::Text info(arial);
    info.setString("premi SPAZIO per iniziare");
    info.setCharacterSize(22);
    info.setFillColor(sf::Color::White);
    r = info.getLocalBounds();
    info.setOrigin({
        r.position.x + r.size.x / 2.f,
        r.position.y + r.size.y / 2.f
        });
    info.setPosition({ 380.f, 330.f });
    finestra.draw(info);
}

void pausa() {
    sf::RectangleShape panel({ 420.f, 220.f });
    panel.setFillColor(sf::Color(30, 30, 30, 250));
    panel.setOrigin({ 210.f, 110.f });
    panel.setPosition({ 380.f, 300.f });
    finestra.draw(panel);

    sf::Text pausa(arial);
    pausa.setString("PAUSA");
    pausa.setCharacterSize(50);
    pausa.setFillColor(sf::Color::Yellow);
    pausa.setStyle(sf::Text::Bold);
    sf::FloatRect r = pausa.getLocalBounds();
    pausa.setOrigin({
        r.position.x + r.size.x / 2.f,
        r.position.y + r.size.y / 2.f
        });
    pausa.setPosition({ 380.f, 300.f });
    finestra.draw(pausa);

    sf::Text info(arial);
    info.setString("Premi ESC per riprendere");
    info.setCharacterSize(20);
    info.setFillColor(sf::Color::White);
    r = info.getLocalBounds();
    info.setOrigin({
        r.position.x + r.size.x / 2.f,
        r.position.y + r.size.y / 2.f
        });
    info.setPosition({ 380.f, 350.f });
    finestra.draw(info);
}

void map() { //Genera la mappa
    for (int riga = 0; riga < RIGHE; riga++)
    {
        for (int col = 0; col < COLONNE; col++)
        {
            switch (labirinto[riga][col]) {
            case 1: { //Muro
                muro(riga, col);
                break;
            }
            case 2: { // cibo
                pallino(riga, col);
                break;
            }
            case 'a': { // Mela
                mela(riga, col);
                break;
            }
            case 'b': { // Pera
                pera(riga, col);
                break;
            }
            case 'c': { // Arancia aliena
                arancia(riga, col);
                break;
            }
            }
        }
    }
}

void drawPacman(int dir, float x, float y, int& ciclo)
{
    int slowdown = 10;

    int frame = (ciclo / slowdown) % 4; // 4 frame

    if (frame == 0) pc0(dir, x, y);
    else if (frame == 1) pc1(dir, x, y);
    else if (frame == 2) pc2(dir, x, y);
    else pc3(dir, x, y);

    ciclo++;
    if (ciclo >= slowdown * 4) ciclo = 0;
}

void pc0(int dir, float x, float y) {
    sf::Sprite pc(texPacman0);

    float targetHeight = 30.f;
    float currentHeight = pc.getLocalBounds().size.y;
    float scale = targetHeight / currentHeight;
    pc.setScale({ scale, scale });

    pc.setOrigin({ pc.getLocalBounds().size.x / 2.f, pc.getLocalBounds().size.y / 2.f });

    pc.setPosition({ x, y });

    sf::Angle rotation = sf::degrees(0);

    switch (dir)
    {
    case 1: rotation = sf::degrees(90); break;   // su
    case 2: rotation = sf::degrees(270); break;   // giu
    case 3: rotation = sf::degrees(180); break;   // destra
    case 4: rotation = sf::degrees(0); break;   // sinistra
    }

    pc.setRotation(rotation);

    finestra.draw(pc);
}

void pc1(int dir, float x, float y) {
    sf::Sprite pc(texPacman1);

    float targetHeight = 30.f;
    float currentHeight = pc.getLocalBounds().size.y;
    float scale = targetHeight / currentHeight;
    pc.setScale({ scale, scale });

    pc.setOrigin({ pc.getLocalBounds().size.x / 2.f, pc.getLocalBounds().size.y / 2.f });

    pc.setPosition({ x, y });

    sf::Angle rotation = sf::degrees(0);

    switch (dir)
    {
    case 1: rotation = sf::degrees(90); break;   // su
    case 2: rotation = sf::degrees(270); break;   // giu
    case 3: rotation = sf::degrees(180); break;   // destra
    case 4: rotation = sf::degrees(0); break;   // sinistra
    }

    pc.setRotation(rotation);

    finestra.draw(pc);
}

void pc2(int dir, float x, float y) {
    sf::Sprite pc(texPacman2);

    float targetHeight = 30.f;
    float currentHeight = pc.getLocalBounds().size.y;
    float scale = targetHeight / currentHeight;
    pc.setScale({ scale, scale });

    pc.setOrigin({ pc.getLocalBounds().size.x / 2.f, pc.getLocalBounds().size.y / 2.f });

    pc.setPosition({ x, y });

    sf::Angle rotation = sf::degrees(0);

    switch (dir)
    {
    case 1: rotation = sf::degrees(90); break;   // su
    case 2: rotation = sf::degrees(270); break;   // giu
    case 3: rotation = sf::degrees(180); break;   // destra
    case 4: rotation = sf::degrees(0); break;   // sinistra
    }

    pc.setRotation(rotation);

    finestra.draw(pc);
}

void pc3(int dir, float x, float y) {
    sf::Sprite pc(texPacman3);

    float targetHeight = 30.f;
    float currentHeight = pc.getLocalBounds().size.y;
    float scale = targetHeight / currentHeight;
    pc.setScale({ scale, scale });

    pc.setOrigin({ pc.getLocalBounds().size.x / 2.f, pc.getLocalBounds().size.y / 2.f });

    pc.setPosition({ x, y });

    sf::Angle rotation = sf::degrees(0);

    switch (dir)
    {
    case 1: rotation = sf::degrees(90); break;   // su
    case 2: rotation = sf::degrees(270); break;   // giu
    case 3: rotation = sf::degrees(180); break;   // destra
    case 4: rotation = sf::degrees(0); break;   // sinistra
    }

    pc.setRotation(rotation);

    finestra.draw(pc);
}

void spawnFrutti() {
    for (int r = 0; r < RIGHE; r++) {
        for (int c = 0; c < COLONNE; c++) {
            if (labirinto[r][c] == 2) {
                char f = fruit();
                if (f != 0) labirinto[r][c] = f;
            }
        }
    }
}

char fruit() { //spawna i frutti
    char ans = 0;
    int a;

    a = rand() % 5000000;

    if (a == 1) ans = 'a';
    else if (a == 2) ans = 'b';
    else if (a == 3) ans = 'c';

    return ans;
}

void muro(int riga, int col) {
    sf::RectangleShape muro({(float)dimCel, (float)dimCel});
    muro.setFillColor(sf::Color::Black);
    muro.setPosition(sf::Vector2f(col * dimCel, riga * dimCel));
    finestra.draw(muro);
}

void pallino(int riga, int col) {
    sf::CircleShape pallino(dimCel / 6.0);
    pallino.setFillColor(sf::Color::White);
    pallino.setPosition(sf::Vector2f(col * dimCel + dimCel / 3, riga * dimCel + dimCel / 3));
    finestra.draw(pallino);
}

void pera(int riga, int col) {
    sf::Sprite pera(texPera);
    float targetHeight = 35.f;
    float currentHeight = pera.getLocalBounds().size.y;
    float scale = targetHeight / currentHeight;
    pera.setScale({ scale, scale });
    pera.setPosition(sf::Vector2f(col * dimCel + (dimCel - targetHeight) / 2.f, riga * dimCel + (dimCel - targetHeight) / 2.f));
    finestra.draw(pera);
}

void mela(int riga, int col) {
    sf::Sprite mela(texMela);
    float targetHeight = 35.f;
    float currentHeight = mela.getLocalBounds().size.y;
    float scale = targetHeight / currentHeight;
    mela.setScale({ scale, scale });
    mela.setPosition(sf::Vector2f(col * dimCel + (dimCel - targetHeight) / 2.f, riga * dimCel + (dimCel - targetHeight) / 2.f));
    finestra.draw(mela);
}

void arancia(int riga, int col) { //arancia aliena
    sf::Sprite arancia(texArancia);
    float targetHeight = 35.f;
    float currentHeight = arancia.getLocalBounds().size.y;
    float scale = targetHeight / currentHeight;
    arancia.setScale({ scale, scale });
    arancia.setPosition(sf::Vector2f(col * dimCel + (dimCel - targetHeight) / 2.f, riga * dimCel + (dimCel - targetHeight) / 2.f));
    finestra.draw(arancia);
}

void input(int& dirT, bool& run) {
    while (auto event = finestra.pollEvent()) {

        if (event->is<sf::Event::Closed>())
            finestra.close();

        if (event->is<sf::Event::KeyPressed>()) {

            auto key = event->getIf<sf::Event::KeyPressed>()->code;

            if (key == sf::Keyboard::Key::Up) dirT = 1;
            if (key == sf::Keyboard::Key::Down) dirT = 2;
            if (key == sf::Keyboard::Key::Left) dirT = 4;
            if (key == sf::Keyboard::Key::Right) dirT = 3;

            if (key == sf::Keyboard::Key::Escape || key == sf::Keyboard::Key::Space) {
                run = !run;
            }
        }
    }
}

void direction(int& dir, int dirT, float& x, float& y, float raggio) { //Muove la palla

    // Controllo se posso cambiare direzione solo se centrato sull'altro asse
    if (dirT == 1 && centratoX(x) && whatIs(x, y - dimCel / 2 - SP) != 1) dir = 1; // su
    if (dirT == 2 && centratoX(x) && whatIs(x, y + dimCel / 2 + SP) != 1) dir = 2; // gi 
    if (dirT == 4 && centratoY(y) && whatIs(x - dimCel / 2 - SP, y) != 1) dir = 4; // sinistra
    if (dirT == 3 && centratoY(y) && whatIs(x + dimCel / 2 + SP, y) != 1) dir = 3; // destra

    // Accentraento
    if ((dir == 1 || dir == 2) && !centratoX(x))
        x = round((x - dimCel / 2) / dimCel) * dimCel + dimCel / 2;
    if ((dir == 3 || dir == 4) && !centratoY(y))
        y = round((y - dimCel / 2) / dimCel) * dimCel + dimCel / 2;

    // Movimento
    if (dir == 1 && whatIs(x, y - dimCel / 2 - SP) != 1) y -= SP; // su
    if (dir == 2 && whatIs(x, y + dimCel / 2 + SP - 1) != 1) y += SP; // gi 
    if (dir == 4 && whatIs(x - dimCel / 2 - SP, y) != 1) x -= SP; // sinistra
    if (dir == 3 && whatIs(x + dimCel / 2 + SP - 1, y) != 1) x += SP; // destra

    // Teletrasporto
    if (x - raggio < 0) x = maxX - raggio;
    else if (x + raggio > maxX) x = raggio;

    if (y - raggio < 0) y = maxY - raggio;
    else if (y + raggio > maxY) y = raggio;
}

int whatIs(float x, float y) {
    int col = (int)(x / dimCel);
    int riga = (int)(y / dimCel);
    int isIt = labirinto[riga][col];

    return isIt;
}

void punteggio(float x, float y) {
    int col = (int)(x / dimCel);
    int riga = (int)(y / dimCel);
    int isIt = labirinto[riga][col];

    if (isIt == 2) {
        labirinto[riga][col] = 0;
        punti++;
    }
    else if (isIt == 'a' || isIt == 'b' || isIt == 'c') {
        labirinto[riga][col] = 0;
        punti += 10;
    }
}

bool centratoX(float x) {
    bool centrato = false;
    if (((int)(x - dimCel / 2) % (int)dimCel) == 0) centrato = true;
    return centrato;
}

bool centratoY(float y) {
    bool centrato = false;
    if (((int)(y - dimCel / 2) % (int)dimCel) == 0) centrato = true;
    return centrato;
}

void schermataVittoria() {
        // Pannello centrale
        sf::RectangleShape panel;
        panel.setSize({ 520.f, 320.f });
        panel.setFillColor(sf::Color(20, 20, 20));
        panel.setOrigin({ 260.f, 160.f });
        panel.setPosition({ 380.f, 300.f });
        finestra.draw(panel);

        // ---------- PAC-MAN DECORATIVO ----------
        int dirPacman = 3; // destra
        int cicloFake = 0;
        drawPacman(dirPacman, 380.f, 220.f, cicloFake);

        // ---------- TITOLO ----------
        sf::Text titolo(arial);
        titolo.setString("VITTORIA!");
        titolo.setCharacterSize(46);
        titolo.setFillColor(sf::Color::Yellow);
        titolo.setStyle(sf::Text::Bold);
        sf::FloatRect r = titolo.getLocalBounds();
        titolo.setOrigin({
            r.position.x + r.size.x / 2.f,
            r.position.y + r.size.y / 2.f
            });
        titolo.setPosition({ 380.f, 265.f });
        finestra.draw(titolo);

        // Linea decorativa
        sf::RectangleShape line;
        line.setSize({ 300.f, 3.f });
        line.setFillColor(sf::Color::Yellow);
        line.setOrigin({ 150.f, 1.5f });
        line.setPosition({ 380.f, 290.f });
        finestra.draw(line);

        // ---------- PUNTEGGIO ----------
        sf::Text score(arial);
        score.setString("PUNTEGGIO: " + std::to_string(punti));
        score.setCharacterSize(24);
        score.setFillColor(sf::Color::White);
        score.setStyle(sf::Text::Bold);
        r = score.getLocalBounds();
        score.setOrigin({
            r.position.x + r.size.x / 2.f,
            r.position.y + r.size.y / 2.f
            });
        score.setPosition({ 380.f, 325.f });
        finestra.draw(score);

        // ---------- TESTO ----------
        sf::Text info(arial);
        info.setString("HAI MANGIATO TUTTO");
        info.setCharacterSize(18);
        info.setFillColor(sf::Color(200, 200, 200));
        r = info.getLocalBounds();
        info.setOrigin({
            r.position.x + r.size.x / 2.f,
            r.position.y + r.size.y / 2.f
            });
        info.setPosition({ 380.f, 355.f });
        finestra.draw(info);

        // ---------- FRUTTI DECORATIVI ----------
        mela(7, 4);
        pera(7, 5);
        arancia(7, 13);
        mela(7, 14);

        // ---------- SOTTOTESTO ----------
        sf::Text sub(arial);
        sub.setString("PREMI SPAZIO PER GIOCARE ANCORA");
        sub.setCharacterSize(18);
        sub.setFillColor(sf::Color(180, 180, 180));

        r = sub.getLocalBounds();
        sub.setOrigin({
            r.position.x + r.size.x / 2.f,
            r.position.y + r.size.y / 2.f
            });

        sub.setPosition({ 380.f, 395.f });
        finestra.draw(sub);
}

bool win() {
    for (int r = 0; r < RIGHE; r++) {
        for (int c = 0; c < COLONNE; c++) {
            if (labirinto[r][c] == 2 ||
                labirinto[r][c] == 'a' ||
                labirinto[r][c] == 'b' ||
                labirinto[r][c] == 'c') {
                return false; 
            }
        }
    }
    return true;
}


