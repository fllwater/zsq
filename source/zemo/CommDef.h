/*****************************************************************************
������˾�� ������ͼ�����Ƽ����޹�˾ Copyright(c)@2010
��������:
    1.ͨ������
    2.����ͨ��Э��
    3.ִ���û�����
��ʷ��¼:
    <����>     <ʱ��>       <����>
     zhj      2009-12-26     ����
******************************************************************************
*/
#ifndef _COMM_DEF_H_
#define _COMM_DEF_H_

//ͨѶ�˿ڶ���
typedef enum
{
    DBG_PORT = 0, //ϵͳά���͵���ͨ��
    CON_PORT = 1, //PC����̨ͨ��
//    KEY_PORT = 2, //KEY�˿�  
//    IR_PORT  = 3, //IR�˿�
    UART_NUM      //������Ŀ
}COMMNR;

//------------------------------------------------------------------------
//ͨ��Э��(�������շ�,PC->USBͨ��)
//1.֡��ʽ��[ǰ����(1Byte)][������(1Byte)][֡����(9Bytes)][У���(1Byte)]
//2.У�����ָ�������룬֡���ݡ������ֽڵ�У����
//3.֡�ܳ��̶�Ϊ12Bytes
//--------------------------------------------------------------------------
#define COMM_FRM_LEN    (12)
#define COMM_IR_LEN     (4)
#define COMM_FRM_HEAD   (0x53)


//------------------------------------------------------------------------
//USB���Ͱ�ͨ��Э��(USB->PCͨ��)
//֡��ʽ��[ǰ����][֡����(6·��դ��λ��(4Bytes))][У���(1Byte)]
//------------------------------------------------------------------------
//#define USB_OPTFRM_LEN      (18)
//#define UDP_OPTFRM_LEN      (18)
//#define USB_SEND_BUF_LEN    (360)
#define UDP_SEND_DATA_LEN   (900)
//#define USB_PACK_NUM		(20)
//#define UDP_PACK_NUM        (50)

//ͨ��������(�������շ�,PC->USBͨ��)
typedef enum
{
    CMD_INVALID                = 0x00,   //��Ч����

    //ͨ������0x10~0x2F
    CMD_HD_VER                 = 0x10,   //��ȡӲ���汾
    CMD_SW_VER                 = 0x11,   //��ȡ����汾
    CMD_SET_BEEP               = 0x12,   //����
    CMD_DEVRST                 = 0x15,   //�豸��λ
    CMD_DEVRST_END             = 0x16,   //�豸��λ
    CMD_ERR_DBGPRINT           = 0x17,   //ϵͳ���Դ�ӡ����
    CMD_ERR_RPT                = 0x18,   //ϵͳ����״̬����
    CMD_READ_DEVPARA           = 0x20,   //��ȡ�豸����
    CMD_SET_DEVSN              = 0x27,   //�����豸���
    CMD_GET_DEVSN              = 0x28,   //��ȡ�豸���
    CMD_RS232_EN               = 0x2F,   //RS232ʹ�ܿ���

    //��������0x30~0x5F
    CMD_SET_WORKMODE           = 0x30,   //�����豸����ģʽ
    CMD_SJ_ZERO                = 0x35,   //��դ����
    CMD_SJ_GO                  = 0x36,   //��դ�ƶ�
    CMD_SJ_GO_END              = 0x37,
    CMD_SJ_GODIST              = 0x38,   //��դ�ƶ�����
    CMD_SJ_GODIST_END          = 0x39,
    CMD_SJ_GOTOZERO            = 0x3A,   //��դ�ƶ������
    CMD_SJ_GOTOZERO_END        = 0x3B,
    CMD_SJ_GO_STOP             = 0x3C,   //��դ�ƶ�ֹͣ
    CMD_SJ_GO_STOP_END         = 0x3D,
//    CMD_SJ_OPTXXX              = 0x3E,   //����ƶ�����
//    CMD_SJ_OPTXXX_END          = 0x3F,   //����ƶ�����
    CMD_SJ_GETDATA             = 0x40,   //������դʾֵ
//    CMD_SJ_TEST_TASK           = 0x55,   //OPT��������
    CMD_SJ_MOTOR_GO            = 0x56,   //�������
//    CMD_HELM_CTRL              = 0x57,   //�������
//    CMD_HELM_CTRL_END          = 0x58,   //�������
//    CMD_USB_CONNECT            = 0x59,   //USB���ӿ���
//    CMD_USB_CONNECT_END        = 0x5A,   //USB���ӿ������
//    CMD_SET_ETH_IP             = 0x5B,   //����ip��ַ
//    CMD_GET_ETH_IP             = 0x5C,   //��ȡip��ַ
//    CMD_SET_ETH_MAC            = 0x5D,   //����MAC��ַ
//    CMD_GET_ETH_MAC            = 0x5E,   //��ȡMAC��ַ
//    CMD_GET_MISCINFO           = 0x61,   //��ȡ�豸�ۺ���Ϣ
//    CMD_KEY_OK                 = 0x63,   //�����а���OK��
    CMD_RST_OPT                = 0x64,   //��λ��դֵ
    CMD_RST_OPT_END            = 0x65,   //��λ��դֵ
//    CMD_XZ_SCAN                = 0x66,   //x,z����ɨ��
//    CMD_XZ_SCAN_END            = 0x67,   //x,z����ɨ��

//    CMD_SETSCAN_PARAM          = 0x69,   //����ɨ�����
//    CMD_SJ_TOUCH               = 0x70,   //���е��ֱ���Ӵ�����
//    CMD_SJ_TOUCH_END           = 0x71,   //���е��ֱ���Ӵ�����
//   
//    CMD_SJ_SMARTSCAN           = 0x76,   //����ɨ��
//    CMD_SJ_SMARTSCAN_END       = 0x77,   //����ɨ�����

    //���ݲ�������0xA0~0xBF
//    CMD_READ_POSCALI_DATA      = 0xA0,   //��ȡλ��У׼����
//    CMD_SET_POSCALI_DATA       = 0xA1,   //����У׼����
//    CMD_SAVE_POSCALI_DATA      = 0xA2,   //�洢У׼����
//    CMD_CLR_POSCALI_DATA       = 0xA3,   //���У׼����
//    CMD_ENTER_POSCAL           = 0xA4,   //����У׼
//    CMD_REC_CALDATA            = 0xA5,   //��¼У׼����
//    CMD_SAVE_CALDATA           = 0xA6,   //����У׼����
//    CMD_SET_HELM_PARAM         = 0xA7,   //���ö������
//    CMD_GET_HELM_PARAM         = 0xA8,   //��ȡ�������
//    CMD_SAVE_HELM_PARAM        = 0xA9,   //����������
//    CMD_SET_FORCE_CALI         = 0xAA,   //���ò���У׼����
//    CMD_GET_FORCE_CALI         = 0xAB,   //��ȡ����У׼����
//    CMD_SET_TMP_PARAM          = 0xAC,   //�����¶Ȳ���
//    CMD_GET_TMP_PARAM          = 0xAD,   //��ȡ�¶Ȳ���
//    CMD_SAVE_TMP_PARAM         = 0xAE,   //�����¶Ȳ���
//    CMD_READ_OPTSIG_DATA       = 0xB0,   //��ѯ��դ�ź�����
    CMD_READ_OPTPOS_DATA       = 0xB1,   //��ѯ��դλ������
//    CMD_READ_SINCOS_DATA       = 0xB2,   //��ȡ��դsin/cosֵ
    CMD_SAVE_RSTMECHPOS        = 0xB3,   //���渴λ���еλ��
//    CMD_SAVE_OPTZ0Range        = 0xB4,   //����Z0���˶���Χ
    
//    CMD_SET_POLEWEIGHT_DATA    = 0xB5,   //���ðڸ˲�������
//    CMD_GET_POLEWEIGHT_DATA    = 0xB6,   //��ȡ�ڸ˲�������
//    CMD_SAVE_POLEWEIGHT_DATA   = 0xB7,   //����ڸ˲�������
//    CMD_READ_PRSCALI_DATA      = 0xB5,   //��ȡѹ��У׼����
//    CMD_SET_PRSCALI_DATA       = 0xB6,   //����ѹ��У׼����
//    CMD_SAVE_PRSCALI_DATA      = 0xB7,   //�洢ѹ��У׼����
//    CMD_SET_PRSTHRE            = 0xB8,   //������������ѹ������
    CMD_SET_OPTMISCPARAM       = 0xB9,   //���ù�դ�ۺϲ���
//    CMD_SET_C1C2               = 0xBA,   //����C1C2��ֵ�����²��������������ߵľ���
//    CMD_SET_KEY_PARAM          = 0xBB,   //���ð�������
//    CMD_GET_KEY_PARAM          = 0xBC,   //��ȡ��������
//    CMD_SET_FORCE_PARAM        = 0xBD,   //����ˮƽ��������
//    CMD_GET_FORCE_PARAM        = 0xBE,   //��ȡˮƽ��������
//    CMD_SAVE_FORCE_PARAM       = 0xBF,   //����ˮƽ��������
    

    //����ģʽ����0xC0~0xFF
    CMD_SJ_TESTSTEP            = 0xC0,   //�̶���������
    CMD_SJ_WRITE_CAL           = 0xC1,   //����У׼
    CMD_SJ_READ_CAL            = 0xC2,   //��ȡУ׼
    CMD_BOARD_TEST             = 0xD3,   //��·�����
    CMD_UPDATE_FILE            = 0xD5,   //����ָ���ļ�
    CMD_READ_SPI_FLASH         = 0xD6,   //��ȡSPIFlash
    CMD_READ_NAND_FLASH        = 0xD7,   //��ȡNandFlash
    CMD_SJ_REPEAT_GO           = 0xD8,   //��դ�ظ��ƶ�����
//    CMD_READND280DATA          = 0xDB,   //��ȡND280��������
//    CMD_READ_SCREEN            = 0xF0,   //��ȡ��Ļ����
//    CMD_USB_TEST               = 0xF1,   //USB�������
//    CMD_VALSE_CTRL             = 0xF2,   //���ſ���
    CMD_PRINT_TASK_DATA        = 0xF3,   //��ӡ��������
//    CMD_HELM_ANGLE_CTRL        = 0xF4,   //����Ƕȿ���
//    CMD_OPTRPT_TEST            = 0xF5,   //��դ����ظ��Բ���
//    CMD_QUICK_RESET            = 0xF6,   //���ٸ�λ
//    CMD_SET_ERR_MASK           = 0xF7,   //���ô�������λ
//    CMD_SET_HELM_RSTANGLE      = 0xF8,   //���ö����λʱ�ĽǶ�ֵ
//    CMD_OPTY_RESET             = 0xF9,   //y�Ḵλ
//    CMD_GET_PROBE_DATA         = 0xFE,   //��ȡ��ͷ����
}CMDCODE;

#endif






























