//
// Programmer: Fam Trinli
// Fam Trinli's Source Code from: https://www.youtube.com/watch?v=YzhhVHb0WVY
// Description: This program is a car racing game
//
// Modified by: Adrian Alvarez
// Date: 10/27/2023
//
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;

const int num = 8; //checkpoints
int points[num][2] = {300, 610,
                      1270, 430,
                      1380, 2380,
                      1900, 2460,
                      1970, 1700,
                      2550, 1680,
                      2560, 3150,
                      500, 3300}; // coordinates for robot cars

class Car{
public:
    Car(float speed = 2, float angle = 0, int currentCheck = 0, int height = 3648, int width = 2880) {
        setSpeed(speed);
        setAngle(angle);
        setCurrentCheck(currentCheck);
        setBackHeight(height);
        setBackWidth(width);
    }

    void move() { // Function that smoothly moves the direction of the car
        setX(getX() + sin(getAngle()) * getSpeed());
        setY(getY() - cos(getAngle()) * getSpeed());
    }

    void findTarget() { // function to determine where the car should move
        float tx = points[getCurrentCheck()][0];
        float ty = points[getCurrentCheck()][1];
        float beta = getAngle() - atan2(tx - getX(), -ty + getY());
        if (sin(beta) < 0) {
            setAngle(getAngle() + 0.005 * getSpeed());
        }
        else {
            setAngle(getAngle() - 0.005 * getSpeed());
        }
        if ((getX() - tx) * (getX() - tx) + (getY() - ty) * (getY() - ty) < 25 * 25) {
            setCurrentCheck((getCurrentCheck() + 1) % num);
        }
    }

    float getX() const {
        return m_x;
    }
    void setX(float x){
        if (x >= 0 && x <= getBackWidth()) {
            m_x = x;
        }
    }

    float getY() const{
        return m_y;
    }
    void setY(float y){
        if (y >= 0 && y <= getBackHeight()) {
            m_y = y;
        }
    }

    float getSpeed() const{
        return m_speed;
    }
    void setSpeed(float speed){
        m_speed = speed;
    }

    float getAngle() const{
        return m_angle;
    }
    void setAngle(float angle){
        m_angle = angle;
    }

    int getCurrentCheck() const{
        return m_currentCheck;
    }
    void setCurrentCheck(int currentCheck){
        m_currentCheck = currentCheck;
    }

    int getBackHeight() const{
        return backHeight;
    }
    void setBackHeight(int height){
        if (height >= 0) {
            backHeight = height;
        }
    }

    int getBackWidth() const{
        return backWidth;
    }
    void setBackWidth(int width){
        if (width >= 0) {
            backWidth = width;
        }
    }
private:
    float m_x; // x location of the center of the car
    float m_y; // y location of the center of the car
    float m_speed; // speed if the car
    float m_angle; // angle of the car
    int m_currentCheck; // current checkpoint (formally n)
    int backHeight; // height of the background
    int backWidth; // width of the background
};


int main() {
    RenderWindow app(VideoMode(640, 480), "Car Racing Game!"); // creates window
    app.setFramerateLimit(60); // sets render rate

    Texture t1, t2;
    Font f1;
    Music m1;
    if (!t1.loadFromFile("images/background.png")) { return EXIT_FAILURE; } // reads background png
    if (!t2.loadFromFile("images/car.png")) { return EXIT_FAILURE; } // reads car png
    if (!f1.loadFromFile("fonts/PLAYFULCARTOON.ttf")) { return EXIT_FAILURE; } // reads font ttf
    if (!m1.openFromFile("sounds/jazzyfrenchy.ogg")) { return EXIT_FAILURE; } // reads music ogg

    m1.play(); // starts the music
    m1.setLoop(true); // sets the music on a loop

    t1.setSmooth(true);
    t2.setSmooth(true);

    Text text1, text2;
    text1.setFont(f1); // sets the font for the timer
    text1.setCharacterSize(30); // set the character size
    text1.setFillColor(sf::Color::White); // set the color
    text1.setStyle(sf::Text::Bold); // set the text style to bold

    text2.setFont(f1); // sets the font for the game over screen
    text2.setCharacterSize(100);
    text2.setFillColor(sf::Color::White);
    text2.setStyle(sf::Text::Bold);
    text2.setOrigin(-165,-150); // sets the text to the middle of the screen

    Sprite sBackground(t1), sCar(t2); // sets background and car sprite
    sBackground.scale(2, 2);

    sCar.setOrigin(22, 22);
    float carRadius = 22; // sets the radius of the car to 22

    const int numOfCars = 5; // number of cars in the game is set to 5
    Car car[numOfCars];
    for (int i = 0; i < numOfCars; i++) { // for loop to set the starting x, y, and speed of each car
        car[i].setBackHeight(3648);
        car[i].setBackWidth(2880);
        car[i].setX(300 + i * 50);
        car[i].setY(1700 + i * 80);
        car[i].setSpeed(7 + i);
    }

    float speed = 0, angle = 0;
    float maxSpeed = 12.0;
    float acc = 0.2, dec = 0.3;
    float turnSpeed = 0.08;

    int offsetX = 0, offsetY = 0;

    float timer = 0; // sets the timer to 0
    Clock clock;

    bool finish = false; // boolean to determine if the race is finished

    while (app.isOpen()) { // while loop that keeps code running as long as window is open
        float time = clock.getElapsedTime().asSeconds(); // starts the time in seconds
        clock.restart();
        if (!finish){ // if statement that keep the timer going as long as race is not finished
            timer+=time;
        }
        text1.setString("Time: " + std::to_string(timer)); // sets text1 to the timer

        text2.setString("Game Over"); // sets text2 to the game over screen

        // Code for event
        Event e;
        while (app.pollEvent(e)) { // while loop to determine if the window has been closed
            if (e.type == Event::Closed) {
                app.close();
            }
        }

        // could change to true's and false's
        bool Up = 0, Right = 0, Down = 0, Left = 0; // sets all arrow keys to false
        // if statements set up, right, down, and left to true if the key is pressed
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            Up = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            Right = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            Down = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            Left = 1;
        }

        //car movement
        if (Up && speed < maxSpeed) { // increases the car speed if up arrow is pressed
            if (speed < 0) {
                speed += dec;
            }
            else {
                speed += acc;
            }
        }

        if (Down && speed > -maxSpeed) { // decreases the car speed if down arrow is pressed
            if (speed > 0) {
                speed -= dec;
            }
            else {
                speed -= acc;
            }
        }

        if (!Up && !Down) { // decreases speed if up and down key is not pressed
            if (speed - dec > 0) {
                speed -= dec;
            }
            else if (speed + dec < 0) {
                speed += dec;
            }
            else {
                speed = 0;
            }
        }

        if (Right && speed != 0) { // determines the angle the car is turning right while moving
            angle += turnSpeed * speed / maxSpeed;
        }
        if (Left && speed != 0) { // determines the angle the car is turning left while moving
            angle -= turnSpeed * speed / maxSpeed;
        }

        car[0].setSpeed(speed); // sets the speed of the players car
        car[0].setAngle(angle); // sets the angle of the players car

        for (int i = 0; i < numOfCars; i++) { // for loop that turns the cars
            car[i].move();
        }
        for (int i = 1; i < numOfCars; i++) { // for loop that moves the cars to a certain point
            car[i].findTarget();
        }

        //collision
        for (int i = 0; i < numOfCars; i++) { // for loops that determine the distance between two cars
            for (int j = 0; j < numOfCars; j++) {
                int dx = 0, dy = 0; // distance x and y are set to 0
                bool collision = true; // collision of cars is set to true
                while (collision && (dx * dx + dy * dy < 4 * carRadius * carRadius)) { // while loop that makes cars not phase through each other
                    car[i].setX(car[i].getX() + dx/10.0);
                    car[i].setY(car[i].getY() + dy / 10.0);
                    car[j].setX(car[j].getX() - dx/10.0);
                    car[j].setY(car[j].getY() - dy / 10.0);
                    dx = car[i].getX() - car[j].getX();
                    dy = car[i].getY() - car[j].getY();
                    if (!dx && !dy) { // if cars aren't next to each other than they are not colliding
                        collision = false;
                    }
                }
            }
        }


        app.clear(Color::White); // clears the window for drawing

        if (car[0].getX() > 320) { // offsets the x position of the car if it's too big
            offsetX = car[0].getX() - 320;
        }
        if (car[0].getY() > 240) { // offsets the y position of the car if it's too big
            offsetY = car[0].getY() - 240;
        }

        sf::CircleShape circle(100); // makes checkpoint circle with radius 100
        circle.setFillColor(sf::Color(255,255,255,128)); // sets circle color to white and transparent

        CircleShape boost(50); // makes boost circle with radius 50
        boost.setFillColor(Color(128, 0, 128, 120)); // sets circle color to purple and transparent

        sBackground.setPosition(-offsetX, -offsetY); // sets the background position
        app.draw(sBackground); // draws on the window

        Color colors[10] = {Color::Red, Color::Green, Color::Magenta, Color::Blue, Color::White};

        for (int i = 0; i < numOfCars; i++) { // draws the cars onto the window
            sCar.setPosition(car[i].getX() - offsetX, car[i].getY() - offsetY);
            sCar.setRotation(car[i].getAngle() * 180 / 3.141593);
            sCar.setColor(colors[i]);
            app.draw(sCar);
        }

        float tx = points[car[0].getCurrentCheck()][0]; // finds the x cord of each checkpoint
        float ty = points[car[0].getCurrentCheck()][1]; // finds the y cord of each checkpoint

        // if statement that sets the checkpoint after the users car passes through
        if ((car[0].getX() - tx) * (car[0].getX() - tx) + (car[0].getY() - ty) * (car[0].getY() - ty) < 100 * 100) {
            car[0].setCurrentCheck((car[0].getCurrentCheck() + 1) % num);
            if (car[0].getCurrentCheck() == 0){ // if statement to check when the car hits the finish line/last checkpoint
                finish = true;
            }
        }
        circle.setPosition(tx-offsetX, ty-offsetY); // sets where the checkpoint will be
        circle.setOrigin(80,100); // makes the checkpoint somewhat on the track

        boost.setPosition(1330-offsetX, 1700-offsetY); // sets the position of the booster pad
        // if statements that determines if the car hits the booster pad
        if((car[0].getX()-(1330)) * (car[0].getX()-(1330)) + (car[0].getY()-(1700)) * (car[0].getY()-(1700)) < (100 * 100)){
            maxSpeed = 15;
        }

        if (finish == false){ // if statement that draws everything if race is not finished
            app.draw(circle);
            app.draw(text1);
            app.draw(boost);
        }

        if (finish == true){ // if statement that executes code if race is finished
            text1.setOrigin(-280, -280); // sets the time taken to complete the race under game over
            speed =0; // stops the users car
            app.draw(text2); // displays game over screen
            app.draw(text1); // displays total time taken to finish race
            if (e.type == Event::KeyPressed) { // if statement to determine if R was pressed
                if (e.key.code == Keyboard::R) {
                    for (int i = 0; i < numOfCars; i++) { // for loop to set the starting x, y, and speed of each car
                        car[i].setBackHeight(3648);
                        car[i].setBackWidth(2880);
                        car[i].setX(300 + i * 50);
                        car[i].setY(1700 + i * 80);
                        car[i].setSpeed(7 + i);
                        car[i].setAngle(0);
                        car[i].setCurrentCheck(0);
                    }
                    speed = 0; // resets car speed
                    angle = 0; // resets car angle
                    offsetX = 0;
                    offsetY= 0;
                    maxSpeed = 12; // resets max speed of car
                    finish = false; // starts the race
                    m1.stop(); // stops music
                    m1.play(); // plays music from the beginning
                    text1.setOrigin(0,0); // puts the time back in the top left corner
                    timer = 0; // resets timer

                }
            }
        }

        app.display(); // displays everything
    }
    return 0;
}
