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
int32_t tmp[5][5][5][5] = {};
uint8_t dxl_id[5] = {DXL_ID_1, DXL_ID_2, DXL_ID_3, DXL_ID_4, DXL_ID_5};
int32_t goal_position[2] = {0, 1023};
int32_t ready_position[5] = {2250, 2250, 2250, 2350, 1600};
int32_t idle_position[5] = {1250, 1230, 3170, 2352, 1900};
int32_t grab_position[5] = {2530, 2100, 2350, 3300, 1900};
int32_t trash_position[5] = {2700, 2100, 2350, 2300, 1900};

int32_t buf_position[5] = {1803, 2475, 1735, 3650, 1800};
int32_t buf2_position[5] = {1800, 2435, 2205, 3235, 1800};

int32_t present_position[5] = {0, 0, 0, 0, 0};
int32_t led[2] = {0, 1};
const uint8_t handler_index = 0;

int32_t test_position[5] = {2660, 2355, 2400, 2950, 1700};

int loop_num = 0;
int diff = 0;
int e = 0;

char bufferIndex = 0;
char buffer[20];

//*******************************************************
// Path list
int32_t buf_path[64][5] ={ {2790, 2235, 2090, 3480, 1800}, {2695, 2140, 2120, 3560, 1800}, {2520, 2045, 2155, 3580, 1800}, {2400, 2050, 2210, 3565, 1800}, {2160, 2050, 2210, 3565, 1800}, {2010, 2045, 2155, 3580, 1800}, {1845, 2140, 2120, 3560, 1800}, {1730, 2310, 2020, 3520, 1800},
                            {2715, 2345, 1980, 3500, 1800}, {2615, 2320, 1960, 3555, 1800}, {2450, 2370, 1970, 3575, 1800}, {2325, 2280, 1980, 3580, 1800}, {2170, 2280, 1980, 3580, 1800}, {2005, 2180, 2035, 3600, 1800}, {1910, 2320, 1935, 3595, 1800}, {1803, 2475, 1735, 2650, 1800},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {}};
int32_t general_path[64][5] ={ {2827, 2214, 2481, 3120, 1835}, {2710, 2105, 2610, 3120, 1800}, {2550, 2020, 2710, 3095, 1750}, {2335, 1990, 2735, 3100, 1800}, {2140, 1990, 2735, 3050, 1800}, {1930, 2140, 2610, 3130, 1800}, {1830, 2240, 2520, 3110, 1800}, {1710, 2350, 2345, 3155, 1800},
                            {2720, 2365, 2285, 3220, 1800}, {2605, 2285, 2435, 3150, 1800}, {2490, 2245, 2460, 3180, 1800}, {2320, 2225, 2545, 3100, 1800}, {2165, 2265, 2495, 3130, 1800}, {2005, 2315, 2430, 3180, 1800}, {1900, 2340, 2380, 3120, 1800}, {1800, 2435, 2205, 3235, 1800},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {}};
int32_t pawn_path[64][5] ={ {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0},
                            {2800, 2215, 2575, 2890, 1700}, {2750, 2050, 2700, 3000, 1700}, {}, {}, {}, {}, {}, {},
                            {2670, 2225, 2600, 2750, 1700}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {},
                            {}, {}, {}, {}, {}, {}, {}, {}};


//*******************************************************

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
    //while(1);
    /* Set init position
    Serial.println("Init State");
    loop_num = getMax(present_position, idle_position, NCNT);
    for (e = 0; e<loop_num; e+=10) {
        for (int cnt=SCNT; cnt < NCNT; cnt++) {
            if(present_position[cnt] < idle_position[cnt]) { diff=10;}
            else if(present_position[cnt] > idle_position[cnt]) { diff = -10; }
            else { diff = 0; continue; }
            present_position[cnt]+=diff;
            dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
        }
    }
    Serial.println("END");
*/
    int32_t start = 3;
    int32_t destination = 8;

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
            loop_num = getMax(present_position, idle_position, NCNT);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < idle_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > idle_position[cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println("END");
            break;
        case 'r':
            Serial.println("Init State");
            loop_num = getMax(present_position, ready_position, NCNT);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < ready_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > ready_position[cnt]) { diff = -20; }
                    else { diff = 0; continue; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println("END");
            break;
        case 't':
            Serial.print("Ready position");
            loop_num = getMax(present_position, ready_position, NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < ready_position[cnt]) { diff=5;}
                    else if(present_position[cnt] > ready_position[cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
            Serial.print("Grab position");
            loop_num = getMax(present_position, general_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < general_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > general_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            dxl_wb.goalPosition(dxl_id[cnt], (int32_t)1400);
            
            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
            Serial.print("Drop position");
            loop_num = getMax(present_position, general_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < general_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > general_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);

            
            Serial.print("Buf position");
            
            Serial.println(": END"); delay(100);
            
            Serial.print("Idle position");
            
            Serial.println(": END"); delay(100);
            break;
        case 'd':
            Serial.println("Grab State");
            loop_num = getMax(present_position, buf_position, NCNT);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)1900);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=10) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_position[cnt]) { diff=10;}
                    else if(present_position[cnt] > buf_position[cnt]) { diff = -10; }
                    else { diff = 0; continue; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            delay(1000);
            
            loop_num = getMax(present_position, buf2_position, NCNT);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=10) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf2_position[cnt]) { diff=10;}
                    else if(present_position[cnt] > buf2_position[cnt]) { diff = -10; }
                    else { diff = 0; continue; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            delay(1000);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)1400);
            delay(1000);

            loop_num = getMax(present_position, buf_position, NCNT);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=10) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_position[cnt]) { diff=10;}
                    else if(present_position[cnt] > buf_position[cnt]) { diff = -10; }
                    else { diff = 0; continue; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            delay(1000);
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
                    Serial.print(cnt+1);
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
