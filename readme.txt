1. About PrivateDataGenerator
"About PrivateDataGenerator",
"              PrivateDataGenerator               \n"
"This software is for generating Private Data that\n"
"follows the COSHIP OTC Standard.\n"
"-------------------------------------------------\n"
"********   Author: QuNengrong (Neo Qu)   ********\n"
"********   Email : Quner612#qq.com       ********\n"
"-------------------------------------------------\n"

2. PrivateDataGenerator Help
"PrivateDataGenerator Help",
"        PrivateDataGenerator User Guide        \n"
"1. Click 'Default' or 'Open' to get basic data;\n"
"2. Edit the data as you wish\n"
"3. Click 'Generate' to get your private data\n"
"4. Copy the private data, or Save it to a file\n"
"-------------------------------------------------\n"
"        PrivateDataGenerator Edit Tips        \n"
"A). Hex data only accept data like this: ##, ##:##,\n"
"   ##:##:##:##, where, # stands for hex numbers;\n"
"B). While edit, you may uncheck the CheckBox in the\n"
"   Same line, to see the Decimal value of the line.\n"

3. Example data output(the second line)
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 // Byte Index
00:00:00:88:00:06:22:50:00:00:1A:DB:00:03:1F:45:00:00:00:00:00:00:00:00:00:00:00:76:00:00:00:63:00:01:00 // private data

4. CopyLeft and CopyRight
A. The Codes come as a part-time self-training, so it follows the LGPL,
   Anyone can read and use on condition that citing the original Author ABOVE,
   and DO NOT DO ANYTHING EVIL;
B. The Original File Format of OTC Standard is Copyrighted by COSHIP;

5. Data format:
(Just show 00, 35bytes grouped to 12 groups);
00 00 00 00 // 1. manufacture id
00 00 00 00 // 2. Frequency
00 00 00 00 // 3. Symbol rate
00 00		// 4. QAM Mode,01->QAM16, 02->QAM32, 03->QAM64, 04->QAM128, 05->QAM256
00 00		// 5. PID
00 00 00 00 // 6. Start Serial No.
00 00 00 00 // 7. End Serial No.
00 00 00 00 // 8. Software Version
00 00 00 00 // 9. Hardware Version
00			// 10. Hardware control Byte, reserved to 00
00			// 11. OTA Mode, 00 -> MENU OTA, 01 -> FORCE OTA
00			// 12.Serial Control Byte, reserved to 00
