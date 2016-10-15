
/* 
All arrays each contain a sampled sinus signal over one period where each array represents a note, array size = number of samples for each array. 
Frequency for each note commented below 
*/

//Frequency 131
int c3[] =
    { 336, 127, 129, 131, 134, 136, 138, 141, 143, 145, 148, 150, 152, 155, 157,
	159, 162, 164, 166, 168, 171, 173, 175, 177, 179, 182, 184, 186, 188,
	190,
	192, 194, 196, 198, 200, 202, 204, 206, 208, 209, 211, 213, 215, 216,
	218,
	220, 221, 223, 224, 226, 227, 229, 230, 231, 233, 234, 235, 236, 238,
	239,
	240, 241, 242, 243, 244, 245, 246, 246, 247, 248, 249, 249, 250, 250,
	251,
	251, 252, 252, 252, 253, 253, 253, 253, 253, 253, 254, 253, 253, 253,
	253,
	253, 253, 252, 252, 252, 251, 251, 250, 250, 249, 249, 248, 247, 246,
	246,
	245, 244, 243, 242, 241, 240, 239, 238, 236, 235, 234, 233, 231, 230,
	229,
	227, 226, 224, 223, 221, 220, 218, 216, 215, 213, 211, 209, 208, 206,
	204,
	202, 200, 198, 196, 194, 192, 190, 188, 186, 184, 182, 179, 177, 175,
	173,
	171, 168, 166, 164, 162, 159, 157, 155, 152, 150, 148, 145, 143, 141,
	138,
	136, 134, 131, 129, 127, 124, 122, 119, 117, 115, 112, 110, 108, 105,
	103,
	101, 98, 96, 94, 91, 89, 87, 85, 82, 80, 78, 76, 74, 71, 69, 67, 65, 63,
	61, 59, 57, 55, 53, 51, 49, 47, 45, 44, 42, 40, 38, 37, 35, 33, 32, 30,
	29,
	27, 26, 24, 23, 22, 20, 19, 18, 17, 15, 14, 13, 12, 11, 10, 9, 8, 7, 7,
	6,
	5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1,
	1, 2, 2, 3, 3, 4, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18,
	19,
	20, 22, 23, 24, 26, 27, 29, 30, 32, 33, 35, 37, 38, 40, 42, 44, 45, 47,
	49,
	51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 74, 76, 78, 80, 82, 85, 87,
	89,
	91, 94, 96, 98, 101, 103, 105, 108, 110, 112, 115, 117, 119, 122, 124
};

//Frequency 147
int d3[] =
    { 300, 127, 129, 132, 134, 137, 140, 142, 145, 148, 150, 153, 156, 158, 161,
	163, 166, 168, 171, 173, 176, 178, 181, 183, 185, 188, 190, 192, 195,
	197,
	199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223, 224,
	226,
	228, 229, 231, 232, 234, 235, 236, 238, 239, 240, 241, 243, 244, 245,
	246,
	246, 247, 248, 249, 250, 250, 251, 251, 252, 252, 252, 253, 253, 253,
	253,
	253, 254, 253, 253, 253, 253, 253, 252, 252, 252, 251, 251, 250, 250,
	249,
	248, 247, 246, 246, 245, 244, 243, 241, 240, 239, 238, 236, 235, 234,
	232,
	231, 229, 228, 226, 224, 223, 221, 219, 217, 215, 213, 211, 209, 207,
	205,
	203, 201, 199, 197, 195, 192, 190, 188, 185, 183, 181, 178, 176, 173,
	171,
	168, 166, 163, 161, 158, 156, 153, 150, 148, 145, 142, 140, 137, 134,
	132,
	129, 127, 124, 121, 119, 116, 113, 111, 108, 105, 103, 100, 97, 95, 92,
	90,
	87, 85, 82, 80, 77, 75, 72, 70, 68, 65, 63, 61, 58, 56, 54, 52, 50, 48,
	46,
	44, 42, 40, 38, 36, 34, 32, 30, 29, 27, 25, 24, 22, 21, 19, 18, 17, 15,
	14,
	13, 12, 10, 9, 8, 7, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0,
	0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 7, 8, 9, 10, 12, 13, 14,
	15,
	17, 18, 19, 21, 22, 24, 25, 27, 29, 30, 32, 34, 36, 38, 40, 42, 44, 46,
	48,
	50, 52, 54, 56, 58, 61, 63, 65, 68, 70, 72, 75, 77, 80, 82, 85, 87, 90,
	92,
	95, 97, 100, 103, 105, 108, 111, 113, 116, 119, 121, 124
};

//Frequency 165
int e3[] =
    { 267, 127, 129, 132, 135, 138, 141, 144, 147, 150, 153, 156, 159, 162, 165,
	168, 170, 173, 176, 179, 181, 184, 187, 189, 192, 194, 197, 199, 202,
	204,
	207, 209, 211, 213, 216, 218, 220, 222, 224, 226, 227, 229, 231, 233,
	234,
	236, 237, 239, 240, 241, 243, 244, 245, 246, 247, 248, 249, 249, 250,
	251,
	251, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
	252,
	252, 251, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 239,
	238,
	236, 235, 233, 232, 230, 228, 226, 225, 223, 221, 219, 217, 214, 212,
	210,
	208, 205, 203, 201, 198, 196, 193, 191, 188, 185, 183, 180, 177, 175,
	172,
	169, 166, 163, 160, 158, 155, 152, 149, 146, 143, 140, 137, 134, 131,
	128,
	125, 122, 119, 116, 113, 110, 107, 104, 101, 98, 95, 93, 90, 87, 84, 81,
	78, 76, 73, 70, 68, 65, 62, 60, 57, 55, 52, 50, 48, 45, 43, 41, 39, 36,
	34,
	32, 30, 28, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11, 10, 9, 8, 7, 6,
	5,
	4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 4,
	5,
	6, 7, 8, 9, 10, 12, 13, 14, 16, 17, 19, 20, 22, 24, 26, 27, 29, 31, 33,
	35,
	37, 40, 42, 44, 46, 49, 51, 54, 56, 59, 61, 64, 66, 69, 72, 74, 77, 80,
	83,
	85, 88, 91, 94, 97, 100, 103, 106, 109, 112, 115, 118, 121, 124
};

//Frequency 175
int f3[] =
    { 252, 127, 130, 133, 136, 139, 142, 145, 149, 152, 155, 158, 161, 164, 167,
	170, 173, 176, 179, 182, 184, 187, 190, 193, 195, 198, 201, 203, 206,
	208,
	211, 213, 215, 217, 220, 222, 224, 226, 228, 230, 231, 233, 235, 236,
	238,
	240, 241, 242, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 252,
	253,
	253, 253, 253, 253, 254, 253, 253, 253, 253, 253, 252, 252, 251, 250,
	250,
	249, 248, 247, 246, 245, 244, 242, 241, 240, 238, 236, 235, 233, 231,
	230,
	228, 226, 224, 222, 220, 217, 215, 213, 211, 208, 206, 203, 201, 198,
	195,
	193, 190, 187, 184, 182, 179, 176, 173, 170, 167, 164, 161, 158, 155,
	152,
	149, 145, 142, 139, 136, 133, 130, 127, 123, 120, 117, 114, 111, 108,
	104,
	101, 98, 95, 92, 89, 86, 83, 80, 77, 74, 71, 69, 66, 63, 60, 58, 55, 52,
	50, 47, 45, 42, 40, 38, 36, 33, 31, 29, 27, 25, 23, 22, 20, 18, 17, 15,
	13,
	12, 11, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,
	1, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 15, 17, 18, 20, 22, 23, 25,
	27, 29, 31, 33, 36, 38, 40, 42, 45, 47, 50, 52, 55, 58, 60, 63, 66, 69,
	71,
	74, 77, 80, 83, 86, 89, 92, 95, 98, 101, 104, 108, 111, 114, 117, 120,
	123
};

//Frequency 196
int g3[] =
    { 225, 127, 130, 134, 137, 141, 144, 148, 151, 155, 158, 162, 165, 168, 172,
	175, 178, 181, 185, 188, 191, 194, 197, 200, 203, 205, 208, 211, 213,
	216,
	218, 221, 223, 225, 228, 230, 232, 234, 236, 237, 239, 241, 242, 244,
	245,
	246, 247, 248, 249, 250, 251, 252, 252, 253, 253, 253, 253, 253, 253,
	253,
	253, 253, 252, 252, 251, 251, 250, 249, 248, 247, 246, 244, 243, 241,
	240,
	238, 236, 235, 233, 231, 229, 227, 224, 222, 220, 217, 215, 212, 209,
	207,
	204, 201, 198, 195, 192, 189, 186, 183, 180, 177, 173, 170, 167, 163,
	160,
	156, 153, 149, 146, 142, 139, 135, 132, 128, 125, 121, 118, 114, 111,
	107,
	104, 100, 97, 93, 90, 86, 83, 80, 76, 73, 70, 67, 64, 61, 58, 55, 52,
	49,
	46, 44, 41, 38, 36, 33, 31, 29, 26, 24, 22, 20, 18, 17, 15, 13, 12, 10,
	9,
	7, 6, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5,
	6,
	7, 8, 9, 11, 12, 14, 16, 17, 19, 21, 23, 25, 28, 30, 32, 35, 37, 40, 42,
	45, 48, 50, 53, 56, 59, 62, 65, 68, 72, 75, 78, 81, 85, 88, 91, 95, 98,
	102, 105, 109, 112, 116, 119, 123
};

//Frequency 220
int a3[] =
    { 200, 127, 130, 134, 138, 142, 146, 150, 154, 158, 162, 166, 170, 173, 177,
	181, 184, 188, 191, 195, 198, 201, 204, 207, 210, 213, 216, 219, 222,
	224,
	227, 229, 232, 234, 236, 238, 240, 241, 243, 245, 246, 247, 248, 250,
	250,
	251, 252, 252, 253, 253, 253, 254, 253, 253, 253, 252, 252, 251, 250,
	250,
	248, 247, 246, 245, 243, 241, 240, 238, 236, 234, 232, 229, 227, 224,
	222,
	219, 216, 213, 210, 207, 204, 201, 198, 195, 191, 188, 184, 181, 177,
	173,
	170, 166, 162, 158, 154, 150, 146, 142, 138, 134, 130, 127, 123, 119,
	115,
	111, 107, 103, 99, 95, 91, 87, 83, 80, 76, 72, 69, 65, 62, 58, 55, 52,
	49,
	46, 43, 40, 37, 34, 31, 29, 26, 24, 21, 19, 17, 15, 13, 12, 10, 8, 7, 6,
	5,
	3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 5, 6, 7, 8, 10, 12,
	13,
	15, 17, 19, 21, 24, 26, 29, 31, 34, 37, 40, 43, 46, 49, 52, 55, 58, 62,
	65,
	69, 72, 76, 80, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123
};

//Frequency 247
int b3[] =
    { 178, 127, 131, 135, 140, 144, 149, 153, 158, 162, 166, 170, 175, 179, 183,
	187, 191, 194, 198, 202, 205, 209, 212, 216, 219, 222, 225, 227, 230,
	233,
	235, 237, 239, 241, 243, 245, 246, 248, 249, 250, 251, 252, 253, 253,
	253,
	253, 253, 253, 253, 253, 252, 251, 250, 249, 248, 246, 245, 243, 241,
	239,
	237, 235, 233, 230, 227, 225, 222, 219, 216, 212, 209, 205, 202, 198,
	194,
	191, 187, 183, 179, 175, 170, 166, 162, 158, 153, 149, 144, 140, 135,
	131,
	127, 122, 118, 113, 109, 104, 100, 95, 91, 87, 83, 78, 74, 70, 66, 62,
	59,
	55, 51, 48, 44, 41, 37, 34, 31, 28, 26, 23, 20, 18, 16, 14, 12, 10, 8,
	7,
	5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 7, 8, 10, 12, 14,
	16,
	18, 20, 23, 26, 28, 31, 34, 37, 41, 44, 48, 51, 55, 59, 62, 66, 70, 74,
	78,
	83, 87, 91, 95, 100, 104, 109, 113, 118, 122
};

//Frequency 262
int c4[] =
    { 168, 127, 131, 136, 141, 145, 150, 155, 159, 164, 168, 173, 177, 182, 186,
	190, 194, 198, 202, 206, 209, 213, 216, 220, 223, 226, 229, 231, 234,
	236,
	239, 241, 243, 245, 246, 248, 249, 250, 251, 252, 253, 253, 253, 254,
	253,
	253, 253, 252, 251, 250, 249, 248, 246, 245, 243, 241, 239, 236, 234,
	231,
	229, 226, 223, 220, 216, 213, 209, 206, 202, 198, 194, 190, 186, 182,
	177,
	173, 168, 164, 159, 155, 150, 145, 141, 136, 131, 127, 122, 117, 112,
	108,
	103, 98, 94, 89, 85, 80, 76, 71, 67, 63, 59, 55, 51, 47, 44, 40, 37, 33,
	30, 27, 24, 22, 19, 17, 14, 12, 10, 8, 7, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0,
	0,
	0, 1, 2, 3, 4, 5, 7, 8, 10, 12, 14, 17, 19, 22, 24, 27, 30, 33, 37, 40,
	44,
	47, 51, 55, 59, 63, 67, 71, 76, 80, 85, 89, 94, 98, 103, 108, 112, 117,
	122
};

//Frequency 294
int d4[] =
    { 150, 127, 132, 137, 142, 148, 153, 158, 163, 168, 173, 178, 183, 188, 192,
	197, 201, 205, 209, 213, 217, 221, 224, 228, 231, 234, 236, 239, 241,
	244,
	246, 247, 249, 250, 251, 252, 253, 253, 253, 253, 253, 253, 252, 251,
	250,
	249, 247, 246, 244, 241, 239, 236, 234, 231, 228, 224, 221, 217, 213,
	209,
	205, 201, 197, 192, 188, 183, 178, 173, 168, 163, 158, 153, 148, 142,
	137,
	132, 127, 121, 116, 111, 105, 100, 95, 90, 85, 80, 75, 70, 65, 61, 56,
	52,
	48, 44, 40, 36, 32, 29, 25, 22, 19, 17, 14, 12, 9, 7, 6, 4, 3, 2, 1, 0,
	0,
	0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 9, 12, 14, 17, 19, 22, 25, 29, 32, 36, 40,
	44, 48, 52, 56, 61, 65, 70, 75, 80, 85, 90, 95, 100, 105, 111, 116, 121
};

//Frequency 329
int e4[] =
    { 134, 127, 132, 138, 144, 150, 156, 162, 167, 173, 179, 184, 189, 194, 199,
	204, 209, 213, 217, 221, 225, 229, 232, 235, 238, 241, 244, 246, 248,
	249,
	251, 252, 253, 253, 253, 253, 253, 253, 252, 251, 249, 248, 246, 244,
	241,
	238, 235, 232, 229, 225, 221, 217, 213, 209, 204, 199, 194, 189, 184,
	179,
	173, 167, 162, 156, 150, 144, 138, 132, 127, 121, 115, 109, 103, 97, 91,
	86, 80, 74, 69, 64, 59, 54, 49, 44, 40, 36, 32, 28, 24, 21, 18, 15, 12,
	9,
	7, 5, 4, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5, 7, 9, 12, 15, 18, 21, 24,
	28,
	32, 36, 40, 44, 49, 54, 59, 64, 69, 74, 80, 86, 91, 97, 103, 109, 115,
	121
};

//Frequency 349
int f4[] =
    { 126, 127, 133, 139, 145, 152, 158, 164, 170, 176, 182, 187, 193, 198, 203,
	208, 213, 217, 222, 226, 230, 233, 236, 240, 242, 245, 247, 249, 250,
	252,
	253, 253, 253, 253, 253, 253, 252, 250, 249, 247, 245, 242, 240, 236,
	233,
	230, 226, 222, 217, 213, 208, 203, 198, 193, 187, 182, 176, 170, 164,
	158,
	152, 145, 139, 133, 127, 120, 114, 108, 101, 95, 89, 83, 77, 71, 66, 60,
	55, 50, 45, 40, 36, 31, 27, 23, 20, 17, 13, 11, 8, 6, 4, 3, 1, 0, 0, 0,
	0,
	0, 0, 1, 3, 4, 6, 8, 11, 13, 17, 20, 23, 27, 31, 36, 40, 45, 50, 55, 60,
	66, 71, 77, 83, 89, 95, 101, 108, 114, 120
};

//Frequency 392
int g4[] =
    { 112, 127, 134, 141, 148, 155, 162, 168, 175, 182, 188, 194, 200, 206, 211,
	216, 221, 226, 230, 234, 238, 241, 244, 246, 249, 250, 252, 253, 253,
	254,
	253, 253, 252, 250, 249, 246, 244, 241, 238, 234, 230, 226, 221, 216,
	211,
	206, 200, 194, 188, 182, 175, 168, 162, 155, 148, 141, 134, 127, 119,
	112,
	105, 98, 91, 85, 78, 71, 65, 59, 53, 47, 42, 37, 32, 27, 23, 19, 15, 12,
	9,
	7, 4, 3, 1, 0, 0, 0, 0, 0, 1, 3, 4, 7, 9, 12, 15, 19, 23, 27, 32, 37,
	42,
	47, 53, 59, 65, 71, 78, 85, 91, 98, 105, 112, 119
};

//Frequency 441
int a4[] =
    { 100, 127, 134, 142, 150, 158, 166, 173, 181, 188, 195, 201, 207, 213, 219,
	224, 229, 234, 238, 241, 245, 247, 250, 251, 252, 253, 254, 253, 252,
	251,
	250, 247, 245, 241, 238, 234, 229, 224, 219, 213, 207, 201, 195, 188,
	181,
	173, 166, 158, 150, 142, 134, 127, 119, 111, 103, 95, 87, 80, 72, 65,
	58,
	52, 46, 40, 34, 29, 24, 19, 15, 12, 8, 6, 3, 2, 1, 0, 0, 0, 1, 2, 3, 6,
	8,
	12, 15, 19, 24, 29, 34, 40, 46, 52, 58, 65, 72, 80, 87, 95, 103, 111,
	119
};

//Frequency 493
int b4[] =
    { 89, 127, 135, 144, 153, 162, 170, 179, 187, 194, 202, 209, 216, 222, 227,
	233, 237, 241, 245, 248, 250, 252, 253, 253, 253, 253, 251, 249, 246,
	243,
	239, 235, 230, 225, 219, 212, 205, 198, 191, 183, 175, 166, 158, 149,
	140,
	131, 122, 113, 104, 95, 87, 78, 70, 62, 55, 48, 41, 34, 28, 23, 18, 14,
	10,
	7, 4, 2, 0, 0, 0, 0, 1, 3, 5, 8, 12, 16, 20, 26, 31, 37, 44, 51, 59, 66,
	74, 83, 91, 100, 109, 118
};

//Frequency 523
int c5[] =
    { 84, 127, 136, 145, 155, 164, 173, 182, 190, 198, 206, 213, 220, 226, 231,
	236, 241, 245, 248, 250, 252, 253, 254, 253, 252, 250, 248, 245, 241,
	236,
	231, 226, 220, 213, 206, 198, 190, 182, 173, 164, 155, 145, 136, 127,
	117,
	108, 98, 89, 80, 71, 63, 55, 47, 40, 33, 27, 22, 17, 12, 8, 5, 3, 1, 0,
	0,
	0, 1, 3, 5, 8, 12, 17, 22, 27, 33, 40, 47, 55, 63, 71, 80, 89, 98, 108,
	117
};

//Frequency 587
int d5[] =
    { 75, 127, 137, 148, 158, 168, 178, 188, 197, 205, 213, 221, 228, 234, 239,
	244, 247, 250, 252, 253, 253, 253, 251, 249, 246, 241, 236, 231, 224,
	217,
	209, 201, 192, 183, 173, 163, 153, 142, 132, 121, 111, 100, 90, 80, 70,
	61,
	52, 44, 36, 29, 22, 17, 12, 7, 4, 2, 0, 0, 0, 1, 3, 6, 9, 14, 19, 25,
	32,
	40, 48, 56, 65, 75, 85, 95, 105, 116
};

//Frequency 659
int e5[] =
    { 66, 127, 139, 151, 162, 174, 185, 195, 205, 214, 222, 230, 236, 242, 247,
	250, 252, 253, 253, 252, 250, 247, 242, 236, 230, 222, 214, 205, 195,
	185,
	174, 162, 151, 139, 127, 114, 102, 91, 79, 68, 58, 48, 39, 31, 23, 17,
	11,
	6, 3, 1, 0, 0, 1, 3, 6, 11, 17, 23, 31, 39, 48, 58, 68, 79, 91, 102, 114
};

//Frequency 698
int f5[] =
    { 63, 127, 139, 152, 164, 176, 187, 198, 208, 217, 226, 233, 240, 245, 249,
	252, 253, 253, 253, 250, 247, 242, 236, 230, 222, 213, 203, 193, 182,
	170,
	158, 145, 133, 120, 108, 95, 83, 71, 60, 50, 40, 31, 23, 17, 11, 6, 3,
	0,
	0, 0, 1, 4, 8, 13, 20, 27, 36, 45, 55, 66, 77, 89, 101, 114
};

//Frequency 784
int g5[] =
    { 56, 127, 141, 155, 168, 182, 194, 206, 216, 226, 234, 241, 246, 250, 253,
	254, 253, 250, 246, 241, 234, 226, 216, 206, 194, 182, 168, 155, 141,
	127,
	112, 98, 85, 71, 59, 47, 37, 27, 19, 12, 7, 3, 0, 0, 0, 3, 7, 12, 19,
	27,
	37, 47, 59, 71, 85, 98, 112
};

//Frequency 880
int a5[] =
    { 50, 127, 142, 158, 173, 188, 201, 213, 224, 234, 241, 247, 251, 253, 253,
	251, 247, 241, 234, 224, 213, 201, 188, 173, 158, 142, 127, 111, 95, 80,
	65, 52, 40, 29, 19, 12, 6, 2, 0, 0, 2, 6, 12, 19, 29, 40, 52, 65, 80,
	95,
	111
};

//Frequency 987
int b5[] =
    { 44, 127, 145, 162, 179, 195, 210, 222, 233, 242, 248, 252, 254, 252, 248,
	242, 233, 222, 210, 195, 179, 162, 145, 127, 108, 91, 74, 58, 43, 31,
	20,
	11, 5, 1, 0, 1, 5, 11, 20, 31, 43, 58, 74, 91, 108
};

//Frequency 1046
int c6[] =
    { 42, 127, 145, 164, 182, 198, 213, 226, 236, 245, 250, 253, 253, 250, 245,
	236, 226, 213, 198, 182, 164, 145, 127, 108, 89, 71, 55, 40, 27, 17, 8,
	3,
	0, 0, 3, 8, 17, 27, 40, 55, 71, 89, 108
};

//Frequency 1174
int d6[] =
    { 37, 127, 148, 169, 188, 206, 222, 235, 244, 251, 253, 252, 248, 240, 229,
	214, 198, 179, 159, 137, 116, 94, 74, 55, 39, 24, 13, 5, 1, 0, 2, 9, 18,
	31, 47, 65, 84, 105
};

//Frequency 1318
int e6[] =
    { 33, 127, 151, 174, 195, 214, 230, 242, 250, 253, 252, 247, 236, 222, 205,
	185, 162, 139, 114, 91, 68, 48, 31, 17, 6, 1, 0, 3, 11, 23, 39, 58, 79,
	102
};

//Frequency 1397
int f6[] =
    { 31, 127, 152, 177, 199, 219, 234, 246, 252, 253, 249, 241, 227, 209, 188,
	165, 139, 114, 88, 65, 44, 26, 12, 4, 0, 1, 7, 19, 34, 54, 76, 101
};

//Frequency 1568
int g6[] =
    { 28, 127, 155, 182, 206, 226, 241, 250, 254, 250, 241, 226, 206, 182, 155,
	127, 98, 71, 47, 27, 12, 3, 0, 3, 12, 27, 47, 71, 98
};

//Frequency 1760
int a6[] =
    { 25, 127, 158, 188, 213, 234, 247, 253, 251, 241, 224, 201, 173, 142, 111,
	80, 52, 29, 12, 2, 0, 6, 19, 40, 65, 95
};

//Frequency 1975
int b6[] =
    { 22, 127, 162, 195, 222, 242, 252, 252, 242, 222, 195, 162, 127, 91, 58,
	31, 11, 1, 1, 11, 31, 58, 91
};

int silence[] = { 1, 0 };

/*Songs*/
int *SONG_UP[] = { (int *)5, f3, f4, f5, f6, silence };
int *SONG_RIGHT[] = { (int *)8, c5, d5, e5, f5, g5, a5, b5, silence };

int *SONG_DOWN[] =
    { (int *)13, a4, d4, silence, d4, a4, d4, a4, d4, silence, a4, d4, a4,
	silence
};
int *SONG_LEFT[] = { (int *)8, b5, a5, g5, f5, e5, d5, c5, silence };
int *SONG_INTRO[] = { (int *)5, f5, g5, e5, a5, silence };
int *SONG_PEW[] = { (int *)4, f5, f5, a5, silence };
int *SONG_FANFARE[] = { (int *)8, b5, f6, f6, e6, e6, a5, b5, silence };
