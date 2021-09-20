#include <TimerOne.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//-------타이머 인터럽트를 위한 타이머 카운트------------
int time_count = 0;
int temp_count = 0;
int water_count = 0;
//--------------------------------------------------
 
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;  

#define pump 4


void setup(void)  
{  
  //시리얼 포트 초기화  
  Serial.begin(9600);
  
  pinMode(2, INPUT);
  pinMode(pump, OUTPUT);

  TSensor();
  Timer1.initialize(1000); //1000000μs = 1s, 1000us = 0.001s, 1000us = 1ms
  Timer1.attachInterrupt(timerIsr);

}
    
void loop()
{
  sensors.requestTemperaturesByIndex(0); //첫번째 센서의 온도값 읽어옴
  int W = digitalRead(2);
  if(water_count >= 100){
    
    WaterLevelSensor(W);
    water_count = 0;
  }
  if(temp_count >= 150){
    //센서에서 읽어온 온도를 출력  
    printTemperature(insideThermometer);
    temp_count = 0;
  }
} 

void timerIsr() {
  //전체 코드 카운트
  time_count += 1;
  temp_count += 1;
  water_count += 1;
}

void TSensor()
{
  //1-wire 버스 초기화  
  sensors.begin();
     
  //버스에서 첫번째 장치의 주소를 가져온다.  
  sensors.getAddress(insideThermometer, 0);   
    
  //데이터시트에서 확인결과 9~12비트까지 설정 가능  
  sensors.setResolution(insideThermometer, 10);  
}
    
// 온도를 출력하는 함수  
void printTemperature(DeviceAddress deviceAddress)  
{  
  //섭씨 온도를 가져옴  
  float  tempC = sensors.getTempC(deviceAddress);  
      
  Serial.print("Temp: ");  
  Serial.println(tempC); 
}      

void WaterLevelSensor(int water)
{
  Serial.print("Water: ");
  Serial.println(water); 
  if(water == 1){
    digitalWrite(pump, LOW);
  } else{
    digitalWrite(pump, HIGH);
  }
}
