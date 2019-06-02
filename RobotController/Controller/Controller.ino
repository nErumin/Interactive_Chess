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

int32_t ready_position[5] = {2250, 2250, 2000, 3000, grab};
int32_t idle_position[5] = {1250, 1230, 3170, 2352, grab};
int32_t grab_position[5] = {2530, 2100, 2350, 3300, grab};
int32_t trash_position[5] = {1100, 2250, 2000, 3500, grab};
int32_t buf_position[5] = {1900, 2495, 1750, 3450, grab};
int32_t buf2_position[5] = {1905, 2675, 1800, 3330, grab};
int32_t present_position[5] = {0, 0, 0, 0, 0};
int32_t damp = 5;
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
// Path list                 1                                    2                                    3                                    4                                    5                                    6                                    7                                    8 
int32_t buf_path[64][5] ={  {2825+damp, 2215, 1995, 3560, grab}, {2700+damp, 2130, 2060, 3620, grab}, {2565+damp, 1955, 2220, 3520, grab}, {2360+damp, 1955, 2220, 3545, grab}, {2145+damp, 1955, 2235, 3510, grab}, {1975+damp, 2000, 2220, 3450, grab}, {1825+damp, 2130, 2015, 3575, grab}, {1710+damp, 2215, 1940, 3535, grab},
                            {2730+damp, 2320, 1785, 3660, grab}, {2615+damp, 2110, 1990, 3540, grab}, {2485+damp, 2110, 2020, 3570, grab}, {2330+damp, 2110, 2070, 3550, grab}, {2170+damp, 2110, 2055, 3550, grab}, {2035+damp, 2110, 2005, 3530, grab}, {1910+damp, 2200, 2010, 3470, grab}, {1800+damp, 2305, 1850, 3510, grab},
                            {2630+damp, 2420, 1635, 3670, grab}, {2540+damp, 2420, 1720, 3695, grab}, {2430+damp, 2220, 1910, 3550, grab}, {2315+damp, 2215, 1925, 3540, grab}, {2185+damp, 2305, 1845, 3605, grab}, {2065+damp, 2305, 1830, 3590, grab}, {1970+damp, 2425, 1720, 3620, grab}, {1865+damp, 2465, 1650, 3615, grab},
                            {2570+damp, 2450, 1625, 3580, grab}, {2495+damp, 2450, 1650, 3610, grab}, {2400+damp, 2450, 1650, 3660, grab}, {2295+damp, 2450, 1650, 3670, grab}, {2200+damp, 2450, 1660, 3650, grab}, {2095+damp, 2450, 1660, 3620, grab}, {1980+damp, 2450, 1650, 3560, grab}, {1915+damp, 2450, 1575, 3500, grab},
                            
                            {2525+damp, 2420, 1700, 3345, grab}, {2460+damp, 2420, 1715, 3400, grab}, {2370+damp, 2425, 1690, 3450, grab}, {2290+damp, 2175, 2110, 3090, grab}, {2200+damp, 2425, 1710, 3420, grab}, {2105+damp, 2190, 2065, 2990, grab}, {2025+damp, 2425, 1670, 3365, grab}, {1955+damp, 2300, 1905, 3045, grab},
                            {2500+damp, 2445, 1760, 3080, grab}, {2435+damp, 2445, 1760, 3120, grab}, {2350+damp, 2445, 1760, 3175, grab}, {2275+damp, 2445, 1750, 3155, grab}, {2200+damp, 2430, 1740, 3160, grab}, {2120+damp, 2430, 1595, 3270, grab}, {2045+damp, 2430, 1720, 3070, grab}, {1975+damp, 2450, 1745, 2995, grab},
                            {2485+damp, 2605, 1550, 3200, grab}, {2415+damp, 2605, 1530, 3230, grab}, {2335+damp, 2605, 1530, 3230, grab}, {2270+damp, 2585, 1530, 3250, grab}, {2205+damp, 2585, 1530, 3230, grab}, {2130+damp, 2585, 1530, 3220, grab}, {2065+damp, 2585, 1480, 3220, grab}, {2000+damp, 2605, 1515, 3100, grab},
                            {2450+damp, 2635, 1615, 2960, grab}, {2395+damp, 2630, 1610, 2985, grab}, {2330+damp, 2605, 1610, 3070, grab}, {2270+damp, 2605, 1620, 3040, grab}, {2210+damp, 2605, 1595, 3020, grab}, {2140+damp, 2605, 1615, 3010, grab}, {2080+damp, 2600, 1600, 2935, grab}, {2015+damp, 2635, 1560, 2855, grab}};
                             // 1                                 2                                    3                                    4                                    5                                    6                                    7                                    8
int32_t grab_path[64][5] ={ {2825+damp, 2215, 2525, 3030, grab}, {2700+damp, 2145, 2615, 3025, grab}, {2565+damp, 2065, 2690, 3030, grab}, {2365+damp, 2015, 2730, 3035, grab}, {2145+damp, 2045, 2690, 3055, grab}, {1975+damp, 2100, 2660, 3010, grab}, {1825+damp, 2180, 2570, 3020, grab}, {1710+damp, 2265, 2465, 3010, grab},
                            {2730+damp, 2340, 2365, 3080, grab}, {2615+damp, 2260, 2480, 3050, grab}, {2485+damp, 2220, 2510, 3060, grab}, {2330+damp, 2170, 2570, 3050, grab}, {2170+damp, 2165, 2565, 3040, grab}, {2035+damp, 2235, 2515, 3020, grab}, {1910+damp, 2275, 2460, 3020, grab}, {1800+damp, 2385, 2310, 3050, grab},
                            {2645+damp, 2435, 2225, 3095, grab}, {2540+damp, 2400, 2280, 3125, grab}, {2430+damp, 2365, 2330, 3105, grab}, {2315+damp, 2370, 2315, 3150, grab}, {2185+damp, 2360, 2325, 3125, grab}, {2065+damp, 2350, 2340, 3080, grab}, {1970+damp, 2350, 2350, 2990, grab}, {1865+damp, 2430, 2230, 3035, grab},
                            {2570+damp, 2635, 1875, 3320, grab}, {2490+damp, 2535, 2050, 3210, grab}, {2400+damp, 2490, 2130, 3160, grab}, {2295+damp, 2455, 2170, 3150, grab}, {2200+damp, 2470, 2145, 3165, grab}, {2095+damp, 2485, 2130, 3150, grab}, {2000+damp, 2570, 1990, 3220, grab}, {1915+damp, 2725, 1705, 3370, grab},
                            
                            {2530+damp, 2725, 1700, 3340, grab}, {2450+damp, 2730, 1695, 3380, grab}, {2370+damp, 2645, 1840, 3280, grab}, {2295+damp, 2520, 2060, 3105, grab}, {2200+damp, 2620, 1890, 3240, grab}, {2105+damp, 2710, 1730, 3325, grab}, {2025+damp, 2705, 1730, 3305, grab}, {1950+damp, 2765, 1620, 3330, grab},
                            {2500+damp, 2825, 1500, 3340, grab}, {2420+damp, 2835, 1490, 3390, grab}, {2350+damp, 2815, 1515, 3375, grab}, {2275+damp, 2830, 1495, 3410, grab}, {2200+damp, 2835, 1490, 3410, grab}, {2120+damp, 2835, 1495, 3370, grab}, {2045+damp, 2875, 1410, 3380, grab}, {1975+damp, 2875, 1420, 3320, grab},
                            {2475+damp, 2830, 1515, 3180, grab}, {2410+damp, 2810, 1540, 3210, grab}, {2335+damp, 2810, 1540, 3210, grab}, {2270+damp, 2810, 1540, 3240, grab}, {2205+damp, 2810, 1540, 3220, grab}, {2130+damp, 2810, 1540, 3210, grab}, {2065+damp, 2820, 1550, 3150, grab}, {2000+damp, 2845, 1515, 3100, grab},
                            {2450+damp, 2760, 1755, 2800, grab}, {2395+damp, 2755, 1755, 2840, grab}, {2330+damp, 2715, 1810, 2820, grab}, {2270+damp, 2760, 1710, 2950, grab}, {2210+damp, 2765, 1705, 2910, grab}, {2140+damp, 2815, 1595, 2990, grab}, {2080+damp, 2820, 1595, 2930, grab}, {2015+damp, 2905, 1455, 2960, grab}};
                           // 1                                    2                                    3                                    4                                    5                                    6                                    7                                    8
int32_t drop_path[64][5] ={  {2825+damp, 2250, 2430, 3135, grab}, {2710+damp, 2035, 2665, 3025, grab}, {2565+damp, 2090, 2625, 3100, grab}, {2360+damp, 2000, 2690, 3120, grab}, {2145+damp, 2000, 2690, 3060, grab}, {1975+damp, 2020, 2680, 3020, grab}, {1825+damp, 2100, 2620, 2975, grab}, {1710+damp, 2200, 2500, 2980, grab},
                             {2730+damp, 2275, 2400, 3060, grab}, {2595+damp, 2230, 2445, 3080, grab}, {2485+damp, 2150, 2560, 3025, grab}, {2330+damp, 2100, 2605, 3025, grab}, {2170+damp, 2100, 2605, 3010, grab}, {2035+damp, 2160, 2540, 3025, grab}, {1910+damp, 2215, 2480, 3015, grab}, {1810+damp, 2285, 2400, 2990, grab},
                             {2635+damp, 2440, 2205, 3145, grab}, {2540+damp, 2420, 2205, 3190, grab}, {2430+damp, 2330, 2345, 3090, grab}, {2315+damp, 2315, 2370, 3110, grab}, {2185+damp, 2315, 2370, 3080, grab}, {2065+damp, 2320, 2335, 3095, grab}, {1970+damp, 2350, 2300, 3070, grab}, {1865+damp, 2425, 2190, 3090, grab},
                             {2580+damp, 2525, 2025, 3200, grab}, {2480+damp, 2450, 2160, 3130, grab}, {2400+damp, 2405, 2210, 3100, grab}, {2295+damp, 2405, 2210, 3125, grab}, {2200+damp, 2405, 2210, 3110, grab}, {2095+damp, 2405, 2220, 3075, grab}, {2000+damp, 2480, 2100, 3130, grab}, {1915+damp, 2525, 2050, 3090, grab},
                            
                             {2525+damp, 2645, 1790, 3260, grab}, {2450+damp, 2645, 1795, 3295, grab}, {2370+damp, 2565, 1935, 3212, grab}, {2295+damp, 2560, 1935, 3225, grab}, {2200+damp, 2565, 1935, 3220, grab}, {2105+damp, 2605, 1880, 3240, grab}, {2025+damp, 2605, 1880, 3180, grab}, {1955+damp, 2645, 1815, 3180, grab},
                             {2495+damp, 2725, 1655, 3225, grab}, {2420+damp, 2725, 1655, 3280, grab}, {2350+damp, 2725, 1655, 3290, grab}, {2275+damp, 2725, 1655, 3320, grab}, {2200+damp, 2725, 1655, 3300, grab}, {2120+damp, 2735, 1630, 3290, grab}, {2045+damp, 2750, 1590, 3280, grab}, {1975+damp, 2800, 1525, 3260, grab},
                             {2470+damp, 2805, 1480, 3215, grab}, {2415+damp, 2800, 1520, 3220, grab}, {2335+damp, 2800, 1520, 3230, grab}, {2270+damp, 2800, 1520, 3250, grab}, {2205+damp, 2800, 1520, 3250, grab}, {2130+damp, 2800, 1520, 3210, grab}, {2065+damp, 2835, 1460, 3200, grab}, {1990+damp, 2860, 1420, 3210, grab},
                             {2450+damp, 2910, 1370, 3120, grab}, {2380+damp, 2910, 1375, 3130, grab}, {2330+damp, 2880, 1400, 3135, grab}, {2270+damp, 2880, 1400, 3170, grab}, {2210+damp, 2880, 1400, 3155, grab}, {2140+damp, 2880, 1400, 3140, grab}, {2080+damp, 2900, 1360, 3130, grab}, {2015+damp, 2960, 1270, 3100, grab}};
//*******************************************************

void setup() {
    // put your setup code here, to run once:
    Serial.begin(57600);


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

    //Set init position
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
                dxl_wb.goalPosition(dxl_id[4], (int32_t)drop); delay(1000);
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
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=20;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
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
            Serial.println(": END"); delay(1000);
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
                dxl_wb.goalPosition(dxl_id[4], (int32_t)grab); delay(1000);
            } while(!result);
            //**********************************************************************

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=20;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -20; }
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
            Serial.println(": END"); delay(100);

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
            Serial.println(": END"); delay(1000);
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
                dxl_wb.goalPosition(dxl_id[4], (int32_t)drop); delay(1000);
            } while(!result);
            //**********************************************************************
            
            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[start], NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff = 20;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
            Serial.print("Trash State");
            loop_num = getMax(present_position, trash_position, NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < trash_position[cnt]) { diff=20;}
                    else if(present_position[cnt] > trash_position[cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(1000);
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
                dxl_wb.goalPosition(dxl_id[4], (int32_t)grab); delay(1000);
            } while(!result);
            //**********************************************************************
            
//========================================================================================================
            start = differences[1].first;
            destination = differences[1].second;
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
            Serial.println(": END"); delay(1000);
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
                dxl_wb.goalPosition(dxl_id[4], (int32_t)drop); delay(1000);
            } while(!result);
            //**********************************************************************
            
            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[start], NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[start][cnt]) { diff=20;}
                    else if(present_position[cnt] > buf_path[start][cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=20;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -20; }
                    else { diff = 0; }
                    present_position[cnt]+=diff;
                    dxl_wb.goalPosition(dxl_id[cnt], (int32_t)present_position[cnt]);
                }
            }
            Serial.println(": END"); delay(100);
            
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
            Serial.println(": END"); delay(1000);
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
                dxl_wb.goalPosition(dxl_id[4], (int32_t)grab); delay(1000);
            } while(!result);
            //**********************************************************************

            Serial.print("Buf position");
            loop_num = getMax(present_position, buf_path[destination], NCNT);
            for (e = 0; e<loop_num; e+=20) {
                for (int cnt=SCNT; cnt < NCNT-1; cnt++) {
                    if(present_position[cnt] < buf_path[destination][cnt]) { diff=20;}
                    else if(present_position[cnt] > buf_path[destination][cnt]) { diff = -20; }
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
            Serial.println(": END"); delay(100);
            Serial.write("OK");
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
