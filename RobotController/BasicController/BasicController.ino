#include <DynamixelWorkbench.h>
#include <vector>
#include <string>

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
int32_t tmp = 0;
size_t start = 1;
size_t destination = 18;
int32_t grab = 2200;
int32_t drop = 2750;

uint8_t dxl_id[5] = {DXL_ID_1, DXL_ID_2, DXL_ID_3, DXL_ID_4, DXL_ID_5};
int32_t goal_position[2] = {0, 1023};
int32_t ready_position[5] = {2250, 2250, 2000, 3000, grab};
int32_t idle_position[5] = {1250, 1230, 3170, 2352, grab};
int32_t grab_position[5] = {2530, 2100, 2350, 3300, grab};
int32_t trash_position[5] = {2700, 2100, 2350, 2300, grab};

int32_t buf_position[5] = {1900, 2495, 1750, 3450, grab};
int32_t buf2_position[5] = {1905, 2675, 1800, 3330, grab};

int32_t present_position[5] = {0, 0, 0, 0, 0};
int32_t led[2] = {0, 1};
const uint8_t handler_index = 0;

int32_t test_position[5] = {2660, 2355, 2400, 2950, 1700};

int loop_num = 0;
int diff = 0;
int e = 0;

std::vector<std::pair<size_t, size_t>> command;
std::string buf_string;
char bufferIndex = 0;
char buffer[20];

std::vector<std::pair<size_t, size_t>> parseDifferences(const std::string& message);
std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);


//*******************************************************
// Path list                 1                               2                               3                               4                               5                               6                               7                               8 
int32_t buf_path[64][5] ={  {2815, 2215, 1995, 3580, grab}, {2710, 2130, 2060, 3640, grab}, {2550, 1955, 2220, 3620, grab}, {2360, 1955, 2220, 3620, grab}, {2140, 1955, 2220, 3620, grab}, {1960, 2000, 2220, 3550, grab}, {1825, 2130, 2070, 3580, grab}, {1720, 2215, 2010, 3550, grab},
                            {2720, 2320, 1840, 3670, grab}, {2590, 2110, 2110, 3550, grab}, {2480, 2110, 2110, 3570, grab}, {2325, 2110, 2110, 3550, grab}, {2160, 2110, 2110, 3550, grab}, {2025, 2110, 2110, 3550, grab}, {1880, 2200, 2010, 3490, grab}, {1780, 2305, 1870, 3530, grab},
                            {2630, 2420, 1710, 3670, grab}, {2540, 2420, 1710, 3690, grab}, {2420, 2220, 1990, 3550, grab}, {2300, 2215, 1990, 3550, grab}, {2175, 2305, 1860, 3625, grab}, {2040, 2305, 1870, 3610, grab}, {1935, 2425, 1720, 3650, grab}, {1850, 2465, 1650, 3650, grab},
                            {2485, 2450, 1650, 3580, grab}, {2495, 2450, 1650, 3580, grab}, {2390, 2450, 1650, 3640, grab}, {2290, 2450, 1650, 3640, grab}, {2185, 2450, 1650, 3670, grab}, {2070, 2450, 1650, 3640, grab}, {1980, 2450, 1650, 3580, grab}, {1900, 2450, 1650, 3520, grab},
                            
                            {2535, 2420, 1700, 3340, grab}, {2460, 2420, 1700, 3400, grab}, {2360, 2425, 1715, 3450, grab}, {2300, 2190, 2110, 3140, grab}, {2190, 2425, 1715, 3450, grab}, {1960, 2190, 2110, 3115, grab}, {2020, 2425, 1715, 3400, grab}, {1940, 2300, 1915, 3180, grab},
                            {2500, 2445, 1760, 3230, grab}, {2435, 2445, 1760, 3230, grab}, {2360, 2445, 1760, 3230, grab}, {2275, 2445, 1760, 3230, grab}, {2195, 2430, 1760, 3270, grab}, {2120, 2430, 1760, 3270, grab}, {2040, 2430, 1760, 3260, grab}, {1970, 2450, 1765, 3205, grab},
                            {2485, 2605, 1530, 3230, grab}, {2415, 2605, 1530, 3230, grab}, {2340, 2605, 1530, 3230, grab}, {2270, 2585, 1530, 3260, grab}, {2190, 2585, 1530, 3260, grab}, {2130, 2585, 1530, 3260, grab}, {2060, 2585, 1530, 3260, grab}, {2000, 2605, 1530, 3220, grab},
                            {2450, 2635, 1610, 2960, grab}, {2395, 2630, 1610, 2965, grab}, {2330, 2605, 1610, 3050, grab}, {2265, 2605, 1610, 3050, grab}, {2195, 2605, 1610, 3050, grab}, {2140, 2605, 1615, 3050, grab}, {2080, 2605, 1615, 3050, grab}, {2020, 2635, 1610, 2965, grab}};

                             // 1                               2                               3                               4                               5                               6                               7                               8
int32_t drop_path[64][5] ={  {2835, 0, 0, 0, grab}, {2700, 0, 0, 0, grab}, {2550, 0, 0, 0, grab}, {2360, 0, 0, 0, grab}, {2140, 0, 0, 0, grab}, {1960, 0, 0, 0, grab}, {1825, 0, 0, 0, grab}, {1720, 0, 0, 0, grab},
                            {2720, 0, 0, 0, grab}, {2590, 0, 0, 0, grab}, {2480, 0, 0, 0, grab}, {2325, 0, 0, 0, grab}, {2160, 0, 0, 0, grab}, {2025, 0, 0, 0, grab}, {1880, 0, 0, 0, grab}, {1780, 0, 0, 0, grab},
                            {2630, 2440, 2205, 3155, grab}, {2540, 2420, 2205, 3240, grab}, {2430, 2330, 2345, 3135, grab}, {2325, 2315, 2370, 3120, grab}, {2175, 2315, 2370, 3120, grab}, {2060, 2320, 2335, 3100, grab}, {1935, 2350, 2300, 3095, grab}, {1850, 2425, 2190, 3110, grab},
                            {2485, 2525, 2025, 3200, grab}, {2490, 2450, 2160, 3130, grab}, {2400, 2405, 2210, 3120, grab}, {2290, 2405, 2210, 3125, grab}, {2190, 2405, 2210, 3125, grab}, {2070, 2405, 2220, 3080, grab}, {1990, 2480, 2100, 3150, grab}, {1910, 2525, 2050, 3135, grab},
                            
                            {2535, 0, 0, 0, grab}, {2460, 0, 0, 0, grab}, {2360, 0, 0, 0, grab}, {2300, 0, 0, 0, grab}, {2190, 0, 0, 0, grab}, {1960, 0, 0, 0, grab}, {2020, 0, 0, 0, grab}, {1940, 0, 0, 0, grab},
                            {2500, 0, 0, 0, grab}, {2435, 0, 0, 0, grab}, {2360, 0, 0, 0, grab}, {2275, 0, 0, 0, grab}, {2195, 0, 0, 0, grab}, {2120, 0, 0, 0, grab}, {2040, 0, 0, 0, grab}, {1970, 0, 0, 0, grab},
                            {2485, 0, 0, 0, grab}, {2415, 0, 0, 0, grab}, {2340, 0, 0, 0, grab}, {2270, 0, 0, 0, grab}, {2190, 0, 0, 0, grab}, {2130, 0, 0, 0, grab}, {2060, 0, 0, 0, grab}, {2000, 0, 0, 0, grab},
                            {2450, 0, 0, 0, grab}, {2395, 0, 0, 0, grab}, {2330, 0, 0, 0, grab}, {2265, 0, 0, 0, grab}, {2195, 0, 0, 0, grab}, {2140, 0, 0, 0, grab}, {2080, 0, 0, 0, grab}, {2020, 0, 0, 0, grab}};

                             // 1                               2                               3                               4                               5                               6                               7                               8
int32_t grab_path[64][5] ={ {2815, 2215, 2525, 3050, grab}, {2710, 2145, 2615, 3070, grab}, {2550, 2065, 2690, 3070, grab}, {2360, 2015, 2730, 3040, grab}, {2140, 2045, 2690, 3050, grab}, {1960, 2100, 2660, 3035, grab}, {1825, 2180, 2570, 3025, grab}, {1720, 2265, 2465, 3025, grab},
                               {2720, 2340, 2355, 3100, grab}, {2620, 2260, 2480, 3060, grab}, {2480, 2225, 2510, 3080, grab}, {2330, 2170, 2570, 3050, grab}, {2160, 2165, 2565, 3040, grab}, {2025, 2235, 2515, 3040, grab}, {1880, 2275, 2460, 3040, grab}, {1790, 2385, 2310, 3070, grab},
                               {2630, 2435, 2225, 3080, grab}, {2540, 2400, 2280, 3130, grab}, {2420, 2365, 2330, 3130, grab}, {2300, 2370, 2315, 3160, grab}, {2175, 2360, 2325, 3145, grab}, {2040, 2350, 2340, 3100, grab}, {1935, 2350, 2350, 3045, grab}, {1850, 2430, 2230, 3070, grab},
                               {2570, 2635, 1875, 3330, grab}, {2490, 2535, 2050, 3210, grab}, {2390, 2490, 2130, 3180, grab}, {2290, 2455, 2170, 3180, grab}, {2185, 2470, 2145, 3185, grab}, {2085, 2485, 2130, 3170, grab}, {1980, 2570, 1990, 3265, grab}, {1900, 2725, 1705, 3390, grab},
                               
                               {2535, 2725, 1700, 3345, grab}, {2450, 2730, 1695, 3380, grab}, {2370, 2645, 1840, 3300, grab}, {2280, 2520, 2060, 3140, grab}, {2190, 2620, 1890, 3270, grab}, {2110, 2710, 1730, 3370, grab}, {2020, 2705, 1730, 3340, grab}, {1935, 2765, 1620, 3340, grab},
                               {2500, 2825, 1500, 3340, grab}, {2420, 2835, 1490, 3390, grab}, {2340, 2815, 1515, 3420, grab}, {2275, 2830, 1495, 3420, grab}, {2190, 2835, 1490, 3430, grab}, {2120, 2835, 1495, 3400, grab}, {2040, 2875, 1410, 3420, grab}, {1965, 2875, 1420, 3340, grab},
                               {2475, 2830, 1515, 3195, grab}, {2410, 2810, 1540, 3220, grab}, {2340, 2810, 1540, 3240, grab}, {2270, 2810, 1540, 3250, grab}, {2190, 2810, 1540, 3250, grab}, {2130, 2810, 1540, 3230, grab}, {2055, 2820, 1550, 3190, grab}, {2000, 2845, 1515, 3140, grab},
                               {2450, 2760, 1755, 2820, grab}, {2395, 2755, 1755, 2840, grab}, {2330, 2715, 1810, 2870, grab}, {2265, 2760, 1710, 2940, grab}, {2195, 2765, 1705, 2940, grab}, {2140, 2815, 1595, 3000, grab}, {2080, 2820, 1595, 2995, grab}, {2020, 2905, 1455, 3010, grab}};

//*******************************************************

void setup() {
    // put your setup code here, to run once:
    Serial.begin(57600);

/*
    while(!Serial) { // Wait for Opening Serial Monitor
        Serial.println("Fail to use Serial");
    }
*/

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
            Serial.print(cnt+1);
            Serial.print(": ");
            Serial.println(get_data);
            present_position[cnt] = get_data;
        }
    }
/*
    //Set init position
    Serial.println("Init State");
    loop_num = getMax(present_position, idle_position, NCNT);
    for (e = 0; e<loop_num; e+=1) {
        for (int cnt=SCNT; cnt < NCNT; cnt++) {
            if(present_position[cnt] < idle_position[cnt]) { diff=1;}
            else if(present_position[cnt] > idle_position[cnt]) { diff = -1; }
            else { diff = 0; continue; }
            present_position[cnt]+=diff;
            dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
        }
    }
    Serial.println("END");
*/

    while(1){
        buf_string.clear();
        
        while(Serial.available()) {
            char buf = Serial.read();
            buf_string.push_back(buf);
            buffer[bufferIndex]  = buf;
            bufferIndex++;
        }
        
        char num = buffer[0];;
        int degree = atoi(&buffer[2]);
        
        //std::string command_message{ "2:1,0$2,2|3,0$1,0" };
        //auto differences = parseDifferences(buf_string);
        
        switch (num) {
        case 0:
            break;
        case 'i':
            Serial.print("Init State");
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
            Serial.println(": END");
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
        case '1':
        {
            delay(100);
            auto differences = parseDifferences(buf_string);
            start = differences[0].first;
            destination = differences[0].second;
            Serial.println(buf_string.c_str());
            Serial.println(start);
            Serial.println(destination);
            Serial.print("Ready position");
            if(start<0 || start >63 ||destination<0 || destination > 63) {
              while(1);
            }
            
            loop_num = getMax(present_position, ready_position, NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < ready_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > ready_position[cnt]) { diff = -20; }
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
            loop_num = getMax(present_position, grab_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < grab_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)drop);
            
            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(1000);
            
            Serial.print("Drop position");
            loop_num = getMax(present_position, grab_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < grab_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)grab); delay(100);

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
            Serial.print("Init State");
            loop_num = getMax(present_position, idle_position, NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < idle_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > idle_position[cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100); start+=1; destination = start + 16;

            Serial.write("OK");
            
            break; 
        }
//=======================================================================
        case '2':
        {
            auto differences = parseDifferences(buf_string);
            start = differences[0].first;
            destination = differences[0].second;
            Serial.print("Ready position");
            loop_num = getMax(present_position, ready_position, NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < ready_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > ready_position[cnt]) { diff = -20; }
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
            loop_num = getMax(present_position, grab_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < grab_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)drop);
            
            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(1000);
            
            Serial.print("Drop position");
            loop_num = getMax(present_position, grab_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < grab_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)grab); delay(100);

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
            Serial.print("Init State");
            loop_num = getMax(present_position, idle_position, NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < idle_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > idle_position[cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100); start+=1; destination = start + 16;
            
//========================================================================================================
        start = differences[1].first;
        destination = differences[1].second;
        Serial.print("Ready position");
            loop_num = getMax(present_position, ready_position, NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < ready_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > ready_position[cnt]) { diff = -20; }
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
            loop_num = getMax(present_position, grab_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < grab_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)drop);
            
            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[start], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(1000);
            
            Serial.print("Drop position");
            loop_num = getMax(present_position, grab_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < grab_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)grab); delay(100);

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
            Serial.print("Init State");
            loop_num = getMax(present_position, idle_position, NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < idle_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > idle_position[cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100); start+=1; destination = start + 16;
            break;
        }
//========================================================================================================
        case '7':
        {
            std::string command_message{ "2:1,0$2,2|3,0$1,0" };
            auto differences = parseDifferences(command_message);

            for (size_t i = 0; i < differences.size(); ++i)
            {
               Serial.println(differences[i].first);
               Serial.println(differences[i].second);
            }

            break; 
        }
        case 'k':
            loop_num = getMax(present_position, grab_path[tmp], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < grab_path[tmp][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[tmp][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            tmp++;
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
        case 'q':
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

//===============================================
// network code

std::vector<std::string> splitString(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> brokenString;
    size_t searchStartIndex = 0;

    while (searchStartIndex < str.size())
    {
        size_t foundIndex = str.find_first_of(delimiter, searchStartIndex);

        if (foundIndex == std::string::npos)
        {
            foundIndex = str.size() + searchStartIndex;
        }

        brokenString.push_back(str.substr(searchStartIndex, foundIndex - searchStartIndex));
        searchStartIndex = foundIndex + delimiter.size();
    }

    return brokenString;
}


std::vector<std::pair<size_t, size_t>> parseDifferences(const std::string& message)
{
    Serial.println(message.c_str());
    
    auto countSplitMessage = splitString(message, ":");
    
    Serial.println(countSplitMessage[0].c_str());
    
    size_t diffCount = strtoul(countSplitMessage.at(0).c_str(), nullptr, 10);

    std::vector<std::pair<size_t, size_t>> parsedResult;

    if (diffCount > 0)
    {
        auto differences = splitString(countSplitMessage.at(1), "|");

        for (size_t index = 0; index < diffCount; ++index)
        { 
            auto beforeAfter = splitString(differences[index], "$");
            auto before = splitString(beforeAfter.at(0), ",");
            auto after = splitString(beforeAfter.at(1), ",");

            size_t beforePosition = strtoul(before.at(1).c_str(), nullptr, 10) * 8 + (7 - strtoul(before.at(0).c_str(), nullptr, 10));
            size_t afterPosition = strtoul(after.at(1).c_str(), nullptr, 10) * 8 + (7 - strtoul(after.at(0).c_str(), nullptr, 10));
          
            parsedResult.emplace_back(
                std::make_pair(beforePosition, afterPosition)
            );
        }
    }

    return parsedResult;
}



//================================================

void loop() {
  // put your main code here, to run repeatedly:

}
