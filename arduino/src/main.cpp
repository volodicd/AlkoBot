#include <Arduino.h>
#include <HX711_ADC.h>


void waiting_mode();
void test_distance();
void action();
void test_ells();
void menu();
void callibration();
void test_pumps();
void drink1();
void drink2();
// classes
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
    LoadCell LoadCell1;
    LoadCell LoadCell2;
    LoadCell LoadCell3;
    LoadCell LoadCells[3] = {LoadCell1, LoadCell2, LoadCell3};

public:
    Sensors(){
        this -> front_sensor = UltraSoundSensor(8,9);
        this -> left_sensor = UltraSoundSensor(10,11);
        this -> back_sensor = UltraSoundSensor(12, 13);
        this -> right_sensor = UltraSoundSensor(23,22);
        this -> cup_detector = UltraSoundSensor(32,33);
        this->LoadCell1.setPins(27, 26);
        this->LoadCell2.setPins(29, 28);
        this->LoadCell3.setPins(30, 31);

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


    unsigned long t = 0;
    int get_weight(int num){
        this->LoadCells[num].begin();
        float calibrationValue = 696.0;
        unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
        boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
        this-> LoadCells[num].start(stabilizingtime, _tare);
        if (this->LoadCells[num].getTareTimeoutFlag()) {
            Serial.println("Error");
            while (1);
        }
        else {
            this->LoadCells[num].setCalFactor(calibrationValue); // set calibration value (float)
        }
        static boolean newDataReady = 0;
        if (this->LoadCells[num].update()) newDataReady = true;
        const int serialPrintInterval = 0;
        // get smoothed value from the dataset:
        if (newDataReady) {
            if (millis() > t + serialPrintInterval) {
                float i = this->LoadCells[num].getData();
                Serial.print("Load_cell output val: ");
                Serial.println(i);
                newDataReady = 0;
                t = millis();
            }
        }
    }

};

Pump pumps = Pump();
Sensors sensors = Sensors();

void setup() {
    Serial.begin(9600);


    pumps.TurnOffAll();
}
void testdrk();
void loop() {
  menu();

}

void testdrk(){
    unsigned long t = millis();
    pumps.TurnOnAll();
    while (millis() - t<= 5000){
        ;
    }
    pumps.TurnOffAll();
    delay(1000000);
    t = millis();
    pumps.TurnOn(3);
    while (millis() - t<= 4000 && sensors.getCupDetector().avarage_distance(10) > 0 && sensors.getCupDetector().avarage_distance(10) < 20){

    }
    pumps.TurnOff(3);
    delay(1000000);
}
void menu(){
    char command;
    if (Serial.available() > 0) {
        command =  Serial.read();
    }
    if(command == 'm'){
       ;
    }
    if(command == 'a'){
        pumps.TurnOffAll();
        command = '0';
        action();
    }
    if(command == 'c'){
        pumps.TurnOffAll();
        command = '0';
        callibration();
    }
    if(command == 'm'){
        pumps.TurnOffAll();
        command = '0';
        waiting_mode();
    }
    if(command == 'd'){
        for(int i = 0; i <= 10; i++){
            command = '0';
            test_distance();
        }
    }
    if(command == 'p'){
        int pump = Serial.read();
        command = '0';
        if(pump > 6){
            test_pumps();
        } else{
            pumps.TurnOn(pump);
            delay(10000);
            pumps.TurnOff(pump);
        }
    }
    if(command == 'l'){
        for(int i = 0; i <= 10; i++){

            test_ells();
        }
        command = '0';
    }
    if(command == '1'){
        command = '0';
        drink1();
        menu();
    }
    if(command == '2'){
        drink2();
        command = '0';
        menu();
    }
}

void test_distance(){
    Serial.println("Test distance");
    int dist = sensors.getFrontSensor().avarage_distance(10);
    Serial.print("Front: ");
    Serial.println(dist);
    int dist1 = sensors.getLeftSensor().avarage_distance(10);
    Serial.print("Left: ");
    Serial.println(dist1);
    int dist2 = sensors.getBackSensor().avarage_distance(10);
    Serial.print("Back: ");
    Serial.println(dist2);
    int dist3 = sensors.getRightSensor().avarage_distance(10);
    Serial.print("Right: ");
    Serial.println(dist3);
    int dist4 = sensors.getCupDetector().avarage_distance(30);
    Serial.print("Cup: ");
    Serial.println(dist4);
    Serial.println("-----------------------");

}


// modes
void waiting_mode(){
    if(sensors.getCupDetector().avarage_distance(30) <= 10 or sensors.getFrontSensor().avarage_distance(30) <= 30){
        Serial.println("w");
        menu();
    }
}

void callibration(){

}

void action(){
    pumps.TurnOnAll();
    delay(1000);
    pumps.TurnOffAll();
    Serial.flush();
        if(Serial.available() > 0){
        int kst = Serial.read() - '0';
            Serial.flush();
        Serial.println("action");
        for(int i = 0; i <= kst; i++){
            int pump_num = Serial.read() - '0';
            Serial.flush();
            Serial.println(pump_num);
            Serial.flush();
            int weight = Serial.read() - '0';
            int time = millis();
            pumps.TurnOn(pump_num);
            while (millis() - time < (weight/20*1000 + 1500) &&  sensors.getCupDetector().avarage_distance(10) < 10 && sensors.getCupDetector().avarage_distance(10) > 0){
                ;
            }
            delay(1000);
            pumps.TurnOff(pump_num);
            Serial.println("end");
            }
    }
}


// testing functions
void test_ells(){
    Serial.println("Test cells");
    int weight1 = sensors.get_weight(0);
    Serial.print("first: ");
    Serial.println(weight1);
    int weight2 = sensors.get_weight(1);
    Serial.print("second: ");
    Serial.println(weight2);
    int weight3 = sensors.get_weight(2);
    Serial.print("third: ");
    Serial.println(weight3);
    Serial.println("-----------------------");
    delay(10000);
}
//
void test_pumps(){
    for(int i = 0; i <= 1; i++){
        pumps.TurnOn(i);
        delay(1000);
        pumps.TurnOff(i);
    }
}

void drink1(){
    int time = millis();
    pumps.TurnOn(1);
    while ((millis() - time) < 4000 &&  sensors.getCupDetector().avarage_distance(10) < 20 && sensors.getCupDetector().avarage_distance(10) > 0){
        ;
    }
    pumps.TurnOff(1);
    pumps.TurnOn(2);
    time = millis();
    while (millis() - time < 5000 &&  sensors.getCupDetector().avarage_distance(30) < 20 && sensors.getCupDetector().avarage_distance(10) > 0){
        ;
    }
    pumps.TurnOff(2);
    Serial.flush();
    return;
    menu();
}
void drink2(){
    int time = millis();
    pumps.TurnOn(0);
    while (millis() - time < 4000 &&  sensors.getCupDetector().avarage_distance(10) < 20 && sensors.getCupDetector().avarage_distance(10) > 0){
        ;
    }
    pumps.TurnOff(0);
    pumps.TurnOn(3);
    time = millis();
    while (millis() - time < 1000 &&  sensors.getCupDetector().avarage_distance(10) < 20 && sensors.getCupDetector().avarage_distance(10) > 0){
        ;
    }
    pumps.TurnOff(3);
    Serial.flush();
    return;
    menu();
}