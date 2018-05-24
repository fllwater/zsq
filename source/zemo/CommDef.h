/*****************************************************************************
开发公司： 深圳中图仪器科技有限公司 Copyright(c)@2010
功能描述:
    1.通信任务
    2.串口通信协议
    3.执行用户命令
历史记录:
    <作者>     <时间>       <描述>
     zhj      2009-12-26     创建
******************************************************************************
*/
#ifndef _COMM_DEF_H_
#define _COMM_DEF_H_

//通讯端口定义
typedef enum
{
    DBG_PORT = 0, //系统维护和调试通信
    CON_PORT = 1, //PC控制台通信
//    KEY_PORT = 2, //KEY端口  
//    IR_PORT  = 3, //IR端口
    UART_NUM      //串口数目
}COMMNR;

//------------------------------------------------------------------------
//通信协议(含串口收发,PC->USB通信)
//1.帧格式：[前导符(1Byte)][命令码(1Byte)][帧内容(9Bytes)][校验和(1Byte)]
//2.校验和是指“命令码，帧内容”所有字节的校验数
//3.帧总长固定为12Bytes
//--------------------------------------------------------------------------
#define COMM_FRM_LEN    (12)
#define COMM_IR_LEN     (4)
#define COMM_FRM_HEAD   (0x53)


//------------------------------------------------------------------------
//USB发送包通信协议(USB->PC通信)
//帧格式：[前导符][帧内容(6路光栅×位置(4Bytes))][校验和(1Byte)]
//------------------------------------------------------------------------
//#define USB_OPTFRM_LEN      (18)
//#define UDP_OPTFRM_LEN      (18)
//#define USB_SEND_BUF_LEN    (360)
#define UDP_SEND_DATA_LEN   (900)
//#define USB_PACK_NUM		(20)
//#define UDP_PACK_NUM        (50)

//通信命令码(含串口收发,PC->USB通信)
typedef enum
{
    CMD_INVALID                = 0x00,   //无效命令

    //通用命令0x10~0x2F
    CMD_HD_VER                 = 0x10,   //获取硬件版本
    CMD_SW_VER                 = 0x11,   //获取软件版本
    CMD_SET_BEEP               = 0x12,   //蜂鸣
    CMD_DEVRST                 = 0x15,   //设备复位
    CMD_DEVRST_END             = 0x16,   //设备复位
    CMD_ERR_DBGPRINT           = 0x17,   //系统调试打印开关
    CMD_ERR_RPT                = 0x18,   //系统错误状态报告
    CMD_READ_DEVPARA           = 0x20,   //获取设备参数
    CMD_SET_DEVSN              = 0x27,   //设置设备编号
    CMD_GET_DEVSN              = 0x28,   //获取设备编号
    CMD_RS232_EN               = 0x2F,   //RS232使能开关

    //控制命令0x30~0x5F
    CMD_SET_WORKMODE           = 0x30,   //设置设备工作模式
    CMD_SJ_ZERO                = 0x35,   //光栅清零
    CMD_SJ_GO                  = 0x36,   //光栅移动
    CMD_SJ_GO_END              = 0x37,
    CMD_SJ_GODIST              = 0x38,   //光栅移动距离
    CMD_SJ_GODIST_END          = 0x39,
    CMD_SJ_GOTOZERO            = 0x3A,   //光栅移动到零点
    CMD_SJ_GOTOZERO_END        = 0x3B,
    CMD_SJ_GO_STOP             = 0x3C,   //光栅移动停止
    CMD_SJ_GO_STOP_END         = 0x3D,
//    CMD_SJ_OPTXXX              = 0x3E,   //电机移动距离
//    CMD_SJ_OPTXXX_END          = 0x3F,   //电机移动距离
    CMD_SJ_GETDATA             = 0x40,   //采样光栅示值
//    CMD_SJ_TEST_TASK           = 0x55,   //OPT测试任务
    CMD_SJ_MOTOR_GO            = 0x56,   //电机运行
//    CMD_HELM_CTRL              = 0x57,   //舵机控制
//    CMD_HELM_CTRL_END          = 0x58,   //舵机控制
//    CMD_USB_CONNECT            = 0x59,   //USB连接控制
//    CMD_USB_CONNECT_END        = 0x5A,   //USB连接控制完成
//    CMD_SET_ETH_IP             = 0x5B,   //设置ip地址
//    CMD_GET_ETH_IP             = 0x5C,   //获取ip地址
//    CMD_SET_ETH_MAC            = 0x5D,   //设置MAC地址
//    CMD_GET_ETH_MAC            = 0x5E,   //获取MAC地址
//    CMD_GET_MISCINFO           = 0x61,   //获取设备综合信息
//    CMD_KEY_OK                 = 0x63,   //按键盒按下OK键
    CMD_RST_OPT                = 0x64,   //复位光栅值
    CMD_RST_OPT_END            = 0x65,   //复位光栅值
//    CMD_XZ_SCAN                = 0x66,   //x,z联动扫描
//    CMD_XZ_SCAN_END            = 0x67,   //x,z联动扫描

//    CMD_SETSCAN_PARAM          = 0x69,   //设置扫描参数
//    CMD_SJ_TOUCH               = 0x70,   //运行电机直到接触工件
//    CMD_SJ_TOUCH_END           = 0x71,   //运行电机直到接触工件
//   
//    CMD_SJ_SMARTSCAN           = 0x76,   //智能扫描
//    CMD_SJ_SMARTSCAN_END       = 0x77,   //智能扫描结束

    //数据操作命令0xA0~0xBF
//    CMD_READ_POSCALI_DATA      = 0xA0,   //读取位置校准数据
//    CMD_SET_POSCALI_DATA       = 0xA1,   //设置校准数据
//    CMD_SAVE_POSCALI_DATA      = 0xA2,   //存储校准数据
//    CMD_CLR_POSCALI_DATA       = 0xA3,   //清除校准数据
//    CMD_ENTER_POSCAL           = 0xA4,   //进入校准
//    CMD_REC_CALDATA            = 0xA5,   //记录校准数据
//    CMD_SAVE_CALDATA           = 0xA6,   //保存校准数据
//    CMD_SET_HELM_PARAM         = 0xA7,   //设置舵机参数
//    CMD_GET_HELM_PARAM         = 0xA8,   //读取舵机参数
//    CMD_SAVE_HELM_PARAM        = 0xA9,   //保存舵机参数
//    CMD_SET_FORCE_CALI         = 0xAA,   //设置测力校准数据
//    CMD_GET_FORCE_CALI         = 0xAB,   //读取测力校准数据
//    CMD_SET_TMP_PARAM          = 0xAC,   //设置温度参数
//    CMD_GET_TMP_PARAM          = 0xAD,   //获取温度参数
//    CMD_SAVE_TMP_PARAM         = 0xAE,   //保存温度参数
//    CMD_READ_OPTSIG_DATA       = 0xB0,   //查询光栅信号数据
    CMD_READ_OPTPOS_DATA       = 0xB1,   //查询光栅位置数据
//    CMD_READ_SINCOS_DATA       = 0xB2,   //读取光栅sin/cos值
    CMD_SAVE_RSTMECHPOS        = 0xB3,   //保存复位后机械位置
//    CMD_SAVE_OPTZ0Range        = 0xB4,   //保存Z0的运动范围
    
//    CMD_SET_POLEWEIGHT_DATA    = 0xB5,   //设置摆杆测力数据
//    CMD_GET_POLEWEIGHT_DATA    = 0xB6,   //获取摆杆测力数据
//    CMD_SAVE_POLEWEIGHT_DATA   = 0xB7,   //保存摆杆测力数据
//    CMD_READ_PRSCALI_DATA      = 0xB5,   //读取压力校准数据
//    CMD_SET_PRSCALI_DATA       = 0xB6,   //设置压力校准数据
//    CMD_SAVE_PRSCALI_DATA      = 0xB7,   //存储压力校准数据
//    CMD_SET_PRSTHRE            = 0xB8,   //设置正常工作压力门限
    CMD_SET_OPTMISCPARAM       = 0xB9,   //设置光栅综合参数
//    CMD_SET_C1C2               = 0xBA,   //设置C1C2数值：上下测针针尖相对于中线的距离
//    CMD_SET_KEY_PARAM          = 0xBB,   //设置按键参数
//    CMD_GET_KEY_PARAM          = 0xBC,   //读取按键参数
//    CMD_SET_FORCE_PARAM        = 0xBD,   //设置水平测力参数
//    CMD_GET_FORCE_PARAM        = 0xBE,   //读取水平测力参数
//    CMD_SAVE_FORCE_PARAM       = 0xBF,   //保存水平测力参数
    

    //工程模式命令0xC0~0xFF
    CMD_SJ_TESTSTEP            = 0xC0,   //固定步长测试
    CMD_SJ_WRITE_CAL           = 0xC1,   //设置校准
    CMD_SJ_READ_CAL            = 0xC2,   //读取校准
    CMD_BOARD_TEST             = 0xD3,   //电路板测试
    CMD_UPDATE_FILE            = 0xD5,   //升级指定文件
    CMD_READ_SPI_FLASH         = 0xD6,   //读取SPIFlash
    CMD_READ_NAND_FLASH        = 0xD7,   //读取NandFlash
    CMD_SJ_REPEAT_GO           = 0xD8,   //光栅重复移动测试
//    CMD_READND280DATA          = 0xDB,   //获取ND280测试数据
//    CMD_READ_SCREEN            = 0xF0,   //读取屏幕数据
//    CMD_USB_TEST               = 0xF1,   //USB传输测试
//    CMD_VALSE_CTRL             = 0xF2,   //阀门控制
    CMD_PRINT_TASK_DATA        = 0xF3,   //打印任务数据
//    CMD_HELM_ANGLE_CTRL        = 0xF4,   //舵机角度控制
//    CMD_OPTRPT_TEST            = 0xF5,   //光栅零点重复性测试
//    CMD_QUICK_RESET            = 0xF6,   //快速复位
//    CMD_SET_ERR_MASK           = 0xF7,   //设置错误屏蔽位
//    CMD_SET_HELM_RSTANGLE      = 0xF8,   //设置舵机复位时的角度值
//    CMD_OPTY_RESET             = 0xF9,   //y轴复位
//    CMD_GET_PROBE_DATA         = 0xFE,   //获取测头数据
}CMDCODE;

#endif






























