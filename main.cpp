/*
 * main.cpp
 * �����x�v�T���v���v���O����
 * Created: 2021/01/08 13:55:42
 * Author : aragane.masanori
 */ 

#include <avr/io.h>
#include "TM1637Disp.h"
#include "DHT11.h"

int main(void)
{
	UCHR		dispData[4];								// �\���p�f�[�^�o�b�t�@
	TM1637Disp	disp;							        	// 4��7Seg�C���X�^���X����
	DHT11		TandH;										// �����x�Z���T�C���X�^���X����
	_delay_ms(2000);										// �d��ON��2�b�ԑ҂�
	
	while (1) {
		if( !TandH.DHT11Read()){							// �����x�Z���T����l���擾�i�����j
			dispData[0] = TandH.getTemperature_H() / 10;	// ���x10�̈ʂ̒l�擾
			dispData[1] = TandH.getTemperature_H() %10;		// ���x1�̈ʂ̒l�擾
			dispData[2] = 10;								// ���x�̒P�ʕ\��
			dispData[3] = 11;								// ���x�̒P�ʕ\��
			disp.numDisp(dispData);							// 7Seg�ɕ\��
			_delay_ms(2000);								// 2�b�҂�
			dispData[0] = TandH.getHumidity_H() / 10;		// ���x10�̈ʂ̒l�擾
			dispData[1] = TandH.getHumidity_H() %10;		// ���x1�̈ʂ̒l�擾
			dispData[2] = 10;								// ���x�̒P�ʕ\��
			dispData[3] = 12;								// ���x�̒P�ʕ\��
			disp.numDisp(dispData);							// 7Seg�ɕ\��
			_delay_ms(2000);								// 2�b�҂�
		} else {											// �����x�Z���T����̒l�擾���s
			dispData[0] = dispData[1] = dispData[2] = dispData[3] = 0x80;// �u....�v��\���f�[�^�Ƃ��ăZ�b�g
			disp.numDisp(dispData);							// 7Seg�ɕ\��
			_delay_ms(2000);								// 2�b�҂�
		}
		
		
	}
	return 0;
}