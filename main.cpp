#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;

int speed = 10;
bool** field;
//clase que inicia y da propiedades a los jugadores
class player{
public:
    int x, y, dir;
    Color color;
    player(Color c, int maxX, int maxY){
        x=rand()%maxX;
        y=rand()%maxY;
        color=c;
        dir=rand() % 4;
    }
    void tick(int maxX, int maxY){
        if (dir==0) y+=1;
        if (dir==1) x-=1;
        if (dir==2) x+=1;
        if (dir==3) y-=1;

        if (x >= maxX) x = 0; 
        if (x<0) x=maxX-1;
        if (y >= maxY) y = 0; 
        if (y<0) y=maxY-1;
    }
    Vector3f getColor(){
        return Vector3f(color.r, color.g, color.b);
    }
};
int main(){
    srand(time(0));

    //obtener dimensiones de la pantalla completa
    VideoMode desktopMode=VideoMode::getDesktopMode();
    int anchop=desktopMode.width;
    int altop=desktopMode.height;

    //crear ventana en modo pantalla completa
    RenderWindow window(desktopMode, "", Style::Fullscreen);
    window.setFramerateLimit(60);

    //inicializar el campo de juego
    field=new bool*[anchop];
    for (int i = 0; i < anchop; ++i){
        field[i] = new bool[altop]();
    }
    //carge el fondo de pantalla
    Texture texture;
    texture.loadFromFile("fondo.jpeg");
    Sprite sBackhround(texture);
    sBackhround.setScale(
        static_cast<float>(anchop)/texture.getSize().x,
        static_cast<float>(altop)/texture.getSize().y
    );

    //inicializar los jugadores con un color diferente
    player p1(Color::Red, anchop, altop);
    player p2(Color::Green, anchop, altop);

    Sprite sprite;
    RenderTexture t;
    t.create(anchop, altop);
    t.setSmooth(true);
    sprite.setTexture(t.getTexture());
    t.clear();
    t.draw(sBackhround);

    //Fuente del texto al ganar un jugador y al perder el otro
    Font font;
    font.loadFromFile("sansation.ttf");

    Text text("YOU WIN", font, 35);
    text.setPosition(anchop / 2 - 80, 20);

    Text otext("YOU LOSE", font, 35);
    otext.setPosition(anchop / 2 - 80, altop-50);


    // Musica de fondo 
    SoundBuffer buffer;
    buffer.loadFromFile("OST.ogg");
    Sound musicaf;
    musicaf.setBuffer(buffer);
    musicaf.play();
    musicaf.setLoop(true);

    // Musica de game over
    SoundBuffer obuffer;
    obuffer.loadFromFile("gameover.ogg");
    Sound gameover;
    gameover.setBuffer(obuffer);

    bool Game=true;

    while (window.isOpen()){
        Event e;
        while (window.pollEvent(e)){
            if (e.type == Event::Closed){
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) if (p1.dir != 2) p1.dir=1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) if (p1.dir !=1) p1.dir=2;
        if (Keyboard::isKeyPressed(Keyboard::Up)) if (p1.dir != 0) p1.dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Down)) if (p1.dir != 3) p1.dir=0;

        if (Keyboard::isKeyPressed(Keyboard::A)) if (p2.dir != 2) p2.dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::D)) if (p2.dir != 1) p2.dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::W)) if (p2.dir != 0) p2.dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::S)) if (p2.dir != 3) p2.dir = 0;

        if (!Game){
            window.draw(text);
            window.draw(otext);
            window.display();
            continue;
        }
        for (int i = 0; i < speed; i++){
            p1.tick(anchop, altop);
            p2.tick(anchop, altop);
            if (field[p1.x][p1.y] == 1){
                Game=false;
                text.setFillColor(p2.color);
                otext.setFillColor(p1.color);
                musicaf.stop();
                gameover.play();
            }
            if (field[p2.x][p2.y] == 1){
                Game=false;
                text.setFillColor(p1.color);
                otext.setFillColor(p2.color);
                musicaf.stop();
                gameover.play();
            }
            field[p1.x][p1.y] = 1;
            field[p2.x][p2.y] = 1;

            CircleShape c(3);
            c.setPosition(p1.x, p1.y);
            c.setFillColor(p1.color);
            t.draw(c);
            c.setPosition(p2.x, p2.y);
            c.setFillColor(p2.color);
            t.draw(c);
            t.display();
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
    musicaf.stop();
    gameover.stop();
    // Liberar memoria
    for (int i = 0; i < anchop; ++i){
        delete[] field[i];
    }
    delete[] field;
    return 0;
}
