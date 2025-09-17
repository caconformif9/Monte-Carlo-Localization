#include "Robot.h"

void Robot::control(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (killSwitch == false) {
			if (event.key.code == sf::Keyboard::W) {
				sf::Vector2f dir = { std::cos(degToRad(rotation)),std::sin(degToRad(rotation)) };
				position += dir * 3.f;
				circle.setPosition(position);
				detectMove();
			}
			if (event.key.code == sf::Keyboard::S) {
				sf::Vector2f dir = { std::cos(degToRad(rotation)), std::sin(degToRad(rotation)) };
				position -= dir * 3.f;
				circle.setPosition(position);
				detectMove();
			}
			if (event.key.code == sf::Keyboard::A) {
				turn(-5);
				detectMove();
			}
			if (event.key.code == sf::Keyboard::D) {
				turn(5);
				detectMove();
			}

		}
		//distrubutes one half randomly
		if (event.key.code == sf::Keyboard::R) {
			rescue();
		}

		if (event.key.code == sf::Keyboard::L) {
			if (drawRobotLidar) drawRobotLidar = false;
			else drawRobotLidar = true;
		}

		/*if (event.key.code == sf::Keyboard::G) {
			if (drawParticlesLidar) drawParticlesLidar = false;
			else drawParticlesLidar = true;
		}*/

		//reset position in case robot colides
		if (event.key.code == sf::Keyboard::Backspace && killSwitch == true) {
			position.x = 400.f;
			position.y = 300.f;
			circle.setPosition(position);
			circle.setRotation(0);
		}

	}
}


bool Robot::checkCollsion(Environment& env) {
	sf::FloatRect robot_bounds = circle.getGlobalBounds();

	for (auto& it : env.obstacles) {
		sf::FloatRect obstacle_bounds = it.square.getGlobalBounds();
		if (robot_bounds.intersects(obstacle_bounds)) {
			std::cerr << "Colison detected" << std::endl;
			killSwitch = true;
			return true;
		}
	}
	killSwitch = false;
	return false;
}

void Robot::lidar(sf::RenderWindow& window, Environment& env,float fov, float beams_num) {
	
	float radius = circle.getRadius();
	sf::Vector2f centre = circle.getPosition();
	float heading = rotation;
	float best = 800.f;
   
	sf::VertexArray rays(sf::Lines);
	for (float i = -fov/2; i < fov/2; i+=fov/beams_num) {

		float rad_angle = heading * PI / 180.f;
		float rad_iter = i * PI / 180;
		rad_angle += rad_iter;

		sf::Vector2f dir = { std::cos(rad_angle), std::sin(rad_angle) };
		sf::Vector2f origin = centre + dir * radius;

		best = 800.f;

		for (auto& it : env.obstacles) {
			float t = AABB(origin, dir, it.square.getGlobalBounds());
			if (t < best) best = t;
		}

		if (drawRobotLidar) {
			sf::Vector2f end = origin + dir * best;

			rays.append(sf::Vertex(origin, sf::Color(255, 255, 255, 70)));
			rays.append(sf::Vertex(end, sf::Color(0, 255, 0, 200)));

			window.draw(rays);
		}
		//getting the data from lidar   //lidar only works after a move   //it gathers data only after an idle period  
		if (!moveDetection && !lidar_data_saved) {
			lidar_data.push_back(best);
		}
	}
	if (!moveDetection && !lidar_data_saved) {
		lidar_data_saved = true;
		saveLidarData();
	}
	
}

void Robot::axisCheck(float o, float d, float mn, float mx, float& t_min, float& t_max) {

	if (std::abs(d) < 1e-6f) {
		if (o < mn || o >mx) {
			t_min = 1;
			t_max = 0;
		}
		return;
	}

	float t1 = (mn - o) / d;
	float t2 = (mx - o) / d;
	if (t1 > t2) std::swap(t1, t2);

	t_min = std::max(t_min,t1);
	t_max = std::min(t_max, t2);
	
}
float Robot::AABB(sf::Vector2f O, sf::Vector2f D, const sf::FloatRect Obstacle) {
	
	float t_min = 0.f; //minimal non negative t 
	float t_max = std::numeric_limits<float>::infinity(); //max possible t

	axisCheck(O.x,D.x,Obstacle.left,Obstacle.left+Obstacle.width,t_min,t_max);
	axisCheck(O.y, D.y, Obstacle.top, Obstacle.top + Obstacle.height, t_min, t_max);

	if (t_max < t_min) return std::numeric_limits<float>::infinity();
	if (t_min >= 0) return t_min;
	if (t_min < 0 && t_max >= 0) return t_max;

	return std::numeric_limits<float>::infinity();
}

bool Robot::measureIdle() {
	using clock = std::chrono::steady_clock;

	if (!moveDetection) return false;

	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(clock::now() - idleStart);
	if (elapsed.count() >= 2) {
		moveDetection = false;
		lidar_data_saved = false;
		std::cout << "Movement step done. Data collected" << std::endl;
		return true;
	}
	return false;
}


bool Robot::saveLidarData() {
	std::fstream out;
	out.open("C:/Users/alber/Desktop/MC_localization/data/lidar_data.csv", std::ios::out | std::ios::app);
	if (!out) return false;
	for (size_t i = 0; i < lidar_data.size(); ++i) {
		if (i) out << ';';        // separator kolumn (Excel PL lubi ';')
		out << lidar_data[i];
	}
	out << '\n';
	//out.close();
	return true;
}

void Robot::getRange(Particle& particle, Environment& env, float fov, float beams_num) {
	
	//sf::VertexArray rays(sf::Lines);

	//drawStep in case of drawing particles lidar, drawing once every step 
	//int drawStep = 5;
	particle.ranges.clear();
	particle.ranges.reserve(beams_num);
	int j = 0;
	for (float i = -fov / 2; i < fov / 2; i += fov / beams_num) {
		j++;
		float heading = particle.rotation;
		float rad_angle = heading * PI / 180.f;
		float rad_iter = i * PI / 180.f;
		rad_angle += rad_iter;
		sf::Vector2f dir = { std::cos(rad_angle),std::sin(rad_angle) };
		sf::Vector2f origin = particle.position + dir * particle.radius;

		float best = 800.f;

		for (auto& it : env.obstacles) {
			float t = AABB(origin,dir,it.square.getGlobalBounds());
			if (t < best) best = t;
		}

		if (!moveDetection) {
			particle.ranges.push_back(best);
		}




		//drawing particles lidar - do not uncomment if getRanges is multithreaded, SFML is not thread safe
		/*
		if (drawParticlesLidar && j == drawStep) {
			j = 0;
			sf::Vector2f end = origin + dir * best;
			rays.append(sf::Vertex(origin, sf::Color(255, 255, 255, 70)));
			rays.append(sf::Vertex(end, sf::Color(0, 255, 0, 200)));

			window.draw(rays);
		}
		*/
	}

}

void Robot::getRanges(Environment& env, float fov, float beams_num) {
	const size_t N = particles.size();
	const size_t grain = 256; 
	parallel_for(0, N, grain, [&](size_t idx) {
		getRange(particles[idx], env, fov, beams_num); 
		});

	//not parallel version - one thread for whole vector
	/*for (auto& it : particles) {
		it.ranges.clear();
		getRange(it, env, fov, beams_num);
	}
	*/
}

void Robot::resampleMultinomial() {

	if (particles.empty()) return;
	
	//combined probabilities vector
	std::vector<double> cdf(particles.size());
	double acc = 0;

	for (int i = 0; i < particles.size(); i++) {
		acc += particles[i].w;
		cdf[i] = acc;
	}
	cdf.back() = 1.0;

	std::vector<Particle> newColony(particles.size());

	//random number gnerator [0,1)
	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<double> U(0.0, 1.0);
	auto uRand = [&]() { return U(rng); };

	//resampling

	for (size_t i = 0; i < particles.size(); i++) {
		double u = uRand();

		size_t j = std::lower_bound(cdf.begin(), cdf.end(), u) - cdf.begin();

		newColony[i] = particles[j];
	}

	//jitter, adding random noisw

	std::normal_distribution<float> Npos(0, 1.5f);
	std::normal_distribution<float> Nrot(0, 1.f);
	
	for (auto& it : newColony) {
		it.position.x += Npos(rng);
		it.position.y += Npos(rng);
		it.rotation += Nrot(rng);
		
		it.circle_.setPosition(it.position);
		it.circle_.setRotation(it.rotation);
 

	}

	particles.swap(newColony);

}

void Robot::updateWeights() {
	float errors_sum = 0;
	float w_sum = 0;
	if (!lidar_data.empty() && lidar_data.size() == particles[0].ranges.size()) {
		for (int j = 0; j < particles_num; j++) {
			errors_sum = 0;
			for (int i = 0; i < lidar_data.size(); i++) {
				float error = std::fabs(lidar_data[i] - particles[j].ranges[i]);
				particles[j].w = error;
				errors_sum += error;
				//std::cout <<"Lidar mes "<< lidar_data[i]<<"\n";
				//std::cout << "Particle range " << particles[j].ranges[i] << "\n";
				//std::cout <<"Error "<< error << "\n";
			}
			particles[j].w = 1 / (errors_sum + eps);
			w_sum += particles[j].w;
		}
		int i = 0;
		for (auto& it : particles) {
			i++;
			it.w /= w_sum;
			//std::cout << i << ". " << it.w << "\n";
		}
	}
}


void Robot::MCL (sf::RenderWindow & window, Environment & env, float fov, float beams_num) {
	
	//get Ranges + security check
	//MCL only on if the robot doesnt move - saves comp. power for  robot move
	if (!moveDetection) {

		getRanges(env, fov, beams_num);

		std::cout << "Lidar " << lidar_data.size();
		std::cout << " Particle " << particles[0].ranges.size();
		std::cout << "\n";
		if (!lidar_data_saved || moveDetection) return;
		std::cout << "Flags ok" << "\n";

		if (lidar_data.empty() || particles[0].ranges.empty()) return;
		std::cout << "Vectors not empty" << "\n";
		if (lidar_data.size() != particles[0].ranges.size()) return;
		std::cout << "Vectors the same size" << "\n";

		//updateWeights
		updateWeights();
		//resample
		resampleMultinomial();
		
		//colony noise
		colonyNoise();
		
		//draw
		drawParticles(window);

		//if (drawParticlesLidar) drawParticlesLidars(window, fov, beams_num);
	}
}

void Robot::rescue() {
	size_t rescue_colony_size = particles.size() / 2;

	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> xDist(50.f, 750.f);
	std::uniform_real_distribution<float> yDist(50.f, 550.f);
	std::uniform_real_distribution<float> rotDist(0.f, 360.f);

	for (size_t i = 0; i < rescue_colony_size; i++) {
		sf::Vector2f position_(xDist(rng), yDist(rng));
		float rotation_(rotDist(rng));

		particles[i].position = position_;
		particles[i].rotation = rotation_;
		particles[i].w = 0;
		particles[i].ranges.clear();

		particles[i].circle_.setPosition(position_);
		particles[i].circle_.setRotation(rotation_);
	}

}

void Robot::colonyNoise() {
	size_t noise_size = particles.size() / 100;

	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> xDist(50.f, 750.f);
	std::uniform_real_distribution<float> yDist(50.f, 550.f);
	std::uniform_real_distribution<float> rotDist(0.f, 360.f);

	for (size_t i = 0; i < noise_size; i++) {
		sf::Vector2f position_ = { xDist(rng),yDist(rng) };
		float rotation_ = rotDist(rng);

		particles[i].position = position_;
		particles[i].rotation = rotation_;
		particles[i].ranges.clear();
		particles[i].w = 0;

		particles[i].circle_.setPosition(position_);
		particles[i].circle_.setRotation(rotation_);
	}
}
