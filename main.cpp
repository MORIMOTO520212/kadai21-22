/*
 * main.cpp
 * 温湿度計プログラム
 * Created: 2021/01/26
 * Author : MorimotoYuma
 */ 

#include <avr/io.h>
#include "TM1637Disp.h"
#include "DHT11.h"

int main(void)
{
	// 7セグLED表示パターン（0-9，°,C,o ）
	static UCHR	ptn[15] = {
			0x3F,	// 0:0
			0x06,	// 1:1
			0x5B,	// 2:2
			0x4F,	// 3:3
			0x66,	// 4:4
			0x6D,	// 5:5
			0x7D,	// 6:6
			0x07,	// 7:7
			0x7F,	// 8:8
			0x6F,	// 9:9
			0x63,	// °:10
			0x39,	// C:11
			0x5C,	// o:12
			0x40,   // -:13
			0x00    // Blank:14
	};
	UCHR		dispData[4];								// 表示用データバッファ
	//TM1637Disp disp(void);
	TM1637Disp	disp(&ptn[0]);							    // 4桁7Segインスタンス生成
	DHT11		TandH;										// 温湿度センサインスタンス生成
	_delay_ms(2000);										// 電源ON後2秒間待つ
	
	while (1) {
		if( !TandH.DHT11Read()){							// 温湿度センサから値を取得（成功）
			if(TandH.getTemperature() < 0){
				dispData[0] = 13;                           // 温度 マイナス
				dispData[3] = TandH.getTemperature_L()-0x80;// 温度 小数点
			}else{
				dispData[0] = 14;                           // 温度 空白
				dispData[3] = TandH.getTemperature_L();
			}
			if(TandH.getTemperature_H()/10){
				dispData[1] = TandH.getTemperature_H() / 10; // 温度 10の位 数値
			}else{
				dispData[1] = 14;                            // 温度 10の位 空白
			}
			dispData[2] = TandH.getTemperature_H() % 10 | 0x80;	 // 温度 1の位 数値
			disp.numDisp(dispData);
			_delay_ms(2000);
			
			if(TandH.getHumidity_H()/10){
				dispData[0] = TandH.getHumidity_H() / 10;	// 湿度10の位の値取得
			}else{
				dispData[0] = 14;                           // 湿度10の位の空白表示
			}
			dispData[1] = TandH.getHumidity_H() %10;		// 湿度1の位の値取得
			dispData[2] = 10;								// 湿度の単位表示
			dispData[3] = 12;								// 湿度の単位表示
			disp.numDisp(dispData);							// 7Segに表示
			_delay_ms(2000);
			
		} else {											// 温湿度センサからの値取得失敗
			dispData[0] = dispData[1] = dispData[2] = dispData[3] = 0x80;// 「....」を表示データとしてセット
			disp.numDisp(dispData);							// 7Segに表示
			_delay_ms(2000);								// 2秒待つ
		}
	}
	return 0;
}