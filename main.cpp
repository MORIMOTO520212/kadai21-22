/*
 * main.cpp
 * �����x�v�v���O����
 * Created: 2021/01/26
 * Author : MorimotoYuma
 */ 

#include <avr/io.h>
#include "TM1637Disp.h"
#include "DHT11.h"

int main(void)
{
	// 7�Z�OLED�\���p�^�[���i0-9�C��,C,o �j
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
			0x63,	// ��:10
			0x39,	// C:11
			0x5C,	// o:12
			0x40,   // -:13
			0x00    // Blank:14
	};
	UCHR		dispData[4];								// �\���p�f�[�^�o�b�t�@
	//TM1637Disp disp(void);
	TM1637Disp	disp(&ptn[0]);							    // 4��7Seg�C���X�^���X����
	DHT11		TandH;										// �����x�Z���T�C���X�^���X����
	_delay_ms(2000);										// �d��ON��2�b�ԑ҂�
	
	while (1) {
		if( !TandH.DHT11Read()){							// �����x�Z���T����l���擾�i�����j
			if(TandH.getTemperature() < 0){
				dispData[0] = 13;                           // ���x �}�C�i�X
				dispData[3] = TandH.getTemperature_L()-0x80;// ���x �����_
			}else{
				dispData[0] = 14;                           // ���x ��
				dispData[3] = TandH.getTemperature_L();
			}
			if(TandH.getTemperature_H()/10){
				dispData[1] = TandH.getTemperature_H() / 10; // ���x 10�̈� ���l
			}else{
				dispData[1] = 14;                            // ���x 10�̈� ��
			}
			dispData[2] = TandH.getTemperature_H() % 10 | 0x80;	 // ���x 1�̈� ���l
			disp.numDisp(dispData);
			_delay_ms(2000);
			
			if(TandH.getHumidity_H()/10){
				dispData[0] = TandH.getHumidity_H() / 10;	// ���x10�̈ʂ̒l�擾
			}else{
				dispData[0] = 14;                           // ���x10�̈ʂ̋󔒕\��
			}
			dispData[1] = TandH.getHumidity_H() %10;		// ���x1�̈ʂ̒l�擾
			dispData[2] = 10;								// ���x�̒P�ʕ\��
			dispData[3] = 12;								// ���x�̒P�ʕ\��
			disp.numDisp(dispData);							// 7Seg�ɕ\��
			_delay_ms(2000);
			
		} else {											// �����x�Z���T����̒l�擾���s
			dispData[0] = dispData[1] = dispData[2] = dispData[3] = 0x80;// �u....�v��\���f�[�^�Ƃ��ăZ�b�g
			disp.numDisp(dispData);							// 7Seg�ɕ\��
			_delay_ms(2000);								// 2�b�҂�
		}
	}
	return 0;
}