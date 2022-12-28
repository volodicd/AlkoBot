#include <Arduino.h>
#include "HX711.h"

class Pump{
private:
    int kst;
    int *PumpPin = (int*) malloc(kst * sizeof (int ));

public:
    Pump(int kst, int* pins){
        this->kst = kst;
        for(int i = 2; i <= kst; i++){
            *(this->PumpPin + i) = *(pins + i);
            pinMode(*(pins + i), OUTPUT);
        }
        TurnOffAll();
    }
    Pump(){
        this->kst = 6;
        for(int i = 2; i-2 < kst; i++){
            *(this->PumpPin + i - 2) = i;
            pinMode(i, OUTPUT);
        }
        TurnOffAll();
    }
    void TurnOnAll(){
        for(int i = 0; i <= kst; i++){
            digitalWrite(*(this->PumpPin+i), LOW);
        }
    }
    void TurnOffAll(){
        for(int i = 0; i <= kst; i++){
            digitalWrite(*(this->PumpPin+i), HIGH);
        }
    }
    void TurnOn(int num){
        digitalWrite(*(this->PumpPin+num), LOW);
    }
    void TurnOff(int num){
        digitalWrite(*(this->PumpPin+num), HIGH);
    }

    void PourDrink(int kst, int* pump, bool inTime){
        if(inTime){

        }
    }
};


class UltraSoundSensor{
private:
    int trigPin;
    int echoPin;
public:
    UltraSoundSensor(int trigPin, int echoPin) :  trigPin(trigPin), echoPin(echoPin) {
        SetPins();
    }
    UltraSoundSensor(){
        trigPin = 20;
        echoPin = 21;

    }

    int getDistance(){
        int distance;
        long duration;
        digitalWrite(this->trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(this->trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(this->trigPin, LOW);
        duration = pulseIn(this->echoPin, HIGH);
        distance = duration * 0.034 / 2;
        return distance;
    }
    int getTrigPin() const {
        return trigPin;
    }

    void setTrigPin(int trigPin) {
        UltraSoundSensor::trigPin = trigPin;
    }

    int getEchoPin() const {
        return echoPin;
    }

    void setEchoPin(int echoPin) {
        UltraSoundSensor::echoPin = echoPin;
    }

    int avarage_distance(int n){
        int d = 0;
        for(int i = 0; i < n; i++){
            d += getDistance();
        }
        int avg_dst = d/n;
        return avg_dst;
    }
    void SetPins(){
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }
};



class Sensors{
private:
    UltraSoundSensor front_sensor;
    UltraSoundSensor left_sensor;
    UltraSoundSensor back_sensor;
    UltraSoundSensor right_sensor;
    UltraSoundSensor cup_detector;
    UltraSoundSensor arr[5] = {front_sensor, left_sensor, back_sensor, right_sensor, cup_detector};
public:
    Sensors(){
        this -> front_sensor = UltraSoundSensor(8,9);
        this -> left_sensor = UltraSoundSensor(10,11);
        this -> back_sensor = UltraSoundSensor(12, 13);
        this -> right_sensor = UltraSoundSensor(22,23);
        this -> cup_detector = UltraSoundSensor(24,25);
        for(int i = 0; i <5; i++){
            arr[i].SetPins();
        }
    }

    UltraSoundSensor &getFrontSensor(){
        return front_sensor;
    }

    void setFrontSensor(const UltraSoundSensor &frontSensor) {
        front_sensor = frontSensor;
    }

    UltraSoundSensor &getLeftSensor(){
        return left_sensor;
    }

    void setLeftSensor(const UltraSoundSensor &leftSensor) {
        left_sensor = leftSensor;
    }

    UltraSoundSensor &getBackSensor(){
        return back_sensor;
    }

    void setBackSensor(const UltraSoundSensor &backSensor) {
        back_sensor = backSensor;
    }

    UltraSoundSensor &getRightSensor(){
        return right_sensor;
    }

    void setRightSensor(const UltraSoundSensor &rightSensor) {
        right_sensor = rightSensor;
    }

    UltraSoundSensor &getCupDetector(){
        return cup_detector;
    }

    void setCupDetector(const UltraSoundSensor &cupDetector) {
        cup_detector = cupDetector;
    }

    void check(){
        Serial.println(back_sensor.avarage_distance(30));
        Serial.println(front_sensor.avarage_distance(30));
        Serial.println(cup_detector.avarage_distance(30));
        Serial.println(left_sensor.avarage_distance(30));
        Serial.println(right_sensor.avarage_distance(30));
    }
};

Pump pumps = Pump();
Sensors sensors = Sensors();

void setup() {
    Serial.begin(9600);
    pumps.TurnOnAll();
    delay(20000);
}

void loop() {
    int dist = sensors.getLeftSensor().avarage_distance(30);
    Serial.println(dist);
}




void test_distance(){
    int dist = sensors.getFrontSensor().avarage_distance(30);
    int dist1 = sensors.getLeftSensor().avarage_distance(30);
    int dist2 = sensors.getBackSensor().avarage_distance(30);
    int dist3 = sensors.getRightSensor().avarage_distance(30);
    int dist4 = sensors.getCupDetector().avarage_distance(30);
    Serial.print("Distance1: ");
    Serial.println(dist);
    Serial.print("Distance2: ");
    Serial.println(dist1);
    Serial.print("Distance3: ");
    Serial.println(dist2);
    Serial.print("Distance4: ");
    Serial.println(dist3);
    Serial.print("Distance5: ");
    Serial.println(dist4);
    Serial.println("-----------------------");
    delay(1000);
}

void test_pumps(){
    for(int i = 0; i <= 6; i++){
        pumps.TurnOn(i);
        delay(1000);
        pumps.TurnOff(i);
    }
}

