/****************************************************************************/
/*	TM1637 + 4degit 7Seg LED Display Base Class								*/
/*																			*/
/*	本ファイルは，TM1637+4Digit 7Seg LEDに情報を表示するためのBase Class を定義	*/
/*	したものである．															*/
/*																			*/
/*	■制約事項																*/
/*		・TM1637Disp関数はCLK及びDIOが同一PORTにある場合のみ利用可能である．		*/
/*		・TM1637Disp関数は非リエントラントである．								*/
/*		・ライセンスはGPLに準じるものとする．									*/
/*																			*/
/*											東京国際工科専門職大学　荒金匡德	*/
/****************************************************************************/
/* #000		2021/01/08 初版										荒金匡德		*/
/*																			*/
/****************************************************************************/
#include "TM1637Disp.h"

// コンストラクタ
TM1637Disp::TM1637Disp(void){
	TM_DDR |= CLK|DIO;				// CLKポートDIOポートを出力に設定
	// 7セグLED表示パターンデフォルト値設定（ 0-9, Blank ）
	this->segPattern[0]  = 0x3F;			// 0
	this->segPattern[1]  = 0x06;			// 1
	this->segPattern[2]  = 0x5B;			// 2
	this->segPattern[3]  = 0x4F;			// 3
	this->segPattern[4]  = 0x66;			// 4
	this->segPattern[5]  = 0x6D;			// 5
	this->segPattern[6]  = 0x7D;			// 6
	this->segPattern[7]  = 0x07;			// 7
	this->segPattern[8]  = 0x7F;			// 8
	this->segPattern[9]  = 0x6F;			// 9
	this->segPattern[10] = 0x00;			// Blank

}
TM1637Disp::TM1637Disp(UCHR *ptr){
	TM_DDR |= CLK|DIO;
	//7セグLED表示パターン設定
	UCHR *dest = &(this->segPattern[0]);
	for(int i = 0;i < 128; i++){
		if(*ptr == 0xFF){
			break;
		}
		*dest++ = *ptr++;
	}
}
///////////////////////////////////////////////////////////
//
//    TM1637 4桁7セグメントLED表示用関数
//    呼出形式：void		num_disp( UCHR *num);
//
//    引数：UCHR *num  :  表示データが入ったデータの先頭アドレス
//                        (表示データは4バイト分の表示コード)
//    戻り値：なし
//
//　　機能：引数で受け取った4バイトのメモリに入っているデータを
//         4桁7セグメントLEDに表示する．
//         但し，上記表示文字に.を付加する場合は，コードに0x80を
//         加算する．
//
///////////////////////////////////////////////////////////
void	TM1637Disp::numDisp( UCHR *num){

	TM_DDR |= CLK|DIO;								// CLKポートDIOポートを出力に設定
	Start_bit();									// Start bit 送信
	send(MODE);										// モード送信
	Stop_bit();										// Stop bit 送信

	for( int i = 0; i < 4; i++){					// 4桁分のLoop
		Start_bit();								// START Bit 送信
		send(ADDR + i);								// 表示エリアのアドレス送信
		if( *num >= 0x80){							// ドットを付加する場合
			send(this->segPattern[(int)(*num-0x80)]|0x80);// ドットのセグメントを付加した情報を送信
			} else {								// ドットを付加しない場合
			send(this->segPattern[(int)*num]);			// ドットなしの表示情報を送信
		}
		Stop_bit();									// STOP Bit 送信
		num++;										// 表示桁を進める
	}

	Start_bit();									// STOP Bit 送信
	send(DCNT);										// Display Control Command 送信
	Stop_bit();										// STOP Bit 送信
}


///////////////////////////////////////////////////////////
//		START Bit 送信関数
///////////////////////////////////////////////////////////
void	TM1637Disp::Start_bit( void){
	CLK_H;											// CLKをHIGH
	_delay_us(DLY);
	DIO_H;											// DIOをHIGH
	_delay_us(DLY);
	DIO_L;											// DIOをLOW
	_delay_us(DLY);
}

///////////////////////////////////////////////////////////
//		STOP Bit 送信関数
///////////////////////////////////////////////////////////
void	TM1637Disp::Stop_bit( void){
	DIO_L;											// DIOをLOW
	_delay_us(DLY);
	CLK_H;											// CLKをHIGH
	_delay_us(DLY);
	DIO_H;											// DIOをHIGH
	_delay_us(DLY);
}

///////////////////////////////////////////////////////////
//		Byte data 送信関数
///////////////////////////////////////////////////////////
void	TM1637Disp::send( UCHR	data){
	for( int i=0; i<8; i++){						// 8Bitデータ送出なので8回Loopする．
		CLK_L;										// CLKにLOWを出力
		_delay_us(DLY);
		if( (data & 0x01) == 0){					// 出力データが0の時
			DIO_L;									//　 DIOにLOWを出力
		} else{										// 出力データが1の時
			DIO_H;									// 　DIOにHIGHを出力
		}
		_delay_us(DLY);
		CLK_H;										// CLKにHIGHを出力
		_delay_us(DLY);
		data >>= 1;									// 出力データを1ビット右シフト
	}
	// ACK待ち
	CLK_L;
	_delay_us(DLY);
	// CLKをHIGHにしてACK解除
	CLK_H;											// CLKにHIGHを出力
	_delay_us(DLY);
	DIO_L;											// DIOにLOWを出力
	_delay_us(DLY);
	CLK_L;											// CLKにLOWを出力
	_delay_us(DLY);
}