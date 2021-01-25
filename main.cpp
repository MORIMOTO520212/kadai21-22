/*
 * main.cpp
 * 温湿度計サンプルプログラム
 * Created: 2021/01/08 13:55:42
 * Author : aragane.masanori
 */ 

#include <avr/io.h>
#include "TM1637Disp.h"
#include "DHT11.h"

int main(void)
{
	UCHR		dispData[4];								// 表示用データバッファ
	TM1637Disp	disp;										// 4桁7Segインスタンス生成
	DHT11		TandH;										// 温湿度センサインスタンス生成
	_delay_ms(2000);										// 電源ON後2秒間待つ
	
    while (1) {
		if( !TandH.DHT11Read()){							// 温湿度センサから値を取得（成功）
			dispData[0] = TandH.getTemperature_H() / 10;	// 温度10の位の値取得
			dispData[1] = TandH.getTemperature_H() %10 | 0x80;// 温度1の位の値取得
			dispData[2] = TandH.getHumidity_H() / 10;		// 湿度10の位の値取得
			dispData[3] = TandH.getHumidity_H() % 10;		// 湿度1の位の値取得
		} else {											// 温湿度センサからの値取得失敗
			dispData[0] = dispData[1] = dispData[2] = dispData[3] = 0x80;// 「....」を表示データとしてセット
		}
		disp.numDisp(dispData);								// 7Segに表示
		_delay_ms(2000);									// 2秒間待つ
    }
	return 0;
}

