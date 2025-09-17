#include <SFML/Graphics.hpp>
#include "Environment.h"
#include "Robot.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "MC");
    window.setFramerateLimit(60);

    Environment Env;
    Robot Karl;
    float fov = 360.f;
    float beams_num = 90.f;
    
    std::cout << "---Control--- \n\n";
    std::cout << "- Robot movement:\n A - turn left   D - turn right   W - go forawrd   S - go backward    Backspace - reset robot posiition in case of colison\n\n";
    std::cout << "- Simulation control:\n R - rescue robot   L - display robot lidar\n\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            Karl.control(event);
        }
        Karl.measureIdle();

        window.clear(sf::Color(30, 30, 30)); 
        Env.draw(window);
        Karl.draw(window);
        Karl.checkCollsion(Env);
        Karl.lidar(window,Env,fov, beams_num);
        //Karl.drawParticles(window);
        Karl.MCL(window, Env, fov, beams_num);
        
        window.display();   
        
    }
return 0;
}