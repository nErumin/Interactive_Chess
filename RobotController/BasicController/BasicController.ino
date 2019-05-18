#include <DynamixelWorkbench.h>
#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define BAUDRATE  57600
#define NCNT      5
#define SCNT      0
#define DXL_ID_1  1
#define DXL_ID_2  2
#define DXL_ID_3  3
#define DXL_ID_4  4
#define DXL_ID_5  5

DynamixelWorkbench dxl_wb;

uint8_t dxl_id[5] = {DXL_ID_1, DXL_ID_2, DXL_ID_3, DXL_ID_4, DXL_ID_5};
int32_t goal_position[2] = {0, 1023};
int32_t init_position[5] = {2250, 2250, 2250, 2350, 1600};
int32_t led[2] = {0, 1};
const uint8_t handler_index = 0;

char bufferIndex = 0;
char buffer[20];

void setup() {
    // put your setup code here, to run once:
    Serial.begin(57600);
    while(!Serial) { // Wait for Opening Serial Monitor
        Serial.println("Fail to use Serial");
    }

    const char *log;
    bool result = false;
    uint16_t model_number = 0;

    // Init Actuator
    result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
    if (result == false) {
        Serial.println(log);
        Serial.println("Failed to init");
    } else {
        Serial.print("Succeeded to init : ");
        Serial.println(BAUDRATE);  
    }

    // Setting Actuator
    for (int cnt = SCNT; cnt < NCNT; cnt++) {
        result = dxl_wb.ping(dxl_id[cnt], &model_number, &log);
        if (result == false) {
            Serial.println(log);
            Serial.println("Failed to ping");
        } else {
            Serial.println("Succeeded to ping");
            Serial.print("id : ");
            Serial.print(dxl_id[cnt]);
            Serial.print(" model_number : ");
            Serial.println(model_number);
        }

        result = dxl_wb.jointMode(dxl_id[cnt], 0, 0, &log);
        if (result == false) {
            Serial.println(log);
            Serial.println("Failed to change joint mode");
        } else {
            Serial.println("Succeed to change joint mode");
        }
    }

    // Set initial goalposition
    for (int cnt=SCNT; cnt < NCNT; cnt++) {
        dxl_wb.goalPosition(dxl_id[cnt], init_position[cnt]);
    }

    while(1){
        while(Serial.available()) {
            buffer[bufferIndex]  = Serial.read();
            bufferIndex++;
        }
        char num = buffer[0];
        int degree = atoi(&buffer[2]);
        switch (num) {
        case 0:
            break;
        case '1':
            Serial.println(num);
            Serial.println(degree);
            dxl_wb.goalPosition(dxl_id[0], (int32_t)degree);
            break;
        case '2':
            Serial.println(num);
            Serial.println(degree);
            dxl_wb.goalPosition(dxl_id[1], (int32_t)degree);
            break;
        case '3':
            Serial.println(num);
            Serial.println(degree);
            dxl_wb.goalPosition(dxl_id[2], (int32_t)degree);
            break;
        case '4':
            Serial.println(num);
            Serial.println(degree);
            dxl_wb.goalPosition(dxl_id[3], (int32_t)degree);
            break;
        case '5':
            Serial.println(num);
            Serial.println(degree);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)degree);
            break;
        case 'i':
            Serial.println("Init State");
            for (int cnt=SCNT; cnt < NCNT; cnt++) {
                dxl_wb.goalPosition(dxl_id[cnt], init_position[cnt]);
            }
            break;
        default:
            break;
        }
        for(int a=0;a<21;a++) {
            buffer[a] = NULL;
        }
        bufferIndex = 0;
        delay(1000);
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}
