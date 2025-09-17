#ifndef ROBOT_H
#define ROBOT_H
#include <SFML/Graphics.hpp>
#include "Environment.h"
#include "ParallelFor.h"
#include <cmath>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <random>
#define PI 3.1415926535f
#define eps 1e-6


class Robot {
private:
	struct Particle {
		sf::Vector2f position;
		float rotation = 0;
		std::vector<float> ranges;
		float w;
		sf::CircleShape circle_;
		float radius = 1.5f;

		Particle() : circle_(radius), position(rand() % 750 + 50 + 1, rand() % 550 + 50 + 1) {
			circle_.setOrigin(radius, radius);
			circle_.setRotation(rotation);
			circle_.setPosition(position);
			circle_.setFillColor(sf::Color::Magenta);
			w = 0;
		}

		void print() {
			std::cout << "Position " << position.x << " " << position.y << "\n";
			std::cout << "Weight " << w << "\n";
			std::cout << "Rotation " << rotation << "\n";
			for (auto& it : ranges) {
				std::cout << it << " ";
			}
		}

	};

	std::vector<Particle> particles;
	int particles_num = 2500;
	
	int epochs = 0;

	sf::CircleShape circle;
	sf::Vector2f position = sf::Vector2f(400.f,300.f);
	float rotation;

	bool killSwitch = false;  //disables robot action
	bool moveDetection = false;
	bool moved = false;
	std::chrono::steady_clock::time_point idleStart;

	std::vector<float> lidar_data;
	bool lidar_data_saved = true;
	bool resampled = false;
	bool drawRobotLidar = true;
	bool drawParticlesLidar = false;
	
	std::fstream effn_log;

public:
	Robot():circle(8.0f) {
		circle.setPosition(position);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setRotation(0);
		circle.setOutlineThickness(1.0f);
		circle.setOutlineColor(sf::Color::Yellow);
		circle.setFillColor(sf::Color::Blue);

		setRotation();

		effn_log.open("C:/Users/alber/Desktop/MC_localization/data/EffN.csv", std::ios::out | std::ios::trunc);
		srand(static_cast<unsigned>(time(nullptr)));

		particles.reserve(particles_num);
		for (int i = 0; i < particles_num; i++) {
			Particle temp = Particle();
			particles.push_back(temp);
		}
	}

	~Robot() {
		if (effn_log) effn_log.close();
	}

	void turn(float deg) {
		circle.setRotation(rotation + deg);
		setRotation();
	}

	void setRotation() {
		rotation = circle.getRotation();
		//return circle.getRotation();
	}
	void draw(sf::RenderWindow& window) {
		window.draw(circle);
	}

	void control(sf::Event& event);


	sf::FloatRect getBounds() {
		return circle.getGlobalBounds();
	}

	float degToRad(float deg) {
		return deg * PI / 180.f;
	}
	bool checkCollsion(Environment& env);

	void lidar(sf::RenderWindow& window,Environment& env,float fov, float beams_num);

	void axisCheck(float o, float d, float mn, float mx, float& t_min, float& t_max);

	float AABB(sf::Vector2f O, sf::Vector2f D, const sf::FloatRect Obstacle);

	void detectMove() {
		using clock = std::chrono::steady_clock;
		moveDetection = true;
		moved = true;
		lidar_data_saved = false;
		lidar_data.clear();
		idleStart = clock::now();
	}
	bool measureIdle();

	bool saveLidarData();

	//////-------------------------------MCL----------------------------------/////

	void getRange(Particle& particle, Environment& env, float fov, float beams_num);

	void getRanges(Environment& env, float fov, float beams_num);
	
	void drawParticles(sf::RenderWindow& window) {
		for (auto& it : particles) {
			window.draw(it.circle_);
		}
	}
	
	void updateWeights();

	void resampleMultinomial();

	void rescue();

	void colonyNoise();
	
	void MCL(sf::RenderWindow& window, Environment& env, float fov, float beams_num);
	
	double EffN();

	bool saveEffN(int epoch, double EffN);

};


#endif

/*void drawParticleLidar(Particle& particle,sf::RenderWindow& window, float fov, float beams_num) {
		sf::VertexArray rays(sf::Lines);
		for (int i = -fov / 2; i < fov / 2; i += fov/beams_num) {
			float heading = particle.rotation;
			float rad_angle = heading * PI / 180.f;
			float rad_iter = i * PI / 180.f;
			rad_angle += rad_iter;
			sf::Vector2f dir = { std::cos(rad_angle),std::sin(rad_angle) };
			sf::Vector2f origin = particle.position + dir * particle.radius;
			float best = particle.ranges[i];
			sf::Vector2f end = origin + dir * best;
			rays.append(sf::Vertex(origin, sf::Color(255, 255, 255, 70)));
			rays.append(sf::Vertex(end, sf::Color(0, 255, 0, 200)));

			window.draw(rays);
		}

	}

	void drawParticlesLidars(sf::RenderWindow& window, float fov, float beams_num) {
		for (auto& it : particles) {
			drawParticleLidar(it,window,fov,beams_num);
		}
	}*/