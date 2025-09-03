#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

int main() {
	// Define window
	const int w = 800;
	const int h = 600;
	sf::RenderWindow window(sf::VideoMode({ w, h }), "Sorting Visualizer", sf::Style::Titlebar | sf::Style::Close);

	// Load font
	sf::Font font("font/PressStart2P-Regular.ttf");

	// Define title text
	sf::Text titleText(font, "Bubble Sort Visualizer", 31);
	titleText.setFillColor(sf::Color(0, 150, 255));

	// Get title text bounds
	sf::FloatRect titleBounds = titleText.getLocalBounds();

	// Set title text origin to the center
	titleText.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });
	titleText.setPosition({ window.getSize().x / 2.f, 100.f });

	// Define instruction text
	sf::Text startText(font, "Press SPACE to start sorting.\n\nPress R to reset.", 15);
	startText.setFillColor(sf::Color::White);

	// Get info text bounds
	sf::FloatRect textBounds = startText.getLocalBounds();

	// Set info text origin to the center
	startText.setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 2.f });
	startText.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f });

	// Flags
	bool sortingStarted = false;
	bool sortingCompleted = false;
	bool isPaused = false;

	// Define vector
	const int n = 127;
	std::vector<int> values(n);

	// Seed random and fill values with random heights
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	for (int i = 0; i < n; ++i) {
		values[i] = std::rand() % (window.getSize().y - 50) + 10;
	}

	// Indices for bubble sort iteration
	int i = 0, j = 0;

	// While window is open
	while (window.isOpen()) {
		// Handle events
		while (std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			} else if (auto key = event->getIf<sf::Event::KeyPressed>()) {
				if (key->scancode == sf::Keyboard::Scan::Space) {
					if (!sortingStarted) {
						sortingStarted = true;
						sortingCompleted = false;
						isPaused = false;
					} else {
						isPaused = !isPaused;
					}
				}

				if (key->scancode == sf::Keyboard::Scan::R) {
					sortingStarted = false;
					i = 0;
					j = 0;

					// Shuffle array
					for (auto& v : values) {
						v = std::rand() % (window.getSize().y - 50) + 10;
					}
				}
			}
		}

		// Clear window
		window.clear(sf::Color(10, 10, 10));

		// Draw texts before sorting starts, else draw the bars
		if (!sortingStarted) {
			window.draw(titleText);
			window.draw(startText);
		} else {
			// Width of each bar
			float barWidth = static_cast<float>(window.getSize().x) / n;

			// Draw bars
			for (int k = 0; k < n; ++k) {
				sf::RectangleShape bar;
		
				float gap = 1.f;
				bar.setSize({ barWidth - gap, static_cast<float>(values[k]) });
				bar.setPosition({ static_cast<float>(k) * barWidth, static_cast<float>(window.getSize().y) - values[k] });

				// Highlight bars being compared
				if (k == j || k == j + 1) {
					bar.setFillColor(sf::Color::Red);
				} else {
					// Map height to color 
					int colorVal = static_cast<int>(255 * values[k] / window.getSize().y);
					bar.setFillColor(sf::Color(colorVal, colorVal, 255));
				}

				window.draw(bar);
			}

			// If sorting is active, and not paused
			if (sortingStarted && !isPaused && !sortingCompleted) {
				// Show info text
				sf::Text info(font, "Sorting...Press SPACE to pause");
				info.setPosition({ 10, 10 });
				info.setCharacterSize(12);

				window.draw(info);

				// Bubble sort visualization state
				if (i < n) {
					if (j < n - i - 1) {
						if (values[j] > values[j + 1]) {
							std::swap(values[j], values[j + 1]);
						}
						j++;
					} else {
						j = 0;
						i++;
					}
				} else {
					sortingCompleted = true;
				}
			}
			
			// If paused, draw overlay and paused text
			if (isPaused) {
				// Overlay
				sf::RectangleShape overlay(sf::Vector2f(window.getSize()));

				// Semi-transparent 
				overlay.setFillColor(sf::Color(0, 0, 0, 127));
				window.draw(overlay);

				// Pause text
				sf::Text pauseText(font, "PAUSED", 32);
				sf::FloatRect pauseTextBounds = pauseText.getLocalBounds();

				pauseText.setOrigin({ pauseTextBounds.size.x / 2.f, pauseTextBounds.size.y / 2.f });
				pauseText.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f });

				window.draw(pauseText);
			}

			// If sorting is done
			if (sortingCompleted) {
				sf::Text done(font, "Sorting Completed!", 12);
				done.setFillColor(sf::Color::Green);
				done.setPosition({ 10, 10 });
				window.draw(done);

				sf::Text reset(font, "Press R to reset.", 12);
				reset.setFillColor(sf::Color::White);
				reset.setPosition({ 10, done.getLocalBounds().size.y + 20});
				window.draw(reset);
				
			}
		}

		// Display
		window.display();
	}

	return 0;
}