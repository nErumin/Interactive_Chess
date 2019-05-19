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
int32_t pause_position[5] = {2245, 900, 2740, 3160, 1900};
int32_t grab_position[5] = {2530, 2100, 2350, 3300, 1900};
int32_t trash_position[5] = {2700, 2100, 2350, 2300, 1900};
int32_t present_position[5] = {0, 0, 0, 0, 0};
int32_t led[2] = {0, 1};
const uint8_t handler_index = 0;

int loop_num = 0;
int diff = 0;
int e = 0;

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

    // Get initial position
    for (int cnt=SCNT; cnt < NCNT; cnt++) {
        int32_t get_data = 0;
        result = dxl_wb.itemRead(dxl_id[cnt], "Present_Position", &get_data, &log);
        Serial.println("Get present State");
        if (result == false) {
            Serial.println(log);
            Serial.println("Failed to get present position");
        } else {
            Serial.print(cnt);
            Serial.print(": ");
            Serial.println(get_data);
            present_position[cnt] = get_data;
        }
    }

    // Set init position
    Serial.println("Init State");
    loop_num = getMax(present_position, init_position, NCNT);
    for (e = 0; e<loop_num; e+=10) {
        for (int cnt=SCNT; cnt < NCNT; cnt++) {
            if(present_position[cnt] < init_position[cnt]) { diff=10;}
            else if(present_position[cnt] > init_position[cnt]) { diff = -10; }
            else { diff = 0; }
            present_position[cnt]+=diff;
            dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
        }
    }
    Serial.println("END");

    while(1){
        while(Serial.available()) {
            buffer[bufferIndex]  = Serial.read();
            bufferIndex++;
        }
        char num = buffer[0];;
        int degree = atoi(&buffer[2]);
        switch (num) {
        case 0:
            break;
        case '1':
            Serial.println(num);
            Serial.println(degree);
            if(present_position[0] < degree) { diff=1;}
            else { diff = -1; }
            for(e = present_position[0]; degree != e; e+= diff) {
                dxl_wb.goalPosition(dxl_id[0], (int32_t)e);
                present_position[0] = e;
            }
            break;
        case '2':
            Serial.println(num);
            Serial.println(degree);
            if(present_position[1] < degree) { diff=1;}
            else { diff = -1; }
            for(e = present_position[1]; degree != e; e+= diff) {
                dxl_wb.goalPosition(dxl_id[1], (int32_t)e);
                present_position[1] = e;
            }
            break;
        case '3':
            Serial.println(num);
            Serial.println(degree);
            if(present_position[2] < degree) { diff=1;}
            else { diff = -1; }
            for(e = present_position[2]; degree != e; e+= diff) {
                dxl_wb.goalPosition(dxl_id[2], (int32_t)e);
                present_position[2] = e;
            }
            break;
        case '4':
            Serial.println(num);
            Serial.println(degree);
            if(present_position[3] < degree) { diff=1;}
            else { diff = -1; }
            for(e = present_position[3]; degree != e; e+= diff) {
                dxl_wb.goalPosition(dxl_id[3], (int32_t)e);
                present_position[3] = e;
            }
            break;
        case '5':
            Serial.println(num);
            Serial.println(degree);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)degree);
            break;
        case 'i':
            Serial.println("Init State");
            loop_num = getMax(present_position, init_position, NCNT);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=10) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < init_position[cnt]) { diff=10;}
                    else if(present_position[cnt] > init_position[cnt]) { diff = -10; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println("END");
            break;
        case 'd':
            Serial.println("Grab State");
            loop_num = getMax(present_position, grab_position, NCNT);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)grab_position[4]);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=10) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < grab_position[cnt]) { diff=10;}
                    else if(present_position[cnt] > grab_position[cnt]) { diff = -10; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            delay(2000);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)1400);
            present_position[4] = 1400;
            Serial.println("END");

            
            if(present_position[3] < 2500) { diff=1;}
            else { diff = -1; }
            for(e = present_position[3]; 2500 != e; e+= diff) {
                dxl_wb.goalPosition(dxl_id[3], (int32_t)e);
                present_position[3] = e;
            }
            delay(100);

            if(present_position[0] < 2700) { diff=1;}
            else { diff = -1; }
            for(e = present_position[0]; 2700 != e; e+= diff) {
                dxl_wb.goalPosition(dxl_id[0], (int32_t)e);
                present_position[0] = e;
            }
            delay(1000);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)1900);
            Serial.println("END");
            break;
        case 'g':
            for (int cnt=SCNT; cnt < NCNT; cnt++) {
                int32_t get_data = 0;
                result = dxl_wb.itemRead(dxl_id[cnt], "Present_Position", &get_data, &log);
                Serial.println("Get present State");
                if (result == false) {
                    Serial.println(log);
                    Serial.println("Failed to get present position");
                } else {
                    Serial.print(cnt);
                    Serial.print(": ");
                    Serial.println(get_data);
                }
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

int getMax(int32_t* arr1, int32_t* arr2, int arr_len) {
    int max = 0;
    int buf = 0;
    for (int i=0; i<arr_len; i++) {
        buf = abs(arr1[i]-arr2[i]);
        if(max < buf) { max =buf; }
    }
    return max;
}

void loop() {
  // put your main code here, to run repeatedly:

}
