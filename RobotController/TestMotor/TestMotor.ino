/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Taehun Lim (Darby) */

#include <DynamixelWorkbench.h>

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define BAUDRATE  57600
#define DXL_ID2    2
#define DXL_ID3    3

DynamixelWorkbench dxl_wb;
char buffer1[20];               //통신을 할때 buffer배열에 전송받은 데이터 입력
char bufferIndex1 = 0;
char buffer2[20];
char bufferIndex2 = 0; 

void setup() 
{
  Serial.begin(57600);
  // while(!Serial); // Wait for Opening Serial Monitor

  const char *log;
  bool result = false;

  uint8_t dxl_id2 = DXL_ID2;
  uint8_t dxl_id3 = DXL_ID3;
  uint16_t model_number = 0;

  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }

  result = dxl_wb.ping(dxl_id2, &model_number, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping");
  }
  else
  {
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id2);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }

  result = dxl_wb.jointMode(dxl_id2, 0, 0, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to change joint mode");
  }
  else
  {
    Serial.println("Succeed to change joint mode");
    Serial.println("Dynamixel is moving...");
    /* 
     *  Set init state
     *  #2 - 2250
     *  #3 - 2250
     *  #4 - 2350(?) => need to recheck it and change it
     *  #5 - 2050 / min:1600, max: 2400 => Gripper
     */
    //dxl_wb.goalPosition(dxl_id2, (int32_t)2250);
    //dxl_wb.goalPosition(dxl_id3, (int32_t)2250);
    do {
      while(Serial.available()) {
        buffer1[bufferIndex1]  = Serial.read();   //시리얼 통신으로 버퍼배열에 데이터 수신
        bufferIndex1++;                          //데이터 수신 후 버퍼 인덱스 1 증가
      }
      while(Serial.available()){
        buffer2[bufferIndex2]  = Serial.read();
        bufferIndex2++;
      }
      int pos1 = atoi(buffer1);
      int pos2 = atoi(buffer2);
      if(pos1!=0) {
        int buf = 0;
        Serial.println(pos1);
        //dxl_wb.goalPosition(dxl_id2, (int32_t)pos1);
      }
      if(pos2!=0) {
        int buf = 0;
        Serial.println(pos2);
        //dxl_wb.goalPosition(dxl_id3, (int32_t)pos2);
      }
      delay(1000);
      for(int a=0;a<21;a++) {
        buffer1[a] = NULL;
        buffer2[a] = NULL;
      }
      bufferIndex1 = 0;
      bufferIndex2 = 0;
    }while(1);
  }
}

void loop() 
{

}
