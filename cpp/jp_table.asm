; Huge jump table to enable fast random access in external RAM.

cseg at 4000h
l_jp_table:

; destination for A = 0:
   MOV DPTR, #0
   RET


; destination for A = 1:
   MOV DPTR, #1
   RET


; destination for A = 2:
   MOV DPTR, #2
   RET


; destination for A = 3:
   MOV DPTR, #3
   RET


; destination for A = 4:
   MOV DPTR, #4
   RET


; destination for A = 5:
   MOV DPTR, #5
   RET


; destination for A = 6:
   MOV DPTR, #6
   RET


; destination for A = 7:
   MOV DPTR, #7
   RET


; destination for A = 8:
   MOV DPTR, #8
   RET


; destination for A = 9:
   MOV DPTR, #9
   RET


; destination for A = 10:
   MOV DPTR, #10
   RET


; destination for A = 11:
   MOV DPTR, #11
   RET


; destination for A = 12:
   MOV DPTR, #12
   RET


; destination for A = 13:
   MOV DPTR, #13
   RET


; destination for A = 14:
   MOV DPTR, #14
   RET


; destination for A = 15:
   MOV DPTR, #15
   RET


; destination for A = 16:
   MOV DPTR, #16
   RET


; destination for A = 17:
   MOV DPTR, #17
   RET


; destination for A = 18:
   MOV DPTR, #18
   RET


; destination for A = 19:
   MOV DPTR, #19
   RET


; destination for A = 20:
   MOV DPTR, #20
   RET


; destination for A = 21:
   MOV DPTR, #21
   RET


; destination for A = 22:
   MOV DPTR, #22
   RET


; destination for A = 23:
   MOV DPTR, #23
   RET


; destination for A = 24:
   MOV DPTR, #24
   RET


; destination for A = 25:
   MOV DPTR, #25
   RET


; destination for A = 26:
   MOV DPTR, #26
   RET


; destination for A = 27:
   MOV DPTR, #27
   RET


; destination for A = 28:
   MOV DPTR, #28
   RET


; destination for A = 29:
   MOV DPTR, #29
   RET


; destination for A = 30:
   MOV DPTR, #30
   RET


; destination for A = 31:
   MOV DPTR, #31
   RET


; destination for A = 32:
   MOV DPTR, #32
   RET


; destination for A = 33:
   MOV DPTR, #33
   RET


; destination for A = 34:
   MOV DPTR, #34
   RET


; destination for A = 35:
   MOV DPTR, #35
   RET


; destination for A = 36:
   MOV DPTR, #36
   RET


; destination for A = 37:
   MOV DPTR, #37
   RET


; destination for A = 38:
   MOV DPTR, #38
   RET


; destination for A = 39:
   MOV DPTR, #39
   RET


; destination for A = 40:
   MOV DPTR, #40
   RET


; destination for A = 41:
   MOV DPTR, #41
   RET


; destination for A = 42:
   MOV DPTR, #42
   RET


; destination for A = 43:
   MOV DPTR, #43
   RET


; destination for A = 44:
   MOV DPTR, #44
   RET


; destination for A = 45:
   MOV DPTR, #45
   RET


; destination for A = 46:
   MOV DPTR, #46
   RET


; destination for A = 47:
   MOV DPTR, #47
   RET


; destination for A = 48:
   MOV DPTR, #48
   RET


; destination for A = 49:
   MOV DPTR, #49
   RET


; destination for A = 50:
   MOV DPTR, #50
   RET


; destination for A = 51:
   MOV DPTR, #51
   RET


; destination for A = 52:
   MOV DPTR, #52
   RET


; destination for A = 53:
   MOV DPTR, #53
   RET


; destination for A = 54:
   MOV DPTR, #54
   RET


; destination for A = 55:
   MOV DPTR, #55
   RET


; destination for A = 56:
   MOV DPTR, #56
   RET


; destination for A = 57:
   MOV DPTR, #57
   RET


; destination for A = 58:
   MOV DPTR, #58
   RET


; destination for A = 59:
   MOV DPTR, #59
   RET


; destination for A = 60:
   MOV DPTR, #60
   RET


; destination for A = 61:
   MOV DPTR, #61
   RET


; destination for A = 62:
   MOV DPTR, #62
   RET


; destination for A = 63:
   MOV DPTR, #63
   RET


; destination for A = 64:
   MOV DPTR, #64
   RET


; destination for A = 65:
   MOV DPTR, #65
   RET


; destination for A = 66:
   MOV DPTR, #66
   RET


; destination for A = 67:
   MOV DPTR, #67
   RET


; destination for A = 68:
   MOV DPTR, #68
   RET


; destination for A = 69:
   MOV DPTR, #69
   RET


; destination for A = 70:
   MOV DPTR, #70
   RET


; destination for A = 71:
   MOV DPTR, #71
   RET


; destination for A = 72:
   MOV DPTR, #72
   RET


; destination for A = 73:
   MOV DPTR, #73
   RET


; destination for A = 74:
   MOV DPTR, #74
   RET


; destination for A = 75:
   MOV DPTR, #75
   RET


; destination for A = 76:
   MOV DPTR, #76
   RET


; destination for A = 77:
   MOV DPTR, #77
   RET


; destination for A = 78:
   MOV DPTR, #78
   RET


; destination for A = 79:
   MOV DPTR, #79
   RET


; destination for A = 80:
   MOV DPTR, #80
   RET


; destination for A = 81:
   MOV DPTR, #81
   RET


; destination for A = 82:
   MOV DPTR, #82
   RET


; destination for A = 83:
   MOV DPTR, #83
   RET


; destination for A = 84:
   MOV DPTR, #84
   RET


; destination for A = 85:
   MOV DPTR, #85
   RET


; destination for A = 86:
   MOV DPTR, #86
   RET


; destination for A = 87:
   MOV DPTR, #87
   RET


; destination for A = 88:
   MOV DPTR, #88
   RET


; destination for A = 89:
   MOV DPTR, #89
   RET


; destination for A = 90:
   MOV DPTR, #90
   RET


; destination for A = 91:
   MOV DPTR, #91
   RET


; destination for A = 92:
   MOV DPTR, #92
   RET


; destination for A = 93:
   MOV DPTR, #93
   RET


; destination for A = 94:
   MOV DPTR, #94
   RET


; destination for A = 95:
   MOV DPTR, #95
   RET


; destination for A = 96:
   MOV DPTR, #96
   RET


; destination for A = 97:
   MOV DPTR, #97
   RET


; destination for A = 98:
   MOV DPTR, #98
   RET


; destination for A = 99:
   MOV DPTR, #99
   RET


; destination for A = 100:
   MOV DPTR, #100
   RET


; destination for A = 101:
   MOV DPTR, #101
   RET


; destination for A = 102:
   MOV DPTR, #102
   RET


; destination for A = 103:
   MOV DPTR, #103
   RET


; destination for A = 104:
   MOV DPTR, #104
   RET


; destination for A = 105:
   MOV DPTR, #105
   RET


; destination for A = 106:
   MOV DPTR, #106
   RET


; destination for A = 107:
   MOV DPTR, #107
   RET


; destination for A = 108:
   MOV DPTR, #108
   RET


; destination for A = 109:
   MOV DPTR, #109
   RET


; destination for A = 110:
   MOV DPTR, #110
   RET


; destination for A = 111:
   MOV DPTR, #111
   RET


; destination for A = 112:
   MOV DPTR, #112
   RET


; destination for A = 113:
   MOV DPTR, #113
   RET


; destination for A = 114:
   MOV DPTR, #114
   RET


; destination for A = 115:
   MOV DPTR, #115
   RET


; destination for A = 116:
   MOV DPTR, #116
   RET


; destination for A = 117:
   MOV DPTR, #117
   RET


; destination for A = 118:
   MOV DPTR, #118
   RET


; destination for A = 119:
   MOV DPTR, #119
   RET


; destination for A = 120:
   MOV DPTR, #120
   RET


; destination for A = 121:
   MOV DPTR, #121
   RET


; destination for A = 122:
   MOV DPTR, #122
   RET


; destination for A = 123:
   MOV DPTR, #123
   RET


; destination for A = 124:
   MOV DPTR, #124
   RET


; destination for A = 125:
   MOV DPTR, #125
   RET


; destination for A = 126:
   MOV DPTR, #126
   RET


; destination for A = 127:
   MOV DPTR, #127
   RET


; destination for A = 128:
   MOV DPTR, #128
   RET


; destination for A = 129:
   MOV DPTR, #129
   RET


; destination for A = 130:
   MOV DPTR, #130
   RET


; destination for A = 131:
   MOV DPTR, #131
   RET


; destination for A = 132:
   MOV DPTR, #132
   RET


; destination for A = 133:
   MOV DPTR, #133
   RET


; destination for A = 134:
   MOV DPTR, #134
   RET


; destination for A = 135:
   MOV DPTR, #135
   RET


; destination for A = 136:
   MOV DPTR, #136
   RET


; destination for A = 137:
   MOV DPTR, #137
   RET


; destination for A = 138:
   MOV DPTR, #138
   RET


; destination for A = 139:
   MOV DPTR, #139
   RET


; destination for A = 140:
   MOV DPTR, #140
   RET


; destination for A = 141:
   MOV DPTR, #141
   RET


; destination for A = 142:
   MOV DPTR, #142
   RET


; destination for A = 143:
   MOV DPTR, #143
   RET


; destination for A = 144:
   MOV DPTR, #144
   RET


; destination for A = 145:
   MOV DPTR, #145
   RET


; destination for A = 146:
   MOV DPTR, #146
   RET


; destination for A = 147:
   MOV DPTR, #147
   RET


; destination for A = 148:
   MOV DPTR, #148
   RET


; destination for A = 149:
   MOV DPTR, #149
   RET


; destination for A = 150:
   MOV DPTR, #150
   RET


; destination for A = 151:
   MOV DPTR, #151
   RET


; destination for A = 152:
   MOV DPTR, #152
   RET


; destination for A = 153:
   MOV DPTR, #153
   RET


; destination for A = 154:
   MOV DPTR, #154
   RET


; destination for A = 155:
   MOV DPTR, #155
   RET


; destination for A = 156:
   MOV DPTR, #156
   RET


; destination for A = 157:
   MOV DPTR, #157
   RET


; destination for A = 158:
   MOV DPTR, #158
   RET


; destination for A = 159:
   MOV DPTR, #159
   RET


; destination for A = 160:
   MOV DPTR, #160
   RET


; destination for A = 161:
   MOV DPTR, #161
   RET


; destination for A = 162:
   MOV DPTR, #162
   RET


; destination for A = 163:
   MOV DPTR, #163
   RET


; destination for A = 164:
   MOV DPTR, #164
   RET


; destination for A = 165:
   MOV DPTR, #165
   RET


; destination for A = 166:
   MOV DPTR, #166
   RET


; destination for A = 167:
   MOV DPTR, #167
   RET


; destination for A = 168:
   MOV DPTR, #168
   RET


; destination for A = 169:
   MOV DPTR, #169
   RET


; destination for A = 170:
   MOV DPTR, #170
   RET


; destination for A = 171:
   MOV DPTR, #171
   RET


; destination for A = 172:
   MOV DPTR, #172
   RET


; destination for A = 173:
   MOV DPTR, #173
   RET


; destination for A = 174:
   MOV DPTR, #174
   RET


; destination for A = 175:
   MOV DPTR, #175
   RET


; destination for A = 176:
   MOV DPTR, #176
   RET


; destination for A = 177:
   MOV DPTR, #177
   RET


; destination for A = 178:
   MOV DPTR, #178
   RET


; destination for A = 179:
   MOV DPTR, #179
   RET


; destination for A = 180:
   MOV DPTR, #180
   RET


; destination for A = 181:
   MOV DPTR, #181
   RET


; destination for A = 182:
   MOV DPTR, #182
   RET


; destination for A = 183:
   MOV DPTR, #183
   RET


; destination for A = 184:
   MOV DPTR, #184
   RET


; destination for A = 185:
   MOV DPTR, #185
   RET


; destination for A = 186:
   MOV DPTR, #186
   RET


; destination for A = 187:
   MOV DPTR, #187
   RET


; destination for A = 188:
   MOV DPTR, #188
   RET


; destination for A = 189:
   MOV DPTR, #189
   RET


; destination for A = 190:
   MOV DPTR, #190
   RET


; destination for A = 191:
   MOV DPTR, #191
   RET


; destination for A = 192:
   MOV DPTR, #192
   RET


; destination for A = 193:
   MOV DPTR, #193
   RET


; destination for A = 194:
   MOV DPTR, #194
   RET


; destination for A = 195:
   MOV DPTR, #195
   RET


; destination for A = 196:
   MOV DPTR, #196
   RET


; destination for A = 197:
   MOV DPTR, #197
   RET


; destination for A = 198:
   MOV DPTR, #198
   RET


; destination for A = 199:
   MOV DPTR, #199
   RET


; destination for A = 200:
   MOV DPTR, #200
   RET


; destination for A = 201:
   MOV DPTR, #201
   RET


; destination for A = 202:
   MOV DPTR, #202
   RET


; destination for A = 203:
   MOV DPTR, #203
   RET


; destination for A = 204:
   MOV DPTR, #204
   RET


; destination for A = 205:
   MOV DPTR, #205
   RET


; destination for A = 206:
   MOV DPTR, #206
   RET


; destination for A = 207:
   MOV DPTR, #207
   RET


; destination for A = 208:
   MOV DPTR, #208
   RET


; destination for A = 209:
   MOV DPTR, #209
   RET


; destination for A = 210:
   MOV DPTR, #210
   RET


; destination for A = 211:
   MOV DPTR, #211
   RET


; destination for A = 212:
   MOV DPTR, #212
   RET


; destination for A = 213:
   MOV DPTR, #213
   RET


; destination for A = 214:
   MOV DPTR, #214
   RET


; destination for A = 215:
   MOV DPTR, #215
   RET


; destination for A = 216:
   MOV DPTR, #216
   RET


; destination for A = 217:
   MOV DPTR, #217
   RET


; destination for A = 218:
   MOV DPTR, #218
   RET


; destination for A = 219:
   MOV DPTR, #219
   RET


; destination for A = 220:
   MOV DPTR, #220
   RET


; destination for A = 221:
   MOV DPTR, #221
   RET


; destination for A = 222:
   MOV DPTR, #222
   RET


; destination for A = 223:
   MOV DPTR, #223
   RET


; destination for A = 224:
   MOV DPTR, #224
   RET


; destination for A = 225:
   MOV DPTR, #225
   RET


; destination for A = 226:
   MOV DPTR, #226
   RET


; destination for A = 227:
   MOV DPTR, #227
   RET


; destination for A = 228:
   MOV DPTR, #228
   RET


; destination for A = 229:
   MOV DPTR, #229
   RET


; destination for A = 230:
   MOV DPTR, #230
   RET


; destination for A = 231:
   MOV DPTR, #231
   RET


; destination for A = 232:
   MOV DPTR, #232
   RET


; destination for A = 233:
   MOV DPTR, #233
   RET


; destination for A = 234:
   MOV DPTR, #234
   RET


; destination for A = 235:
   MOV DPTR, #235
   RET


; destination for A = 236:
   MOV DPTR, #236
   RET


; destination for A = 237:
   MOV DPTR, #237
   RET


; destination for A = 238:
   MOV DPTR, #238
   RET


; destination for A = 239:
   MOV DPTR, #239
   RET


; destination for A = 240:
   MOV DPTR, #240
   RET


; destination for A = 241:
   MOV DPTR, #241
   RET


; destination for A = 242:
   MOV DPTR, #242
   RET


; destination for A = 243:
   MOV DPTR, #243
   RET


; destination for A = 244:
   MOV DPTR, #244
   RET


; destination for A = 245:
   MOV DPTR, #245
   RET


; destination for A = 246:
   MOV DPTR, #246
   RET


; destination for A = 247:
   MOV DPTR, #247
   RET


; destination for A = 248:
   MOV DPTR, #248
   RET


; destination for A = 249:
   MOV DPTR, #249
   RET


; destination for A = 250:
   MOV DPTR, #250
   RET


; destination for A = 251:
   MOV DPTR, #251
   RET


; destination for A = 252:
   MOV DPTR, #252
   RET


; destination for A = 253:
   MOV DPTR, #253
   RET


; destination for A = 254:
   MOV DPTR, #254
   RET


; destination for A = 255:
   MOV DPTR, #255
   RET
