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
int32_t start = 48;
int32_t destination = start+16;
int32_t grab = 2250;
int32_t drop = 2750;

uint8_t dxl_id[5] = {DXL_ID_1, DXL_ID_2, DXL_ID_3, DXL_ID_4, DXL_ID_5};
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
int32_t buf_path[64][5] ={  {2815, 2215, 1995, 3580, grab}, {2700, 2130, 2060, 3620, grab}, {2550, 1955, 2220, 3540, grab}, {2360, 1955, 2220, 3550, grab}, {2140, 1955, 2220, 3520, grab}, {1960, 2000, 2220, 3475, grab}, {1825, 2130, 2015, 3580, grab}, {1720, 2215, 1940, 3550, grab},
                            {2715, 2320, 1785, 3670, grab}, {2615, 2110, 1990, 3550, grab}, {2480, 2110, 2020, 3570, grab}, {2325, 2110, 2070, 3550, grab}, {2160, 2110, 2055, 3550, grab}, {2025, 2110, 2005, 3550, grab}, {1880, 2200, 2010, 3490, grab}, {1780, 2305, 1850, 3530, grab},
                            {2630, 2420, 1635, 3670, grab}, {2540, 2420, 1720, 3690, grab}, {2420, 2220, 1910, 3550, grab}, {2300, 2215, 1925, 3550, grab}, {2175, 2305, 1845, 3625, grab}, {2040, 2305, 1830, 3610, grab}, {1940, 2425, 1720, 3675, grab}, {1850, 2465, 1650, 3650, grab},
                            {2570, 2450, 1625, 3580, grab}, {2495, 2450, 1650, 3610, grab}, {2390, 2450, 1650, 3660, grab}, {2280, 2450, 1650, 3700, grab}, {2185, 2450, 1660, 3670, grab}, {2070, 2450, 1660, 3640, grab}, {1980, 2450, 1650, 3605, grab}, {1900, 2450, 1575, 3520, grab},
                            
                            {2525, 2420, 1700, 3345, grab}, {2460, 2420, 1715, 3400, grab}, {2360, 2425, 1690, 3450, grab}, {2300, 2190, 2110, 3090, grab}, {2190, 2425, 1710, 3450, grab}, {2110, 2190, 2110, 2990, grab}, {2020, 2425, 1670, 3400, grab}, {1940, 2300, 1915, 3045, grab},
                            {2500, 2445, 1760, 3080, grab}, {2435, 2445, 1760, 3120, grab}, {2350, 2445, 1760, 3175, grab}, {2275, 2445, 1760, 3155, grab}, {2195, 2430, 1760, 3160, grab}, {2120, 2430, 1760, 3270, grab}, {2040, 2430, 1760, 3070, grab}, {1970, 2450, 1765, 2995, grab},
                            {2485, 2605, 1550, 3220, grab}, {2415, 2605, 1530, 3230, grab}, {2340, 2605, 1530, 3230, grab}, {2270, 2585, 1530, 3260, grab}, {2185, 2585, 1530, 3260, grab}, {2130, 2585, 1530, 3260, grab}, {2060, 2585, 1480, 3260, grab}, {2000, 2605, 1595, 3100, grab},
                            {2450, 2635, 1615, 2960, grab}, {2395, 2630, 1610, 2985, grab}, {2330, 2605, 1610, 3070, grab}, {2265, 2605, 1610, 3040, grab}, {2195, 2605, 1595, 3050, grab}, {2140, 2605, 1615, 3050, grab}, {2080, 2600, 1600, 3050, grab}, {2020, 2635, 1610, 2855, grab}};
                             // 1                               2                               3                               4                               5                               6                               7                               8
int32_t grab_path[64][5] ={ {2815, 2215, 2525, 3030, grab}, {2700, 2145, 2615, 3025, grab}, {2550, 2065, 2690, 3055, grab}, {2365, 2015, 2730, 3035, grab}, {2140, 2045, 2690, 3080, grab}, {1950, 2100, 2660, 3030, grab}, {1825, 2180, 2570, 3030, grab}, {1710, 2265, 2465, 3025, grab},
                            {2715, 2340, 2365, 3100, grab}, {2615, 2260, 2480, 3060, grab}, {2480, 2220, 2510, 3080, grab}, {2325, 2170, 2570, 3050, grab}, {2160, 2165, 2565, 3040, grab}, {2025, 2235, 2515, 3040, grab}, {1885, 2275, 2460, 3025, grab}, {1790, 2385, 2310, 3070, grab},
                            {2645, 2435, 2225, 3095, grab}, {2540, 2400, 2280, 3120, grab}, {2425, 2365, 2330, 3115, grab}, {2300, 2370, 2315, 3150, grab}, {2175, 2360, 2325, 3145, grab}, {2045, 2350, 2340, 3090, grab}, {1940, 2350, 2350, 3030, grab}, {1850, 2430, 2230, 3065, grab},
                            {2570, 2635, 1875, 3320, grab}, {2490, 2535, 2050, 3210, grab}, {2390, 2490, 2130, 3180, grab}, {2280, 2455, 2170, 3170, grab}, {2185, 2470, 2145, 3185, grab}, {2085, 2485, 2130, 3160, grab}, {1980, 2570, 1990, 3240, grab}, {1900, 2725, 1705, 3390, grab},
                               
                            {2530, 2725, 1700, 3340, grab}, {2450, 2730, 1695, 3380, grab}, {2370, 2645, 1840, 3300, grab}, {2280, 2520, 2060, 3120, grab}, {2190, 2620, 1890, 3260, grab}, {2110, 2710, 1730, 3365, grab}, {2020, 2705, 1730, 3335, grab}, {1935, 2765, 1620, 3330, grab},
                            {2500, 2825, 1500, 3340, grab}, {2420, 2835, 1490, 3390, grab}, {2350, 2815, 1515, 3395, grab}, {2275, 2830, 1495, 3410, grab}, {2190, 2835, 1490, 3410, grab}, {2120, 2835, 1495, 3370, grab}, {2040, 2875, 1410, 3380, grab}, {1965, 2875, 1420, 3330, grab},
                            {2475, 2830, 1515, 3180, grab}, {2410, 2810, 1540, 3210, grab}, {2340, 2810, 1540, 3230, grab}, {2270, 2810, 1540, 3240, grab}, {2195, 2810, 1540, 3240, grab}, {2130, 2810, 1540, 3220, grab}, {2055, 2820, 1550, 3165, grab}, {2000, 2845, 1515, 3120, grab},
                            {2450, 2760, 1755, 2800, grab}, {2395, 2755, 1755, 2840, grab}, {2330, 2715, 1810, 2860, grab}, {2265, 2760, 1710, 2950, grab}, {2195, 2765, 1705, 2940, grab}, {2140, 2815, 1595, 3000, grab}, {2080, 2820, 1595, 2980, grab}, {2020, 2905, 1455, 3000, grab}};
                           // 1                               2                               3                               4                               5                               6                               7                               8
int32_t drop_path[64][5] ={  {2825, 2250, 2430, 3135, grab}, {2710, 2035, 2665, 3025, grab}, {2550, 2090, 2625, 3120, grab}, {2360, 2000, 2690, 3120, grab}, {2140, 2000, 2690, 3100, grab}, {1960, 2020, 2680, 3035, grab}, {1830, 2100, 2620, 2995, grab}, {1715, 2200, 2500, 3000, grab},
                             {2710, 2275, 2400, 3060, grab}, {2595, 2230, 2445, 3090, grab}, {2470, 2150, 2560, 3035, grab}, {2325, 2100, 2605, 3035, grab}, {2165, 2100, 2605, 3020, grab}, {2025, 2160, 2540, 3025, grab}, {1880, 2215, 2480, 3015, grab}, {1785, 2285, 2400, 3000, grab},
                             {2635, 2440, 2205, 3145, grab}, {2540, 2420, 2205, 3180, grab}, {2430, 2330, 2345, 3115, grab}, {2310, 2315, 2370, 3110, grab}, {2175, 2315, 2370, 3100, grab}, {2065, 2320, 2335, 3110, grab}, {1935, 2350, 2300, 3070, grab}, {1855, 2425, 2190, 3100, grab},
                             {2580, 2525, 2025, 3200, grab}, {2480, 2450, 2160, 3130, grab}, {2395, 2405, 2210, 3120, grab}, {2295, 2405, 2210, 3125, grab}, {2190, 2405, 2210, 3125, grab}, {2075, 2405, 2220, 3080, grab}, {1990, 2480, 2100, 3150, grab}, {1910, 2525, 2050, 3100, grab},
                            
                             {2525, 2645, 1790, 3260, grab}, {2450, 2645, 1795, 3295, grab}, {2365, 2565, 1935, 3212, grab}, {2280, 2560, 1935, 3215, grab}, {2185, 2565, 1935, 3240, grab}, {2110, 2605, 1880, 3240, grab}, {2015, 2605, 1880, 3230, grab}, {1940, 2645, 1815, 3210, grab},
                             {2495, 2725, 1655, 3225, grab}, {2420, 2725, 1655, 3280, grab}, {2350, 2725, 1655, 3300, grab}, {2265, 2725, 1655, 3320, grab}, {2190, 2725, 1655, 3320, grab}, {2120, 2735, 1630, 3320, grab}, {2050, 2750, 1590, 3300, grab}, {1970, 2800, 1525, 3290, grab},
                             {2470, 2805, 1480, 3215, grab}, {2415, 2800, 1520, 3220, grab}, {2330, 2800, 1520, 3250, grab}, {2270, 2800, 1520, 3250, grab}, {2190, 2800, 1520, 3250, grab}, {2120, 2800, 1520, 3250, grab}, {2060, 2835, 1460, 3250, grab}, {1990, 2860, 1420, 3210, grab},
                             {2450, 2910, 1370, 3120, grab}, {2380, 2910, 1375, 3130, grab}, {2320, 2880, 1400, 3200, grab}, {2255, 2880, 1400, 3190, grab}, {2195, 2880, 1400, 3195, grab}, {2130, 2880, 1400, 3190, grab}, {2080, 2900, 1360, 3130, grab}, {2025, 2960, 1270, 3100, grab}};
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
        while(Serial.available()) {
            char buf = Serial.read();
            buf_string.push_back(buf);
            buffer[bufferIndex]  = buf;
            bufferIndex++;
        }
        
        char num = buffer[0];;
        int degree = atoi(&buffer[2]);

        //---------------------------------
        // ACtuator state check
        for (int cnt = SCNT; cnt < NCNT; cnt++) {
            result = dxl_wb.ping(dxl_id[cnt], &model_number, &log);
            if (result == false) {
                Serial.println(log);;
                Serial.print("id : ");
                Serial.print(dxl_id[cnt]);
                Serial.println("Failed to ping");
                result = dxl_wb.reboot(dxl_id[cnt], &log);
                if (result == false) {
                    Serial.println(log);
                    Serial.println("Failed to change joint mode");
                } else {
                    Serial.println("Reset: Succeed to change joint mode");
                }
                result = dxl_wb.jointMode(dxl_id[cnt], 0, 0, &log);
                if (result == false) {
                    Serial.println(log);
                    Serial.println("Failed to change joint mode");
                } else {
                    Serial.println("Succeed to change joint mode");
                }
            }
        }
        //--------------------------------

        if (start >= 64) {
            start -= 64;
        }
        if(destination >= 64) {
            destination -= 64;
        }
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
        case '8':
            command = parseDifferences(buf_string);
            Serial.println(command[0].first);
            Serial.println(command[0].second);
            Serial.println(command[1].first);
            Serial.println(command[1].second);
            Serial.println(buf_string.c_str());
            break;
        case '9':
            command = parseDifferences(buf_string);
            Serial.println(command[0].first);
            Serial.println(command[0].second);
            Serial.println(command[1].first);
            Serial.println(command[1].second);
            Serial.println(buf_string.c_str());
            
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
        case 't':
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
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=20;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -20; }
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
            Serial.println(": END"); delay(2000);
            //**********************************************************************
            do {
                result = dxl_wb.ping(dxl_id[4], &model_number, &log);
                if (result == false) {
                    Serial.println(log);;
                    Serial.print("id : ");
                    Serial.print(dxl_id[4]);
                    Serial.println("Failed to ping");
                    result = dxl_wb.reboot(dxl_id[4], &log);
                    if (result == false) {
                        Serial.println(log);
                        Serial.println("Failed to change joint mode");
                    } else {
                        Serial.println("Reset: Succeed to change joint mode");
                    }
                    result = dxl_wb.jointMode(dxl_id[4], 0, 0, &log);
                    dxl_wb.goalPosition(dxl_id[4], (int32_t)grab);
                    if (result == false) {
                        Serial.println(log);
                        Serial.println("Failed to change joint mode");
                    } else {
                        Serial.println("Succeed to change joint mode");
                    }
                }
                dxl_wb.goalPosition(dxl_id[4], (int32_t)drop); delay(100);
            } while(!result);
            //**********************************************************************
            
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
            loop_num = getMax(present_position, drop_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < drop_path[destination][cnt]) { diff=5;}
                    else if(present_position[cnt] > drop_path[destination][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            //**********************************************************************
            do {
                result = dxl_wb.ping(dxl_id[4], &model_number, &log);
                if (result == false) {
                    Serial.println(log);;
                    Serial.print("id : ");
                    Serial.print(dxl_id[4]);
                    Serial.println("Failed to ping");
                    result = dxl_wb.reboot(dxl_id[4], &log);
                    if (result == false) {
                        Serial.println(log);
                        Serial.println("Failed to change joint mode");
                    } else {
                        Serial.println("Reset: Succeed to change joint mode");
                    }
                    result = dxl_wb.jointMode(dxl_id[4], 0, 0, &log);
                    dxl_wb.goalPosition(dxl_id[4], (int32_t)grab);
                    if (result == false) {
                        Serial.println(log);
                        Serial.println("Failed to change joint mode");
                    } else {
                        Serial.println("Succeed to change joint mode");
                    }
                }
                dxl_wb.goalPosition(dxl_id[4], (int32_t)grab); delay(100);
            } while(!result);
            //**********************************************************************

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
            /*
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
            Serial.println(": END"); delay(100); */
            start+=1; destination = start + 16;
            break;
        case 'd':
            Serial.println("Grab State");
            loop_num = getMax(present_position, buf_path[start], NCNT);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)grab);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -5; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            delay(1000);
            
            loop_num = getMax(present_position, grab_path[start], NCNT);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < grab_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > grab_path[start][cnt]) { diff = -5; }
                    else { diff = 0; continue; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            delay(1000);
            dxl_wb.goalPosition(dxl_id[4], (int32_t)drop);
            delay(1000);

            loop_num = getMax(present_position, buf_path[start], NCNT);
            Serial.println(loop_num);
            for (e = 0; e<loop_num; e+=5) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=5;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -5; }
                    else { diff = 0; continue; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            delay(1000); start+=1;
            Serial.println("END");
            break;
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
        case 'b':
            start -= 1;
            destination = start + 16;
            if (start < 0) {
                start = 63;
            }
            
            if (start >= 64) {
                start -= 64;
            }
            if(destination >= 64) {
                destination -= 64;
            }
            Serial.print("start: ");
            Serial.println(start);
            break;
        case 'p':
            start += 1;
            destination = start + 16;
            if (start >= 64) {
                start -= 64;
            }
            if(destination >= 64) {
                destination -= 64;
            }
            Serial.print("start: ");
            Serial.println(start);
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
    auto countSplitMessage = splitString(message, ":");
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

            size_t beforePosition = strtoul(before.at(1).c_str(), nullptr, 10) * 8 + strtoul(before.at(0).c_str(), nullptr, 10);
            size_t afterPosition = strtoul(after.at(1).c_str(), nullptr, 10) * 8 + strtoul(after.at(1).c_str(), nullptr, 10);
            
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
