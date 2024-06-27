#include <ctime>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
int vel = 4;
bool** campodj;
class Player{
    public:
    int x, y, dir;
    Color color;
    Player(Color c, int maxX, int maxY) {
        x = rand() % maxX;
        y = rand() % maxY;
        color = c;
        dir = rand() % 4;
    }
    void pos(int maxX, int maxY) {
        if (dir == 0) y += 1;
        if (dir == 1) x -= 1;
        if (dir == 2) x += 1;
        if (dir == 3) y -= 1;

        if (x >= maxX) x = 0; if (x < 0) x = maxX - 1;
        if (y >= maxY) y = 0; if (y < 0) y = maxY - 1;
    }
    Vector3f getColor() {
        return Vector3f(color.r, color.g, color.b);
    }
};
class Juego {
    public:
    Player p1;
    Player p2;
    bool funciona;
    int anchopan, altpan;

    Juego(Player p1, Player p2, int anchopan, int altpan)
        : p1(p1), p2(p2), anchopan(anchopan), altpan(altpan), funciona(true){
        campodj = new bool* [anchopan];
        for (int i = 0; i < anchopan; ++i) {
            campodj[i] = new bool[altpan]();
        }
    }
    ~Juego() {
        for (int i = 0; i < anchopan; ++i) {
            delete[] campodj[i];
        }
        delete[] campodj;
    }

    void update() {
        for (int i = 0; i < vel; i++) {
            p1.pos(anchopan, altpan);
            p2.pos(anchopan, altpan);
            if (campodj[p1.x][p1.y] == 1) {
                funciona = false;
            }
            if (campodj[p2.x][p2.y] == 1) {
                funciona = false;
            }
            campodj[p1.x][p1.y] = 1;
            campodj[p2.x][p2.y] = 1;
        }
    }
    void controles() {
        if (Keyboard::isKeyPressed(Keyboard::Left)) if (p1.dir != 2) p1.dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) if (p1.dir != 1) p1.dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up)) if (p1.dir != 0) p1.dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down)) if (p1.dir != 3) p1.dir = 0;
        if (Keyboard::isKeyPressed(Keyboard::A)) if (p2.dir != 2) p2.dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::D)) if (p2.dir != 1) p2.dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::W)) if (p2.dir != 0) p2.dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::S)) if (p2.dir != 3) p2.dir = 0;
    }
};
class VentanaDJ {
public:
    RenderWindow window;
    RenderTexture rt;
    Sprite sBackground, sprite;
    Text text;
    Font font;
    VentanaDJ(int anchopan, int altpan) : window(VideoMode(anchopan, altpan), "TRON", Style::Fullscreen) {
        window.setFramerateLimit(60);
        rt.create(anchopan, altpan);
        rt.setSmooth(true);
        sprite.setTexture(rt.getTexture());
        rt.clear();
        Texture texture;
        sBackground.setTexture(texture);
        sBackground.setScale(
            static_cast<float>(anchopan) / texture.getSize().x,
            static_cast<float>(altpan) / texture.getSize().y
        );
        rt.draw(sBackground);
        font.loadFromFile("sansation.ttf");
        text.setFont(font);
        text.setCharacterSize(35);
        text.setString("VICTORIA");
        text.setPosition(anchopan / 2 - 80, 20);
    }
    void render(Juego& juego) {
        if (!juego.funciona) {
            window.draw(text);
            window.display();
            return;
        }
        rt.clear();
        rt.draw(sBackground);
        CircleShape c(3);
        c.setPosition(juego.p1.x, juego.p1.y);
        c.setFillColor(juego.p1.color);
        rt.draw(c);
        c.setPosition(juego.p2.x, juego.p2.y);
        c.setFillColor(juego.p2.color);
        rt.draw(c);
        rt.display();
        window.clear();
        window.draw(sprite);
        window.display();
    }
};
int main() {
    srand(time(0));
    int anchopan = 800, altpan = 800;
    Player p1(Color::Red, anchopan, altpan);
    Player p2(Color::Blue, anchopan, altpan);
    Juego juego(p1,p2,anchopan, altpan);
    VentanaDJ ventanadj(anchopan, altpan);

    while (ventanadj.window.isOpen()) {
        Event e;
        while (ventanadj.window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                ventanadj.window.close();
            }
        }
        juego.controles();
        juego.update();
        ventanadj.render(juego);
    }
    return 0;
}