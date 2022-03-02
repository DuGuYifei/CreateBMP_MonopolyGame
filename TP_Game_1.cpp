#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <stdint.h>

struct BITMAPCOREHEADER
{
    uint32_t bcSize;
    uint16_t bcWidth;
    uint16_t bcHeight;
    uint16_t bcPlanes;
    uint16_t bcBitCount;
};

struct BITMAPFILEHEADER
{
    uint8_t  bfType[2] = { 0x42,0x4d };
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

BITMAPCOREHEADER coreheader;
BITMAPFILEHEADER fileheader;

using namespace std;

const int Max_k = 2;
const int Max_Bounus_Move = 100;

unsigned char _base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//used to encode by base64
string Encode(unsigned char* str, int bytes) {
    int num = 0, bin = 0, i;
    string _encode_result;
    unsigned char* current;
    current = str;
    while (bytes > 2) {
        _encode_result += _base64_table[current[0] >> 2];
        _encode_result += _base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
        _encode_result += _base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
        _encode_result += _base64_table[current[2] & 0x3f];

        current += 3;
        bytes -= 3;
    }
    if (bytes > 0)
    {
        _encode_result += _base64_table[current[0] >> 2];
        if (bytes % 3 == 1) {
            _encode_result += _base64_table[(current[0] & 0x03) << 4];
            _encode_result += "==";
        }
        else if (bytes % 3 == 2) {
            _encode_result += _base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
            _encode_result += _base64_table[(current[1] & 0x0f) << 2];
            _encode_result += "=";
        }
    }
    return _encode_result;
}

int main()
{
    struct rgb
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    }white, black;

    white.blue = 255;
    white.green = 255;
    white.red = 255;

    black.blue = 0;
    black.green = 0;
    black.red = 0;

    int end = 0;
    unsigned int boardSize;
    std::string command;
    unsigned int player = 0;
    int k = 2;
    int m[Max_k];
    m[0] = 1;
    m[1] = 6;
    int start_m_1 = 0, start_m_2 = 0, start_m_3 = 0, start_m_4 = 0; //record test to which m;
    int start_1 = 0, start_2 = 0, start_3 = 0, start_4 = 0;         //record whether start
    int p1Pos = 0;
    int p2Pos = 0;
    int p3Pos = 0;
    int p4Pos = 0;
    unsigned char States = 0;
    int mineSet = 0;
    unsigned int mineFieldNumber[Max_Bounus_Move];
    unsigned int bonusMove[Max_Bounus_Move];
    bool random = false;
    int seed;
    int active[4] = { 1,1,1,1 };

    do
    {
        cin >> command;
        if (command == "INIT")
        {
            cin >> boardSize;
        }
        else if (command == "INITSEQ")
        {
            cin >> k;
            if (k != 0)
            {
                for (int i = 0; i < k; i++)
                {
                    cin >> m[i];
                }
            }
            if (k == 0)
            {
                start_1 = 1, start_2 = 1, start_3 = 1, start_4 = 1;
            }
            start_m_1 = 0, start_m_2 = 0, start_m_3 = 0, start_m_4 = 0;
        }

        else if (command == "MOVE")
        {
            unsigned int moveValue;
            cin >> moveValue;
            player = (player + 1);
            if (player == 5)
            {
                player = 1;
            }
            while (active[player - 1] == 0)
            {
                player = (player + 1);
                if (player == 5)
                {
                    player = 1;
                }
            }
            switch (player)
            {
            case 1:
                if (k != 0 && start_1 == 0)
                {
                    if (moveValue == m[start_m_1])
                    {
                        start_m_1++;
                    }
                    else
                    {
                        start_m_1 = 0;
                    }
                    if (start_m_1 == k)
                    {
                        start_1 = 1;
                    }
                    break;
                }
                if (start_1 == 1)
                {
                    p1Pos += moveValue;
                    if (mineSet)
                    {
                        bool bomb = true;
                        int beginPosition = p1Pos;
                        while (bomb && active[0])
                        {
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p1Pos == mineFieldNumber[i])
                                {
                                    p1Pos += bonusMove[i];
                                    if (p1Pos == beginPosition)
                                    {
                                        active[0] = 0;
                                        cout << "P1 was defeated by mines\n";
                                        
                                    }
                                    bomb = false;
                                    break;
                                }
                            }
                            bomb = false;
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p1Pos == mineFieldNumber[i])
                                {
                                    bomb = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            case 2:
                if (k != 0 && start_2 == 0)
                {
                    if (moveValue == m[start_m_2])
                    {
                        start_m_2++;
                    }
                    else
                    {
                        start_m_2 = 0;
                    }
                    if (start_m_2 == k)
                    {
                        start_2 = 1;
                    }
                    break;
                }
                if (start_2 == 1)
                {
                    p2Pos += moveValue;
                    if (mineSet)
                    {
                        bool bomb = true;
                        int beginPosition = p2Pos;
                        while (bomb && active[1])
                        {
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p2Pos == mineFieldNumber[i])
                                {
                                    p2Pos += bonusMove[i];
                                    if (p2Pos == beginPosition)
                                    {
                                        active[1] = 0;
                                        cout << "P2 was defeated by mines\n";
                                    }
                                    bomb = false;
                                    break;
                                }
                            }
                            bomb = false;
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p2Pos == mineFieldNumber[i])
                                {
                                    bomb = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            case 3:
                if (k != 0 && start_3 == 0)
                {
                    if (moveValue == m[start_m_3])
                    {
                        start_m_3++;
                    }
                    else
                    {
                        start_m_3 = 0;
                    }
                    if (start_m_3 == k)
                    {
                        start_3 = 1;
                    }
                    break;
                }
                if (start_3 == 1)
                {
                    p3Pos += moveValue;
                    if (mineSet)
                    {
                        bool bomb = true;
                        int beginPosition = p3Pos;
                        while (bomb && active[2])
                        {
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p3Pos == mineFieldNumber[i])
                                {
                                    p3Pos += bonusMove[i];
                                    if (p3Pos == beginPosition)
                                    {
                                        active[2] = 0;
                                        cout << "P3 was defeated by mines\n";
                                    }
                                    bomb = false;
                                    break;
                                }
                            }
                            bomb = false;
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p3Pos == mineFieldNumber[i])
                                {
                                    bomb = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            case 4:
                if (k != 0 && start_4 == 0)
                {
                    if (moveValue == m[start_m_4])
                    {
                        start_m_4++;
                    }
                    else
                    {
                        start_m_4 = 0;
                    }
                    if (start_m_4 == k)
                    {
                        start_4 = 1;
                    }
                    break;
                }
                if (start_4 == 1)
                {
                    p4Pos += moveValue;
                    if (mineSet)
                    {
                        bool bomb = true;
                        int beginPosition = p4Pos;
                        while (bomb && active[3])
                        {
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p4Pos == mineFieldNumber[i])
                                {
                                    p4Pos += bonusMove[i];
                                    if (p4Pos == beginPosition)
                                    {
                                        active[3] = 0;
                                        cout << "P4 was defeated by mines\n";
                                    }
                                    bomb = false;
                                    break;
                                }
                            }
                            bomb = false;
                            for (int i = 0; i < mineSet; i++)
                            {
                                if (p4Pos == mineFieldNumber[i])
                                {
                                    bomb = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            default:
                cerr << "Wrong player's turn" << endl;
                break;
            }
        }
        else if (command == "ABORT")
        {
            cout << "ABORTED" << endl;
            end = 1; 
        }
        else if (command == "ABORT_S")
        {
            end = 1;
        }
        else if (command == "PRINT")
        {
            // FOR 0 should print only positions. For 1 positions and diodes
            char arg;
            cin >> arg;

            States = 0;
            States = States | (0b10000000 * start_1) | (0b00100000 * start_2) | (0b00001000 * start_3) | (0b00000010 * start_4);
            States = States | (0b01000000 * (p1Pos % 2)) | (0b00010000 * (p2Pos % 2)) | (0b00000100 * (p3Pos % 2)) | (0b00000001 * (p4Pos % 2));

            string p1 = to_string(p1Pos);
            string p2 = to_string(p2Pos);
            string p3 = to_string(p3Pos);
            string p4 = to_string(p4Pos);
            if (active[0] == 0)
            {
                p1 = "X";
            }
            if (active[1] == 0)
            {
                p2 = "X";
            }
            if (active[2] == 0)
            {
                p3 = "X";
            }
            if (active[3] == 0)
            {
                p4 = "X";
            }

            if (arg == '0')
            {
                cout << p1 << " " << p2 << " " << p3 << " " << p4 << " ";
            }
            else if (arg == '1')
            {
                cout << p1 << " " << p2 << " " << p3 << " " << p4 << " ";
                for (unsigned int i = 0; i < 8; i++)
                {
                    if (States & (1 << (7 - i)))
                    {
                        cout << 1;
                    }
                    else
                    {
                        cout << 0;
                    }
                }
            }
            else if (arg == '2' || arg == '3')
            {
                coreheader.bcSize = 12;
                coreheader.bcWidth = 4 * 5;
                coreheader.bcHeight = (boardSize + 1) * 5;
                coreheader.bcPlanes = 1;
                coreheader.bcBitCount = 24;           

                fileheader.bfOffBits = 26;
                fileheader.bfSize = coreheader.bcHeight * coreheader.bcWidth * 3 + fileheader.bfOffBits;
                fileheader.bfReserved1 = 0;
                fileheader.bfReserved2 = 0;

                int Number_Pixels = coreheader.bcHeight * coreheader.bcWidth;
                int pixels[(Max_Bounus_Move + 1) * 5 * 4 * 5];          //0 white,1 black

                for (int i = 0; i < Number_Pixels; i++)
                {
                    pixels[i] = 0; //background is white
                }

                //Player 1
                pixels[(boardSize - p1Pos) * 20 * 5 + 20 * 4 + 5 - 1] = 1;
                pixels[(boardSize - p1Pos) * 20 * 5 + 20 * 3 + 4 - 1] = 1;
                pixels[(boardSize - p1Pos) * 20 * 5 + 20 * 3 + 5 - 1] = 1;
                pixels[(boardSize - p1Pos) * 20 * 5 + 20 * 2 + 3 - 1] = 1;
                pixels[(boardSize - p1Pos) * 20 * 5 + 20 * 2 + 5 - 1] = 1;
                pixels[(boardSize - p1Pos) * 20 * 5 + 20 * 1 + 5 - 1] = 1;
                pixels[(boardSize - p1Pos) * 20 * 5 + 5 - 1] = 1;

                //Player 2
                pixels[(boardSize - p2Pos) * 20 * 5 + 20 * 4 + 8 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 20 * 4 + 9 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 20 * 3 + 7 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 20 * 3 + 10 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 20 * 2 + 9 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 20 * 1 + 8 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 7 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 8 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 9 - 1] = 1;
                pixels[(boardSize - p2Pos) * 20 * 5 + 10 - 1] = 1;

                //player 3
                pixels[(boardSize - p3Pos) * 20 * 5 + 20 * 4 + 13 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 20 * 4 + 14 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 20 * 3 + 12 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 20 * 3 + 15 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 20 * 2 + 14 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 20 * 1 + 12 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 20 * 1 + 15 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 13 - 1] = 1;
                pixels[(boardSize - p3Pos) * 20 * 5 + 14 - 1] = 1;

                //player 4
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 4 + 18 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 4 + 19 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 3 + 17 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 3 + 19 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 2 + 16 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 2 + 17 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 2 + 18 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 2 + 19 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 2 + 20 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 20 * 1 + 19 - 1] = 1;
                pixels[(boardSize - p4Pos) * 20 * 5 + 19 - 1] = 1;

                if (arg == '2')
                {
                    FILE* fp = fopen("1.bmp", "wb");
                    struct Fileheader2
                    {
                        uint8_t  bfType[2] = { 0x42,0x4d };
                        uint16_t bfSize[2];
                        uint16_t bfReserved1;
                        uint16_t bfReserved2;
                        uint16_t bfOffBits[2];
                    };
                    Fileheader2 fileheader2;
                    memcpy(&fileheader2.bfOffBits[0], &fileheader.bfOffBits, sizeof(uint32_t));
                    fileheader2.bfReserved1 = fileheader.bfReserved1;
                    fileheader2.bfReserved2 = fileheader.bfReserved2;
                    fileheader2.bfSize[0] = 0;
                    fileheader2.bfSize[1] = 0;
                    memcpy(&fileheader2.bfSize[0], &fileheader.bfSize, sizeof(uint32_t));
                    fwrite(&fileheader2, sizeof(BITMAPFILEHEADER)-2 , 1, fp);
                    fwrite(&coreheader, sizeof(BITMAPCOREHEADER), 1, fp);
                    for (int i = 0; i < Number_Pixels; i++)
                    {
                        if (pixels[i] == 1)
                        {
                            fwrite(&black, sizeof(rgb), 1, fp);
                        }
                        if (pixels[i] == 0)
                        {
                            fwrite(&white, sizeof(rgb), 1, fp);
                        }
                    }
                    fclose(fp);
                }

                else if (arg == '3')
                {
                    unsigned char BM[(Max_Bounus_Move + 1) * 5 * 4 * 5 * 3 + 26];

                    //fileheader                   
                    memcpy(&BM[0], &fileheader.bfType, 2 * sizeof(uint8_t));                    //type BM
                    memcpy(&BM[2], &fileheader.bfSize, sizeof(uint32_t));                       //file size
                    memcpy(&BM[6], &fileheader.bfReserved1, sizeof(uint16_t));                  //reserved 1
                    memcpy(&BM[8], &fileheader.bfReserved1, sizeof(uint16_t));                  //reserved 2
                    memcpy(&BM[10], &fileheader.bfOffBits, sizeof(uint32_t));                   //OffBits

                    //coreheader
                    memcpy(&BM[14], &coreheader.bcSize, sizeof(uint32_t));                      //bcsize
                    memcpy(&BM[18], &coreheader.bcWidth, sizeof(uint16_t));                     //width
                    memcpy(&BM[20], &coreheader.bcHeight, sizeof(uint16_t));                    //height
                    memcpy(&BM[22], &coreheader.bcPlanes, sizeof(uint16_t));                    //planes
                    memcpy(&BM[24], &coreheader.bcBitCount, sizeof(uint16_t));                  //bitcount 24

                    for (int i = 26; i < fileheader.bfSize; i++)
                    {
                        for (int j = 0; j < Number_Pixels; j++)
                        {
                            if (pixels[j] == 0)
                            {
                                BM[i++] = white.red;
                                BM[i++] = white.green;
                                BM[i++] = white.blue;
                            }
                            if (pixels[j] == 1)
                            {
                                BM[i++] = black.red;
                                BM[i++] = black.green;
                                BM[i++] = black.blue;
                            }
                        }
                    }

                    string BASE64 = Encode(BM, fileheader.bfSize);
                    cout << BASE64;
                }
            }
            cout << endl;
        }
        else if (command == "ENBLRAND")
        {
            cin >> seed;
            srand(seed);
            random = true;
        }
        else if (command == "MINED")
        {
            cin >> mineFieldNumber[mineSet];
            cin >> bonusMove[mineSet];
            mineSet += 1;
            //set the argument as mined field if a player stand on it, then she goes + 1
        }
        else if (command == "LASSO")
        {
            player = (player + 1);
            if (player == 5)
            {
                player = 1;
            }
            while (active[player - 1] == 0)
            {
                player = (player + 1);
                if (player == 5)
                {
                    player = 1;
                }
            }
            string user;
            string target;
            int* userPos;
            int* targetPos;
            if (player == 1)
            {
                userPos = &p1Pos;
            }
            else if (player == 2)
            {
                userPos = &p2Pos;
            }
            else if (player == 3)
            {
                userPos = &p3Pos;
            }
            else
            {
                userPos = &p4Pos;
            }
            cin >> target;
            if (target == "P1")
            {
                targetPos = &p1Pos;
            }
            else if (target == "P2")
            {
                targetPos = &p2Pos;
            }
            else if (target == "P3")
            {
                targetPos = &p3Pos;
            }
            else 
            {
                targetPos = &p4Pos;
            }
            if (*userPos - *targetPos <= 1 && *userPos - *targetPos >= -1)
            {
                break;
            }
            else
            {
                if (random == true)
                {
                    if (rand() % abs(*userPos - *targetPos) == 1)
                    {
                        if (*userPos - *targetPos > 0)
                        {
                            (*userPos)--;
                            (*targetPos)++;
                        }
                        else if (*userPos - *targetPos < 0)
                        {
                            (*userPos)++;
                            (*targetPos)--;
                        }
                    }
                }
                else
                {
                    if (*userPos - *targetPos > 0)
                    {
                        (*userPos)--;
                        (*targetPos)++;
                    }
                    else if (*userPos - *targetPos < 0)
                    {
                        (*userPos)++;
                        (*targetPos)--;
                    }
                }
            }
            switch (player)
            {
            case 1:
               
                if (mineSet)
                {
                    bool bomb = true;
                    int beginPosition = p1Pos;
                    while (bomb && active[0])
                    {
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p1Pos == mineFieldNumber[i])
                            {
                                p1Pos += bonusMove[i];
                                if (p1Pos == beginPosition)
                                {
                                    active[0] = 0;
                                    cout << "P1 was defeated by mines\n";
                                }
                                bomb = false;
                                break;
                            }
                        }
                        bomb = false;
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p1Pos == mineFieldNumber[i])
                            {
                                bomb = true;
                                break;
                            }
                        }
                    }
                }
                
                break;
            case 2:
                
                if (mineSet)
                {
                    bool bomb = true;
                    int beginPosition = p2Pos;
                    while (bomb && active[1])
                    {
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p2Pos == mineFieldNumber[i])
                            {
                                p2Pos += bonusMove[i];
                                if (p2Pos == beginPosition)
                                {
                                    active[1] = 0;
                                    cout << "P2 was defeated by mines\n";
                                }
                                bomb = false;
                                break;
                            }
                        }
                        bomb = false;
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p2Pos == mineFieldNumber[i])
                            {
                                bomb = true;
                                break;
                            }
                        }
                    }
                }
                
                break;
            case 3:
                
                if (mineSet)
                {
                    bool bomb = true;
                    int beginPosition = p3Pos;
                    while (bomb && active[2])
                    {
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p3Pos == mineFieldNumber[i])
                            {
                                p3Pos += bonusMove[i];
                                if (p3Pos == beginPosition)
                                {
                                    active[2] = 0;
                                    cout << "P3 was defeated by mines\n";
                                }
                                bomb = false;
                                break;
                            }
                        }
                        bomb = false;
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p3Pos == mineFieldNumber[i])
                            {
                                bomb = true;
                                break;
                            }
                        }
                    }
                }
                
                break;
            case 4:
                
                if (mineSet)
                {
                    bool bomb = true;
                    int beginPosition = p4Pos;
                    while (bomb && active[3])
                    {
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p4Pos == mineFieldNumber[i])
                            {
                                p4Pos += bonusMove[i];
                                if (p4Pos == beginPosition)
                                {
                                    active[3] = 0;
                                    cout << "P4 was defeated by mines\n";
                                }
                                bomb = false;
                                break;
                            }
                        }
                        bomb = false;
                        for (int i = 0; i < mineSet; i++)
                        {
                            if (p4Pos == mineFieldNumber[i])
                            {
                                bomb = true;
                                break;
                            }
                        }
                    }
                }
                
                break;
            }
        }

        else
        {
            cerr << "BAD INPUT" << endl;
            end = 1;
        }

        if (p1Pos >= boardSize)
        {
            cout << "P1 won";
            end = 1;
        }
        if (p2Pos >= boardSize)
        {
            cout << "P2 won";
            end = 1;
        }
        if (p3Pos >= boardSize)
        {
            cout << "P3 won";
            end = 1;
        }
        if (p4Pos >= boardSize)
        {
            cout << "P4 won";
            end = 1;
        }
        if (!active[0] && !active[1] && !active[2] && !active[3])
        {
            cout << "DRAW";
            end = 1;
        }
    } while (!end);

    return 0;
}