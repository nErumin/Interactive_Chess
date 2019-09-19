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
#define INIT true

DynamixelWorkbench dxl_wb;
int32_t grab = 510;
int32_t releasing = 50;
int32_t gripper = releasing;

int present = 1;
int goal = 0;

uint8_t dxl_id[5] = {DXL_ID_1, DXL_ID_2, DXL_ID_3, DXL_ID_4, DXL_ID_5};
int32_t base_position[5] = {1500, 1120, 1220, 1740, releasing};
int32_t init_position[5] = {480, 520, 1970, 1500, releasing}; 
int32_t gap_position[5] = {0, 0, 0, 0, 0};
int32_t present_position[5] = {0, 0, 0, 0, 0};
int32_t pos_a[5] = {0, 0, 0, 0, 0};
int32_t pos_b[5] = {0, 0, 0, 0, 0};

int diff = -1;
int gap = 0;
bool flag = false;
bool runner = false;

unsigned long start_timer = 0;
unsigned long end_timer = 0;
unsigned long millisTime = 0;

std::string buf_string;
char bufferIndex = 0;
char buffer[20];

int option = 0;
int loop_num=0;

int a1=0, a2=0, a3=0, a4=0, a5=0,a6=0,a7=0,a8=0;
double c1=0, c2=0, c3=0, c4=0;
double d1=0, d2=0, d3=0, d4=0;

int32_t lowwer_path[64][5] = {
//1                                     2                                    3                                    4                                    5                                    6                                    7                                    8
  {2295, 1460, 1430, 1745, gripper}, {2210, 1225, 1630, 1740, gripper}, {2045, 1110, 1715, 1805, gripper}, {1780,  910, 1800, 1890, gripper}, {1420,  950, 1790, 1890, gripper}, {1130, 1020, 1760, 1820, gripper}, { 950, 1260, 1615, 1715, gripper}, { 835, 1380, 1500, 1710, gripper},
  {2165, 1475, 1410, 1710, gripper}, {2060, 1330, 1550, 1690, gripper}, {1910, 1275, 1600, 1750, gripper}, {1710, 1200, 1655, 1770, gripper}, {1480, 1225, 1635, 1790, gripper}, {1250, 1260, 1610, 1750, gripper}, {1090, 1360, 1530, 1710, gripper}, { 970, 1450, 1440, 1680, gripper},
  {2070, 1595, 1270, 1750, gripper}, {1960, 1520, 1375, 1780, gripper}, {1820, 1425, 1465, 1730, gripper}, {1665, 1385, 1505, 1735, gripper}, {1490, 1365, 1530, 1690, gripper}, {1320, 1410, 1480, 1730, gripper}, {1185, 1480, 1420, 1690, gripper}, {1080, 1525, 1370, 1650, gripper},
  {1985, 1670, 1140, 1770, gripper}, {1880, 1595, 1260, 1735, gripper}, {1760, 1525, 1350, 1710, gripper}, {1640, 1515, 1385, 1680, gripper}, {1500, 1515, 1380, 1680, gripper}, {1365, 1540, 1345, 1700, gripper}, {1250, 1595, 1270, 1720, gripper}, {1195, 1710, 1065, 1730, gripper},

  {1920, 1810,  970, 1810, gripper}, {1830, 1725, 1085, 1770, gripper}, {1730, 1670, 1165, 1730, gripper}, {1625, 1665, 1165, 1750, gripper}, {1510, 1665, 1165, 1770, gripper}, {1400, 1645, 1190, 1720, gripper}, {1290, 1645, 1190, 1680, gripper}, {1200, 1760, 1020, 1775, gripper},
  {1870, 1910,  785, 1850, gripper}, {1785, 1910,  785, 1910, gripper}, {1720, 1920,  960, 1820, gripper}, {1615, 1800,  950, 1845, gripper}, {1515, 1800,  950, 1845, gripper}, {1420, 1800,  950, 1820, gripper}, {1325, 1860,  855, 1860, gripper}, {1255, 1860,  855, 1800, gripper},
  {1830, 2105,  435, 2000, gripper}, {1760, 2020,  580, 1970, gripper}, {1680, 2020,  580, 2000, gripper}, {1600, 2020,  580, 2010, gripper}, {1515, 2020,  580, 2010, gripper}, {1430, 2020,  580, 1990, gripper}, {1355, 2020,  580, 1960, gripper}, {1275, 2020,  580, 1910, gripper},
  {1800, 2280,  125, 2040, gripper}, {1730, 2280,  125, 2080, gripper}, {1660, 2280,  125, 2130, gripper}, {1585, 2280,  125, 2130, gripper}, {1510, 2280,  125, 2130, gripper}, {1435, 2280,  125, 2130, gripper}, {1360, 2260,  125, 2100, gripper}, {1300, 1930,  750, 1605, gripper}
};

int32_t upper_path[64][5] = {
//1                                     2                                    3                                    4                                    5                                    6                                    7                                    8
  {2255, 1280, 1275, 2025, gripper}, {2215, 1200, 1340, 2085, gripper}, {2080, 1075, 1450, 2110, gripper}, {1865,  985, 1515, 2145, gripper}, {1465,  970, 1490, 2190, gripper}, {1155, 1010, 1460, 2160, gripper}, { 940, 1200, 1355, 2070, gripper}, { 815, 1305, 1270, 2050, gripper},
  {2175, 1440, 1140, 2065, gripper}, {2045, 1330, 1260, 2050, gripper}, {1900, 1220, 1340, 2100, gripper}, {1715, 1100, 1425, 2090, gripper}, {1470, 1120, 1400, 2120, gripper}, {1265, 1200, 1380, 2060, gripper}, {1100, 1295, 1280, 2055, gripper}, { 995, 1420, 1170, 2030, gripper},
  {2060, 1545, 1040, 2060, gripper}, {1955, 1415, 1165, 2045, gripper}, {1840, 1355, 1230, 2065, gripper}, {1670, 1275, 1295, 2030, gripper}, {1500, 1350, 1250, 2060, gripper}, {1330, 1345, 1240, 2040, gripper}, {1200, 1400, 1185, 2020, gripper}, {1090, 1500, 1090, 2040, gripper},
  {1985, 1645,  910, 2075, gripper}, {1890, 1570, 1000, 2080, gripper}, {1765, 1470, 1115, 2040, gripper}, {1640, 1445, 1130, 2050, gripper}, {1510, 1450, 1125, 2030, gripper}, {1365, 1465, 1105, 2020, gripper}, {1265, 1570, 1000, 2060, gripper}, {1155, 1610,  940, 2065, gripper},

  {1925, 1725,  795, 2050, gripper}, {1835, 1650,  885, 2035, gripper}, {1730, 1620,  930, 2035, gripper}, {1625, 1585,  975, 2020, gripper}, {1515, 1630,  950, 2040, gripper}, {1400, 1575,  980, 1955, gripper}, {1290, 1630,  895, 1995, gripper}, {1210, 1690,  825, 2050, gripper},
  {1870, 1960,  365, 2230, gripper}, {1790, 1710,  740, 2030, gripper}, {1695, 1730,  760, 2040, gripper}, {1595, 1735,  760, 2060, gripper}, {1505, 1755,  730, 2110, gripper}, {1425, 1740,  795, 2000, gripper}, {1325, 1845,  595, 2130, gripper}, {1270, 1850,  565, 2130, gripper},
  {1840, 1980,  440, 2050, gripper}, {1770, 1840,  665, 1940, gripper}, {1690, 1765,  760, 1930, gripper}, {1605, 1735,  810, 1900, gripper}, {1015, 1835,  650, 2020, gripper}, {1430, 1940,  475, 2140, gripper}, {1355, 1950,  455, 2110, gripper}, {1300, 1845,  675, 1860, gripper},
  {1810, 2045,  350, 2000, gripper}, {1720, 2050,  350, 2000, gripper}, {1660, 1980,  420, 2020, gripper}, {1570, 1990,  420, 2020, gripper}, {1505, 1990,  420, 2020, gripper}, {1435, 1965,  445, 2020, gripper}, {1370, 2000,  390, 2020, gripper}, {1325, 2025,  390, 1950, gripper}
};


void setup() {
  // put your setup code here, to run once:
   // put your setup code here, to run once:
    Serial.begin(57600);
    while(!Serial);

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
    Serial.println("Get present State");
    for (int cnt=SCNT; cnt < NCNT; cnt++) {
        int32_t get_data = 0;
        result = dxl_wb.itemRead(dxl_id[cnt], "Present_Position", &get_data, &log);
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

    // Init position
    if (INIT) {
        for (int cnt=3; cnt >= 0; cnt--) {
            gap = present_position[cnt] - init_position[cnt];
            if(gap < 0) { diff = 1; gap *= -1; }
        
            for (int angle=0; angle < gap; angle+=10) {
                dxl_wb.goalPosition(dxl_id[cnt], (int32_t)(present_position[cnt] + angle*diff));
            }
            dxl_wb.goalPosition(dxl_id[cnt], (int32_t)init_position[cnt]);
        }
        
        dxl_wb.goalPosition(DXL_ID_5, (int32_t)releasing);
        
        start_timer = millis()/10;
        while (true) {
            millisTime = millis()/10-start_timer;
            if (millisTime <= 300) {
                int32_t value1 = 480 + (0.034*millisTime*millisTime) - (0.034/450*millisTime*millisTime*millisTime);
                dxl_wb.goalPosition(DXL_ID_1, (int32_t)value1);
                int32_t value2 = 520 + (0.02*millisTime*millisTime) - (millisTime*millisTime*millisTime/(150*150));
                dxl_wb.goalPosition(DXL_ID_2, (int32_t)value2);
               int32_t value3 = 1970 - (millisTime*millisTime/40) + (millisTime*millisTime*millisTime/18000);
               dxl_wb.goalPosition(DXL_ID_3, (int32_t)value3);
               int32_t value4 = 1500 + (0.008*millisTime*millisTime) - (8*millisTime*millisTime*millisTime/450000);
                dxl_wb.goalPosition(DXL_ID_4, (int32_t)value4);
            } else {
                dxl_wb.goalPosition(DXL_ID_4, (int32_t)base_position[3]);
               dxl_wb.goalPosition(DXL_ID_3, (int32_t)base_position[2]);
                dxl_wb.goalPosition(DXL_ID_2, (int32_t)base_position[1]);
                dxl_wb.goalPosition(DXL_ID_1, (int32_t)base_position[0]);
               break;
            }
        }
        flag = true;
    }

    while(1) {
        buf_string.clear();

        // Get command
        while(Serial.available()) {
            char buf = Serial.read();
            buf_string.push_back(buf);
            buffer[bufferIndex]  = buf;
            bufferIndex++;
        }

        // Working loop
        goal = atoi(buffer);
        if (goal != 0) {
          Serial.println(goal);
        }
        
        if(flag) {
            start_timer = millis()/10;
            while (true) {
                millisTime = millis()/10-start_timer;
                if (millisTime <= 200) {
                    int32_t value1 = 1500 + (0.056625*millisTime*millisTime) - (0.0001888*millisTime*millisTime*millisTime);
                    dxl_wb.goalPosition(DXL_ID_1, (int32_t)value1);
                    int32_t value2 = 1120 + (0.012*millisTime*millisTime) - (0.00004*millisTime*millisTime*millisTime);
                    dxl_wb.goalPosition(DXL_ID_2, (int32_t)value2);
                    int32_t value3 = 1220 + (0.004125*millisTime*millisTime) - (0.00001375*millisTime*millisTime*millisTime);
                    dxl_wb.goalPosition(DXL_ID_3, (int32_t)value3);
                    int32_t value4 = 1740 + (0.021375*millisTime*millisTime) - (0.00007125*millisTime*millisTime*millisTime);
                    dxl_wb.goalPosition(DXL_ID_4, (int32_t)value4);
                } else {
                    break;
                }
            }
            flag = false;
        }

        for (int c=0; c<2; c++) {
            if(goal!=0) {
                if(c==0) { option = present; }
                else { option=goal; }
            }
            // switch
            switch (option) {
                case 1:
                    a1=2255, a2=1280, a3=1275, a4=2025;
                    a5=2295, a6=1460, a7=1430, a8=1745;
                    c1=0.003, c2=0.0135, c3=0.011625, c4=-0.021;
                    d1=-0.00001, d2=-0.000045, d3=-0.00003875, d4=0.00007;
                    runner = true;
                break;
                case 2:
                    a1=2215, a2=1200, a3=1340, a4=2085;
                    a5=2210, a6=1225, a7=1630, a8=1740; 
                    c1=-0.000375, c2=0.001875, c3=0.02175, c4=-0.025875;
                    d1=-0.00000125, d2=-0.00000625, d3=-0.0000725, d4=0.00008625;
                    runner = true;
                break;
                case 3:
                    a1=2080, a2=1075, a3=1450, a4=2110;
                    a5=2045, a6=1110, a7=1715, a8=1805;
                    c1=-0.002625, c2=0.002625, c3=0.019875, c4=-0.022875;
                    d1=0.00000875, d2=-0.00000875, d3=-0.00006625, d4=0.00007625;
                    runner = true;
                break;
                case 4:
                    a1=1865, a2=985, a3=1515, a4=2145;
                    a5=1780, a6=910, a7=1800, a8=1890;
                    c1=-0.006375, c2=-0.005625, c3=0.021375, c4=-0.019125;
                    d1=0.00002125, d2=0.00001875, d3=-0.00007125, d4=0.00006375;
                    runner = true;
                break;
                case 5:
                    a1=1465, a2=970, a3=1490, a4=2190;
                    a5=1420, a6=950, a7=1790, a8=1890;
                    c1=-0.003375, c2=-0.0015, c3=0.0225, c4=-0.0225;
                    d1=0.00001125, d2=0.000005, d3=-0.000075, d4=0.000075;
                    runner = true;
                break;
                case 6:
                    a1=1115, a2=1010, a3=1460, a4=2160;
                    a5=1130, a6=1020, a7=1760, a8=1820;
                    c1=0.001125, c2=0.00075, c3=0.0225, c4=-0.0255;
                    d1=-0.00000375, d2=-0.0000025, d3=-0.000075, d4=0.000085;
                    runner = true;
                break;
                case 7:
                    a1=940, a2=1200, a3=1355, a4=2070;
                    a5=950, a6=1260, a7=1615, a8=1715;
                    c1=0.00075, c2=0.0045, c3=0.0195, c4=-0.026625;
                    d1=-0.0000025, d2=-0.000015, d3=-0.000065, d4=0.00008875;
                    runner = true;
                break;
                case 8:
                    a1=815, a2=1305, a3=1270, a4=2050;
                    a5=835, a6=1380, a7=1500, a8=1710;
                    c1=0.0015, c2=0.005625, c3=0.01725, c4=-0.0255;
                    d1=-0.000005, d2=-0.00001875, d3=-0.0000575, d4=0.000085;
                    runner = true;
                break;
                case 9:
                    a1=2175, a2=1440, a3=1140, a4=2065;
                    a5=2165, a6=1475, a7=1410, a8=1710;
                    c1=-0.00075, c2=0.002625, c3=0.02025, c4=-0.026625;
                    d1=0.0000025, d2=-0.00000875, d3=-0.0000675, d4=0.00008875;
                    runner = true;
                break;
                case 10:
                    a1=2045, a2=1330, a3=1260, a4=2050;
                    a5=2060, a6=1330, a7=1550, a8=1690;
                    c1=0.001125, c2=0, c3=0.02175, c4=-0.027;
                    d1=-0.00000375, d2=0, d3=-0.0000725, d4=0.00009;
                    runner = true;
                break;
                case 11:
                    a1=1900, a2=1220, a3=1340, a4=2100;
                    a5=1910, a6=1275, a7=1600, a8=1750;
                    c1=0.00075, c2=0.004125, c3=0.0195, c4=-0.02625;
                    d1=-0.0000025, d2=-0.00001375, d3=-0.000065, d4=0.0000875;
                    runner = true;
                break;
                case 12:
                    a1=1715, a2=1100, a3=1425, a4=2090;
                    a5=1710, a6=1200, a7=1655, a8=1770;
                    c1=-0.000375, c2=0.0075, c3=0.01725, c4=-0.024;
                    d1=0.00000125, d2=-0.000025, d3=-0.0000575, d4=0.00008;
                    runner = true;
                break;
                case 13:
                    a1=1470, a2=1120, a3=1400, a4=2120;
                    a5=1480, a6=1225, a7=1635, a8=1790;
                    c1=0.00075, c2=0.007875, c3=0.017625, c4=-0.02475;
                    d1=-0.0000025, d2=-0.00002625, d3=-0.00005875, d4=0.0000825;
                    runner = true;
                break;
                case 14:
                    a1=1265, a2=1200, a3=1380, a4=2060;
                    a5=1250, a6=1260, a7=1610, a8=1750;
                    c1=-0.001125, c2=0.0045, c3=0.01725, c4=-0.02325;
                    d1=0.00000375, d2=-0.000015, d3=-0.0000575, d4=0.0000775;
                    runner = true;
                break;
                case 15:
                    a1=1100, a2=1295, a3=1280, a4=2055;
                    a5=1090, a6=1360, a7=1530, a8=1710;
                    c1=-0.00075, c2=0.004875, c3=0.01875, c4=-0.025875;
                    d1=0.0000025, d2=-0.00001625, d3=-0.0000625, d4=0.00008625;
                    runner = true;
                break;
                case 16:
                    a1=995, a2=1420, a3=1170, a4=2030;
                    a5=970, a6=1450, a7=1440, a8=1680;
                    c1=-0.001875, c2=0.00225, c3=0.02025, c4=-0.02625;
                    d1=0.00000625, d2=-0.0000075, d3=-0.0000675, d4=0.0000875;
                    runner = true;
                break;
                case 17:
                    a1=2060, a2=1545, a3=1040, a4=2060;
                    a5=2070, a6=1595, a7=1270, a8=1750;
                    c1=0.00075, c2=0.00375, c3=0.01725, c4=-0.02325;
                    d1=-0.0000025, d2=-0.0000125, d3=-0.0000575, d4=0.0000775;
                    runner = true;
                break;
                case 18:
                    a1=1955, a2=1415, a3=1165, a4=2045;
                    a5=1960, a6=1520, a7=1375, a8=1780;
                    c1=0.000375, c2=0.007875, c3=0.01575, c4=-0.019875;
                    d1=-0.00000125, d2=-0.00002625, d3=-0.0000525, d4=0.0000625;
                    runner = true;
                break;
                case 19:
                    a1=1840, a2=1355, a3=1230, a4=2065;
                    a5=1820, a6=1425, a7=1465, a8=1730;
                    c1=-0.0015, c2=0.00525, c3=0.017625, c4=-0.025125;
                    d1=0.000005, d2=-0.0000175, d3=-0.00005875, d4=0.00008375;
                    runner = true;
                break;
                case 20:
                    a1=1670, a2=1275, a3=1295, a4=2030;
                    a5=1665, a6=1385, a7=1505, a8=1735;
                    c1=-0.000375, c2=0.00825, c3=0.01575, c4=-0.022125;
                    d1=0.00000125, d2=-0.0000275, d3=-0.0000525, d4=0.00007375;
                    runner = true;
                break;
                case 21:
                    a1=1500, a2=1350, a3=1250, a4=2060;
                    a5=1490, a6=1365, a7=1530, a8=1690;
                    c1=-0.00075, c2=0.001125, c3=0.021, c4=-0.02775;
                    d1=0.0000025, d2=-0.00000375, d3=-0.00007, d4=0.0000925;
                    runner = true;
                break;
                case 22:
                    a1=1330, a2=1345, a3=1240, a4=2040;
                    a5=1320, a6=1410, a7=1480, a8=1730;
                    c1=-0.00075, c2=0.004875, c3=0.018, c4=-0.02325;
                    d1=0.0000025, d2=-0.00001625, d3=-0.00006, d4=0.0000775;
                    runner = true;
                break;
                case 23:
                    a1=1200, a2=1400, a3=1185, a4=2020;
                    a5=1185, a6=1480, a7=1420, a8=1690;
                    c1=-0.001125, c2=0.006, c3=0.017625, c4=-0.02475;
                    d1=0.00000375, d2=-0.00002, d3=-0.00005875, d4=0.0000825;
                    runner = true;
                break;
                case 24:
                    a1=1090, a2=1500, a3=1090, a4=2040;
                    a5=1080, a6=1525, a7=1370, a8=1650;
                    c1=-0.00075, c2=0.001875, c3=0.021, c4=-0.02925;
                    d1=0.0000025, d2=-0.00000625, d3=-0.00007, d4=0.0000975;
                    runner = true;
                break;
                case 25:
                    a1=1985, a2=1645,  a3=910, a4=2075;
                    a5=1985, a6=1670, a7=1140, a8=1770;
                    c1=0, c2=0.001875, c3=0.01725, c4=-0.022875;
                    d1=0, d2=-0.00000625, d3=-0.0000575, d4=0.00007625;
                    runner = true;
                break;
                case 26:
                    a1=1890, a2=1570, a3=1000, a4=2080;
                    a5=1880, a6=1595, a7=1260, a8=1735;
                    c1=-0.00075, c2=0.001875, c3=0.0195, c4=-0.025875;
                    d1=0.0000025, d2=-0.00000625, d3=-0.000065, d4=0.00008625;
                    runner = true;
                break;
                case 27:
                    a1=1765, a2=1470, a3=1115, a4=2040;
                    a5=1760, a6=1525, a7=1350, a8=1710;
                    c1=-0.000375, c2=0.004125, c3=0.017625, c4=-0.02475;
                    d1=0.00000125, d2=-0.00001375, d3=-0.00005875, d4=0.0000825;
                    runner = true;
                break;
                case 28:
                    a1=1640, a2=1445, a3=1130, a4=2050;
                    a5=1640, a6=1515, a7=1385, a8=1680;
                    c1=0, c2=0.00525, c3=0.019125, c4=-0.02775;
                    d1=0, d2=-0.0000175, d3=-0.00006375, d4=0.0000925;
                    runner = true;
                break;
                case 29:
                    a1=1510, a2=1450, a3=1125, a4=2030;
                    a5=1500, a6=1515, a7=1380, a8=1680;
                    c1=-0.00075, c2=0.004875, c3=0.019125, c4=-0.02625;
                    d1=0.0000025, d2=-0.00001625, d3=-0.00006375, d4=0.0000875;
                    runner = true;
                break;
                case 30:
                    a1=1365, a2=1465, a3=1105, a4=2020;
                    a5=1365, a6=1540, a7=1345, a8=1700;
                    c1=0, c2=0.005625, c3=0.018, c4=-0.024;
                    d1=0, d2=-0.00001875, d3=-0.00006, d4=0.00008;
                    runner = true;
                break;
                case 31:
                    a1=1265, a2=1570, a3=1000, a4=2060;
                    a5=1250, a6=1595, a7=1270, a8=1720;
                    c1=-0.001125, c2=0.001875, c3=0.02025, c4=-0.0255;
                    d1=0.00000375, d2=-0.00000625, d3=-0.0000675, d4=0.000085;
                    runner = true;
                break;
                case 32:
                    a1=1155, a2=1610,  a3=940, a4=2065;
                    a5=1195, a6=1710, a7=1065, a8=1730;
                    c1=0.003, c2=0.0075, c3=0.009375, c4=-0.025125;
                    d1=-0.00001, d2=-0.000025, d3=-0.00003125, d4=0.00008375;
                    runner = true;
                break;
                case 33:
                    a1=1925, a2=1725,  a3=795, a4=2050;
                    a5=1920, a6=1810,  a7=970, a8=1810;
                    c1=-0.000375, c2=0.006375, c3=0.013125, c4=-0.018;
                    d1=0.00000125, d2=-0.00002125, d3=-0.00004375, d4=0.00006;
                    runner = true;
                break;
                case 34:
                    a1=1835, a2=1650,  a3=885, a4=2035;
                    a5=1830, a6=1725, a7=1085, a8=1770;
                    c1=-0.000375, c2=0.005625, c3=0.015, c4=-0.019875;
                    d1=0.00000125, d2=-0.00001875, d3=-0.00005, d4=0.00006625;
                    runner = true;
                break;
                case 35:
                    a1=1730, a2=1620,  a3=930, a4=2035;
                    a5=1730, a6=1670, a7=1165, a8=1730;
                    c1=0, c2=0.00375, c3=0.017625, c4=-0.022875;
                    d1=0, d2=-0.0000125, d3=-0.00005875, d4=0.00007625;
                    runner = true;
                break;
                case 36:
                    a1=1625, a2=1585,  a3=975, a4=2020;
                    a5=1625, a6=1665, a7=1165, a8=1750;
                    c1=0, c2=0.006, c3=0.01425, c4=-0.02025;
                    d1=0, d2=-0.00002, d3=-0.0000475, d4=0.0000675;
                    runner = true;
                break;
                case 37:
                    a1=1515, a2=1630,  a3=950, a4=2040;
                    a5=1510, a6=1665, a7=1165, a8=1770;
                    c1=-0.000375, c2=0.002625, c3=0.016125, c4=-0.02025;
                    d1=0.00000125, d2=-0.00000875, d3=-0.00005375, d4=0.0000675;
                    runner = true;
                break;
                case 38:
                    a1=1400, a2=1575,  a3=980, a4=1955;
                    a5=1400, a6=1645, a7=1190, a8=1720;
                    c1=0, c2=0.00525, c3=0.01575, c4=-0.017625;
                    d1=0, d2=-0.0000175, d3=-0.0000525, d4=0.00005875;
                    runner = true;
                break;
                case 39:
                    a1=1290, a2=1630,  a3=895, a4=1995;
                    a5=1290, a6=1645, a7=1190, a8=1680;
                    c1=0, c2=0.001125, c3=0.022125, c4=-0.023625;
                    d1=0, d2=-0.00000375, d3=-0.00007375, d4=0.00007875;
                    runner = true;
                break;
                case 40:
                    a1=1210, a2=1690,  a3=825, a4=2050;
                    a5=1200, a6=1760, a7=1020, a8=1775;
                    c1=-0.00075, c2=0.00525, c3=0.014625, c4=-0.020625;
                    d1=0.0000025, d2=-0.0000175, d3=-0.00004875, d4=0.00006875;
                    runner = true;
                break;
                case 41:
                    a1=1870, a2=1960,  a3=365, a4=2230;
                    a5=1870, a6=1910,  a7=785, a8=1850;
                    c1=0, c2=-0.00375, c3=0.0315, c4=-0.0285;
                    d1=0, d2=0.0000125, d3=-0.000105, d4=0.000095;
                    runner = true;
                break;
                case 42:
                    a1=1790, a2=1710,  a3=740, a4=2030;
                    a5=1785, a6=1910,  a7=785, a8=1910;
                    c1=-0.000375, c2=0.015, c3=0.003375, c4=-0.009;
                    d1=0.00000125, d2=-0.00005, d3=-0.00001125, d4=0.00003;
                    runner = true;
                break;
                case 43:
                    a1=1695, a2=1730,  a3=760, a4=2040;
                    a5=1720, a6=1920,  a7=960, a8=1820;
                    c1=0.001875, c2=0.01425, c3=0.015, c4=-0.0165;
                    d1=-0.00000625, d2=-0.0000475, d3=-0.00005, d4=0.000055;
                    runner = true;
                break;
                case 44:
                    a1=1595, a2=1735,  a3=760, a4=2060;
                    a5=1615, a6=1800,  a7=950, a8=1845;
                    c1=0.0015, c2=0.004875, c3=0.015, c4=-0.016125;
                    d1=-0.000005, d2=-0.00001625, d3=-0.00005, d4=0.00005375;
                    runner = true;
                break;
                case 45:
                    a1=1505, a2=1755,  a3=730, a4=2110;
                    a5=1515, a6=1800,  a7=950, a8=1845;
                    c1=0.00075, c2=0.003375, c3=0.0165, c4=-0.019875;
                    d1=-0.0000025, d2=-0.00001125, d3=-0.000055, d4=0.00006625;
                    runner = true;
                break;
                case 46:
                    a1=1425, a2=1740,  a3=795, a4=2000;
                    a5=1420, a6=1800,  a7=950, a8=1820;
                    c1=-0.000375, c2=0.0045, c3=0.011625, c4=-0.0135;
                    d1=0.00000125, d2=-0.000015, d3=-0.00003875, d4=0.000045;
                    runner = true;
                break;
                case 47:
                    a1=1325, a2=1845,  a3=595, a4=2130;
                    a5=1325, a6=1860,  a7=855, a8=1860;
                    c1=0, c2=0.001125, c3=0.0195, c4=-0.02025;
                    d1=0, d2=-0.00000375, d3=-0.000065, d4=0.0000675;
                    runner = true;
                break;
                case 48:
                    a1=1270, a2=1850,  a3=565, a4=2130;
                    a5=1255, a6=1860,  a7=855, a8=1800;
                    c1=-0.001125, c2=0.00075, c3=0.02175, c4=-0.02475;
                    d1=0.00000375, d2=-0.0000025, d3=-0.0000725, d4=0.0000825;
                    runner = true;
                break;
                case 49:
                    a1=1840, a2=1980,  a3=440, a4=2050;
                    a5=1830, a6=2105,  a7=435, a8=2000;
                    c1=-0.00075, c2=0.009375, c3=-0.000375, c4=-0.00375;
                    d1=0.0000025, d2=-0.00003125, d3=0.00000125, d4=0.0000125;
                    runner = true;
                break;
                case 50:
                    a1=1770, a2=1840,  a3=665, a4=1940;
                    a5=1760, a6=2020,  a7=580, a8=1970;
                    c1=-0.00075, c2=0.0135, c3=-0.006375, c4=0.00225;
                    d1=0.0000025, d2=-0.000045, d3=0.00002125, d4=-0.0000075;
                    runner = true;
                break;
                case 51:
                    a1=1690, a2=1765,  a3=760, a4=1930;
                    a5=1680, a6=2020,  a7=580, a8=2000;
                    
                    runner = true;
                break;
                case 52:
                    a1=1605, a2=1735,  a3=810, a4=1900;
                    a5=1600, a6=2020,  a7=580, a8=2010;
                    
                    runner = true;
                break;
                case 53:
                    a1=1015, a2=1835,  a3=650, a4=2020;
                    a5=1515, a6=2020,  a7=580, a8=2010;
                    
                    runner = true;
                break;
                case 54:
                    a1=1430, a2=1940,  a3=475, a4=2140;
                    a5=1430, a6=2020,  a7=580, a8=1990;
                    
                    runner = true;
                break;
                case 55:
                    a1=1355, a2=1950,  a3=455, a4=2110;
                    a5=1355, a6=2020,  a7=580, a8=1960;
                    
                    runner = true;
                break;
                case 56:
                    a1=1300, a2=1845,  a3=675, a4=1860;
                    a5=1275, a6=2020,  a7=580, a8=1910;
                    
                    runner = true;
                break;
                case 57:
                    a1=1810, a2=2045,  a3=350, a4=2000;
                    a5=1800, a6=2280,  a7=125, a8=2040;
                    
                    runner = true;
                break;
                case 58:
                    a1=1720, a2=2050,  a3=350, a4=2000;
                    a5=1730, a6=2280,  a7=125, a8=2080;
                    
                    runner = true;
                break;
                case 59:
                    a1=1660, a2=1980,  a3=420, a4=2020;
                    a5=1660, a6=2280,  a7=125, a8=2130;
                    
                    runner = true;
                break;
                case 60:
                    a1=1570, a2=1990,  a3=420, a4=2020;
                    a5=1585, a6=2280,  a7=125, a8=2130;
                    
                    runner = true;
                break;
                case 61:
                    a1=1505, a2=1990,  a3=420, a4=2020;
                    a5=1510, a6=2280,  a7=125, a8=2130;
                    
                    runner = true;
                break;
                case 62:
                    a1=1435, a2=1965,  a3=445, a4=2020;
                    a5=1435, a6=2280,  a7=125, a8=2130;
                    
                    runner = true;
                break;
                case 63:
                    a1=1370, a2=2000,  a3=390, a4=2020;
                    a5=1360, a6=2260,  a7=125, a8=2100;
                    
                    runner = true;
                break;
                case 64:
                    a1=1325, a2=2025,  a3=390, a4=1950;
                    a5=1300, a6=1930,  a7=750, a8=1605;
                    runner = true;
                break;
                
                default:
                break;
            }
            
            if(runner) {
                // Down
                start_timer = millis()/10;
                while (true) {
                    millisTime = millis()/10-start_timer;
                    if (millisTime <= 200) {
                        int32_t value1 = a1 + (c1*millisTime*millisTime) + (d1*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_1, (int32_t)value1);
                        int32_t value2 = a2 + (c2*millisTime*millisTime) + (d2*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_2, (int32_t)value2);
                        int32_t value3 = a3 + (c3*millisTime*millisTime) + (d3*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_3, (int32_t)value3);
                        int32_t value4 = a4 + (c4*millisTime*millisTime) + (d4*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_4, (int32_t)value4);
                    } else {
                        break;
                   }
                }

                // 1. grap 2. releasing
                if (c==0) {
                    gripper = grab;
                } else {
                    gripper = releasing;
                }

                delay(100);
                dxl_wb.goalPosition(DXL_ID_5, (int32_t)gripper);
                delay(100);
                    
                // Up
                start_timer = millis()/10;
                while (true) {
                    millisTime = millis()/10-start_timer;
                    if (millisTime <= 200) {
                        int32_t value1 = a5 - (c1*millisTime*millisTime) - (d1*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_1, (int32_t)value1);
                        int32_t value2 = a6 - (c2*millisTime*millisTime) - (d2*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_2, (int32_t)value2);
                        int32_t value3 = a7 - (c3*millisTime*millisTime) - (d3*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_3, (int32_t)value3);
                        int32_t value4 = a8 - (c4*millisTime*millisTime) - (d4*millisTime*millisTime*millisTime);
                        dxl_wb.goalPosition(DXL_ID_4, (int32_t)value4);
                    } else {
                        break;
                    }
                }
                
                runner = false;
                if (c==1) {
                    present=goal;
                    delay(100);
                    break;
                }
                
                for (int i=0; i<5; i++) {
                    pos_a[i] = upper_path[present-1][i];
                    pos_b[i] = upper_path[goal-1][i];
                }
                // Moving loop
                loop_num = getMax(pos_a, pos_b, NCNT);
                for (int i=0; i<loop_num; i+=10) {
                    for (int cnt=SCNT; cnt < 4; cnt++) {
                        int vel = 0;
                        if (pos_a[cnt] < pos_b[cnt]) {vel=10;}
                        else if (pos_a[cnt] > pos_b[cnt]) {vel=-10;}
                        pos_a[cnt] += vel;
                        dxl_wb.goalPosition(dxl_id[cnt], (int32_t)pos_a[cnt]);
                    }
                }
            }
        }
        
        // reset buffer
        for(int a=0;a<21;a++) {
            buffer[a] = NULL;
        }
        
        bufferIndex = 0;
        goal = 0;
        option=0;
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
