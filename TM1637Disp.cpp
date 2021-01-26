/****************************************************************************/
/*	TM1637 + 4degit 7Seg LED Display Base Class								*/
/*																			*/
/*	�{�t�@�C���́CTM1637+4Digit 7Seg LED�ɏ���\�����邽�߂�Base Class ���`	*/
/*	�������̂ł���D															*/
/*																			*/
/*	�����񎖍�																*/
/*		�ETM1637Disp�֐���CLK�y��DIO������PORT�ɂ���ꍇ�̂ݗ��p�\�ł���D		*/
/*		�ETM1637Disp�֐��͔񃊃G���g�����g�ł���D								*/
/*		�E���C�Z���X��GPL�ɏ�������̂Ƃ���D									*/
/*																			*/
/*											�������ۍH�Ȑ��E��w�@�r������	*/
/****************************************************************************/
/* #000		2021/01/08 ����										�r������		*/
/*																			*/
/****************************************************************************/
#include "TM1637Disp.h"

// �R���X�g���N�^
TM1637Disp::TM1637Disp(void){
	TM_DDR |= CLK|DIO;				// CLK�|�[�gDIO�|�[�g���o�͂ɐݒ�
	// 7�Z�OLED�\���p�^�[���f�t�H���g�l�ݒ�i 0-9, Blank �j
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
	//7�Z�OLED�\���p�^�[���ݒ�
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
//    TM1637 4��7�Z�O�����gLED�\���p�֐�
//    �ďo�`���Fvoid		num_disp( UCHR *num);
//
//    �����FUCHR *num  :  �\���f�[�^���������f�[�^�̐擪�A�h���X
//                        (�\���f�[�^��4�o�C�g���̕\���R�[�h)
//    �߂�l�F�Ȃ�
//
//�@�@�@�\�F�����Ŏ󂯎����4�o�C�g�̃������ɓ����Ă���f�[�^��
//         4��7�Z�O�����gLED�ɕ\������D
//         �A���C��L�\��������.��t������ꍇ�́C�R�[�h��0x80��
//         ���Z����D
//
///////////////////////////////////////////////////////////
void	TM1637Disp::numDisp( UCHR *num){

	TM_DDR |= CLK|DIO;								// CLK�|�[�gDIO�|�[�g���o�͂ɐݒ�
	Start_bit();									// Start bit ���M
	send(MODE);										// ���[�h���M
	Stop_bit();										// Stop bit ���M

	for( int i = 0; i < 4; i++){					// 4������Loop
		Start_bit();								// START Bit ���M
		send(ADDR + i);								// �\���G���A�̃A�h���X���M
		if( *num >= 0x80){							// �h�b�g��t������ꍇ
			send(this->segPattern[(int)(*num-0x80)]|0x80);// �h�b�g�̃Z�O�����g��t���������𑗐M
			} else {								// �h�b�g��t�����Ȃ��ꍇ
			send(this->segPattern[(int)*num]);			// �h�b�g�Ȃ��̕\�����𑗐M
		}
		Stop_bit();									// STOP Bit ���M
		num++;										// �\������i�߂�
	}

	Start_bit();									// STOP Bit ���M
	send(DCNT);										// Display Control Command ���M
	Stop_bit();										// STOP Bit ���M
}


///////////////////////////////////////////////////////////
//		START Bit ���M�֐�
///////////////////////////////////////////////////////////
void	TM1637Disp::Start_bit( void){
	CLK_H;											// CLK��HIGH
	_delay_us(DLY);
	DIO_H;											// DIO��HIGH
	_delay_us(DLY);
	DIO_L;											// DIO��LOW
	_delay_us(DLY);
}

///////////////////////////////////////////////////////////
//		STOP Bit ���M�֐�
///////////////////////////////////////////////////////////
void	TM1637Disp::Stop_bit( void){
	DIO_L;											// DIO��LOW
	_delay_us(DLY);
	CLK_H;											// CLK��HIGH
	_delay_us(DLY);
	DIO_H;											// DIO��HIGH
	_delay_us(DLY);
}

///////////////////////////////////////////////////////////
//		Byte data ���M�֐�
///////////////////////////////////////////////////////////
void	TM1637Disp::send( UCHR	data){
	for( int i=0; i<8; i++){						// 8Bit�f�[�^���o�Ȃ̂�8��Loop����D
		CLK_L;										// CLK��LOW���o��
		_delay_us(DLY);
		if( (data & 0x01) == 0){					// �o�̓f�[�^��0�̎�
			DIO_L;									//�@ DIO��LOW���o��
		} else{										// �o�̓f�[�^��1�̎�
			DIO_H;									// �@DIO��HIGH���o��
		}
		_delay_us(DLY);
		CLK_H;										// CLK��HIGH���o��
		_delay_us(DLY);
		data >>= 1;									// �o�̓f�[�^��1�r�b�g�E�V�t�g
	}
	// ACK�҂�
	CLK_L;
	_delay_us(DLY);
	// CLK��HIGH�ɂ���ACK����
	CLK_H;											// CLK��HIGH���o��
	_delay_us(DLY);
	DIO_L;											// DIO��LOW���o��
	_delay_us(DLY);
	CLK_L;											// CLK��LOW���o��
	_delay_us(DLY);
}