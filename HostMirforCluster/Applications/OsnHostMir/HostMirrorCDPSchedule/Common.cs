using System;
using System.Collections.Generic;
using System.Text;

namespace HostMirrorCDPSchedule
{

    [Serializable]
    public enum OPStatus
    {
        OP_SUCCESS = 0x0,//�ɹ�
        OP_FAILED = 0x01 | 0x20000000,//ʧ��
        OP_FAILED_DELETE_TARGET_ITEM = 0x02 | 0x20000000,//ɾ������������������Ϣʧ��
        OP_FAILED_WRITE_TARGET_ITEM = 0x03 | 0x20000000,//д����������������Ϣʧ��
        OP_DELETE_POOL_WITH_VOLUME = 0x1001 | 0x20000000,//ɾ���о��pool
        OP_EXTEND_POOL_DISK_NOT_FOUND = 0x1002 | 0x20000000,//����δ�ҵ�
        OP_EXTEND_POOL_UNPERMISSION = 0x1003 | 0x20000000,//�Ѵ�������������������������pool
        OP_FALED_GET_POOL_ALLOWED_CAPACITY = 0x1004 | 0x20000000,//��ȡ��������pool����ʧ��
        OP_POOL_NOT_FOUND = 0x1005 | 0x20000000,//poolδ�ҵ�
        OP_SHRINK_POOL_DISK_NOT_FOUND = 0x1006 | 0x20000000,//pool�д���δ�ҵ�
        OP_POOL_EXCEED_MAXIMUM_MEMBER = 0x1007 | 0x20000000,//����pool�����Ա��
        OP_POOL_WITH_CONFLICT_NAME = 0x1008 | 0x20000000,//�洢�����Ƴ�ͻ
        OP_POOL_WITH_CONFLICT_ID = 0x1009 | 0x20000000,//�洢��ID��ͻ
        OP_POOL_EXCEED_MAXIMUM_NUMBER = 0x100a | 0x20000000,//����pool������
        OP_VOLUME_CREATE_CAPACITY_NOT_ENOUGH = 0x2001 | 0x20000000,//�½����������ڿ������������2T
        OP_DELETE_VOLUME_WITH_CDP = 0x2002 | 0x20000000,//����ɾ���ѽ���CDP���յľ�
        OP_DELETE_VOLUME_USED = 0x2003 | 0x20000000,//����ɾ����ʹ�õľ�
        OP_DELETE_VOLUME_NOT_FOUND = 0x2004 | 0x20000000,//��ɾ���ľ�δ�ҵ�
        OP_FAILED_DELETE_VOLUME_CONFIG_ITEM = 0x2005 | 0x20000000,//ɾ����������Ϣʧ��
        OP_CREATE_VOLUME_WITH_CONFLICT_NAME = 0x2006 | 0x20000000,//������ͻ
        OP_LOOP_VOLUME_CHECK_FAILED = 0x2007 | 0x20000000,//����Ƿ���loopʧ��
        OP_LOOP_VOLUME_REFUSED = 0x2008 | 0x20000000,//������ĳЩ��;ʱ��Ӧloopback
        OP_LOOP_VOLUME_FAILED_WTRITE_ITEM = 0x2009 | 0x20000000,//д���loopback��Ϣʧ��
        OP_LOOP_VOLUME_DELETE_REFUSED = 0x200a | 0x20000000,//������ĳЩ��;ʱ��Ӧɾ��loopback
        OP_LOOP_VOLUME_FAILED_DEL_ITEM = 0x200b | 0x20000000,//ɾ�����loopback��Ϣʧ��
        OP_RESIZE_VOLUME_CAPACITY_NOT_ENOUGH = 0x200c | 0x20000000,//���������ڿ������������2T
        OP_RESIZE_VOLUME_USED = 0x200d | 0x20000000,//����resize��ʹ�õľ�
        OP_RESIZE_VOLUME_NOT_FOUND = 0x200e | 0x20000000,//��δ�ҵ�
        OP_RESIZE_VOLUME_REFUSED = 0x200f | 0x20000000,//�ܾ���������С��ԭ����С
        OP_RESIZE_VOLUME_FAIED_UPDATE_ITEM = 0x2010 | 0x20000000,//���¾�������Ϣʧ��
        OP_DELETE_VOLUME_WITH_SINGLE_CDP_CONF = 0x2011 | 0x20000000,//Ӧ��ɾ������CDP����
        OP_VOLUME_NOT_FOUND = 0x2012 | 0x20000000,//��δ�ҵ�
        OP_TARGET_ITEM_NOT_FOUND = 0x2013 | 0x20000000,//������������Ϣδ�ҵ�
        OP_PARTITION_NOT_FOUND = 0x2014 | 0x20000000,
        OP_CREATE_VOLUME_WITH_INVALID_NAME = 0x2015 | 0x20000000,

        OP_CREATE_STORAGE_SERVER_WITH_INVALID_NAME = 0x3001 | 0x20000000,//�洢Ӧ�÷�����������Ч
        OP_CREATE_STORAGE_SERVER_WITH_CONFLICT_NAME = 0x3002 | 0x20000000,//Ӧ�÷��������Ƴ�ͻ
        OP_CREATE_STORAGE_SERVER_CAPACITY_NOT_ALLOWED = 0x3003 | 0x20000000,//�Ѵ���������Ӧ�÷���������
        OP_CREATE_STORAGE_SERVER_WITH_INVALID_OS_TYPE = 0x3004 | 0x20000000,//��Ч�Ĵ洢Ӧ�÷�����ϵͳ����
        OP_CREATE_STORAGE_SERVER_FAILED_WTRITE_ITEM = 0x3005 | 0x20000000,//д�洢Ӧ�÷���������������Ϣʧ��
        OP_DELETE_STORAGE_SERVER_NOT_FOUND = 0x3006 | 0x20000000,//δ�ҵ��洢Ӧ�÷�����
        OP_DELETE_STORAGE_SERVER_WITH_MAPPING = 0x3007 | 0x20000000,//����ɾ�����о�ӳ��Ĵ洢Ӧ�÷�����
        OP_DELETE_STORAGE_SERVER_WITH_INITIATOR_CHANNEL = 0x3008 | 0x20000000,//����ɾ���пͻ���ͨ���Ĵ洢Ӧ�÷�����
        OP_DELETE_STORAGE_SERVER_FAILED_DEL_ITEM = 0x3009 | 0x20000000,//ɾ���洢Ӧ�÷���������������Ϣʧ��
        OP_CREATE_BACKUP_SERVER_WITH_INVALID_NAME = 0x300a | 0x20000000,//�洢���ݷ�����������Ч
        OP_CREATE_BACKUP_SERVER_WITH_CONFLICT = 0x300b | 0x20000000,//Ӧ�÷��������ƻ�ͻ���WWN��ͻ
        OP_CREATE_BACKUP_SERVER_CAPACITY_NOT_ALLOWED = 0x300c | 0x20000000,//�Ѵ���������Ӧ�÷���������
        OP_CREATE_BACKUP_SERVER_WITH_INVALID_CONFIG = 0x300d | 0x20000000,//��Ч�ı���Ӧ�÷���������
        OP_CREATE_BACKUP_SERVER_FAILED_WTRITE_ITEM = 0x300e | 0x20000000,//д����Ӧ�÷���������������Ϣʧ��
        OP_DELETE_BACKUP_SERVER_NOT_FOUND = 0x300f | 0x20000000,//δ�ҵ��洢���ݷ�����
        OP_DELETE_BACKUP_SERVER_WITH_RELATION = 0x3010 | 0x20000000,//����ɾ�����и��ƹ�ϵ�ı���Ӧ�÷�����
        OP_DELETE_BACKUP_SERVER_FALED_GET_RELATION = 0x3011 | 0x20000000,//��ȡRS������Ϣʧ��
        OP_DELETE_BACKUP_SERVER_FAILED_DEL_ITEM = 0x3012 | 0x20000000,//ɾ������Ӧ�÷���������������Ϣʧ��
        OP_STORAGE_SERVER_ADD_CHANNEL_IN_OTHER = 0x3013 | 0x20000000,//��������Ѽ��������洢Ӧ�÷�������ͨ��
        OP_STORAGE_SERVER_ADD_CHANNEL_NOT_FOUND = 0x3014 | 0x20000000,//δ�ҵ�ָ��ͨ������ϸ��Ϣ
        OP_STORAGE_SERVER_ADD_CHANNEL_FAILED_WRITE_ITEM = 0x3015 | 0x20000000,//���ͨ��д����������Ϣʧ��
        OP_STORAGE_SERVER_REMOVE_CHANNEL_NOT_FOUND = 0x3016 | 0x20000000,//ɾ����ͨ��δ�ӱ���������Ϣ�ҵ�
        OP_STORAGE_SERVER_REMOVE_CHANNEL_WITH_MAPPING = 0x3017 | 0x20000000,//����ɾ�����þ�ӳ���ͨ��
        OP_STORAGE_SERVER_REMOVE_CHANNEL_FAILED_DEL_ITEM = 0x3018 | 0x20000000,//ɾ��ͨ������������Ϣʧ��
        OP_BACKUP_SERVER_FAILED_GET_IPADDRESS = 0x3019 | 0x20000000,//��ȡ���ݷ�������IP��ַʧ��
        OP_BACKUP_SERVER_TARGET_NOT_FOUND = 0x301a | 0x20000000,//���ݷ�������Target��Ϣδ�ҵ�
        OP_BACKUP_SERVER_TARGET_EXSITED = 0x301b | 0x20000000,//���ݷ�������Target�Ѿ�����
        OP_RSBACKUP_SOURCE_NOT_FOUND = 0x301c | 0x20000000,//�洢Դδ�ҵ�
        OP_RSBACKUP_EXSITED = 0x301d | 0x20000000,//RS���ݹ�ϵ�ѽ���
        OP_RSBACKUP_NOT_FOUND = 0x301e | 0x20000000,//RS���ݹ�ϵ��Ч
        OP_RSBACKUP_MODE_CHANGE_WITH_RELATION = 0x301f | 0x20000000,//RS���ݹ�ϵ���ڵ�������޸�ģʽ

        OP_COMMON_MAPPING_SUCCESS_WITH_DIFFER_PAIR = 0x4000 | 0x20000000,//ӳ��ɹ�������ͨ�������Ͳ�ͬ
        OP_COMMON_MAPPING_CHANNEL_NOT_MATCHING = 0x4001 | 0x20000000,//ͨ����δ����
        OP_COMMON_MAPPING_EXSITED = 0x4002 | 0x20000000,//��ͬ��ӳ�������Ѵ��ڣ������ٴ�����
        OP_COMMON_MAPPING_VOLUME_USED = 0x4003 | 0x20000000,//���ܶ���ʹ�õľ�����ӳ��
        OP_COMMON_MAPPING_VOLUME_NOT_FOUND = 0x4004 | 0x20000000,//����ӳ��ľ�δ�ҵ�
        OP_COMMON_MAPPING_FAILED_WRITE_ITEM = 0x4005 | 0x20000000,//д��ӳ��������Ϣʧ��
        OP_COMMON_MAPPING_FAILED_GET_LOCAL_WWN = 0x4006 | 0x20000000,//��ȡ�洢��������WWNʧ��
        OP_DELETE_COMMON_MAPPING_FAILED_DEL_ITEM = 0x4007 | 0x20000000,//ɾ����ӳ��������Ϣʧ��
        OP_COMMON_MAPPING_TC_NOT_LOGINED = 0x4008 | 0x20000000,//ӳ���Ӧ�ķ�����ͨ��δ����
        OP_COMMON_MAPPING_NOT_FOUND = 0x4009 | 0x20000000,//ӳ��������Ϣδ�ҵ�
        OP_COMMON_MAPPING_LUN_USED = 0x400a | 0x20000000,//LUN�ѱ�ʹ��
        OP_COMMON_MAPPING_IC_NOT_LOGINED = 0x400b | 0x20000000,//ӳ���Ӧ�Ŀͻ���ͨ��δ����
        OP_COMMON_MAPPING_CHANNEL_INDEX_NOT_FOUND = 0x400c | 0x20000000,//��ȡͨ������ʧ��
        OP_REMOTE_MAPPING_CHANNEL_INDEX_NOT_FOUND = 0x400d | 0x20000000,//��ȡͨ������ʧ��
        OP_DELETE_REMOTE_MAPPING_FAILED_DEL_ITEM = 0x400e | 0x20000000,//ɾ��ӳ��������Ϣʧ��
        OP_REMOTE_MAPPING_EXSITED = 0x400f | 0x20000000,//��ͬ��ӳ�������Ѵ���
        OP_REMOTE_MAPPING_FAILED_WRITE_ITEM = 0x4010 | 0x20000000,//д��ӳ��������Ϣʧ��
        OP_REMOTE_MAPPING_FAILED_GET_SERVER_WWN = 0x4011 | 0x20000000,//��ȡ�洢��������WWNʧ��
        OP_REMOTE_MAPPING_NOT_FOUND = 0x4012 | 0x20000000,//Զ�̷�������ӳ��������Ϣδ�ҵ�

        OP_CREATE_SNAPSHOT_CAPACITY_UNPERMISSION = 0x5001 | 0x20000000,//�Կ���Դ�����Ŀ��չ�ϵ�Ѵ�������
        OP_CREATE_SNAPSHOT_FAILED_WRITE_ITEM = 0x5002 | 0x20000000,//д���չ�ϵ����������Ϣʧ��
        OP_SNAPSHOT_NOT_FOUND = 0x5003 | 0x20000000,//���չ�ϵδ�ҵ�
        OP_DELETE_SNAPSHOT_FAILED_DEL_ITEM = 0x5004 | 0x20000000,//ɾ�����չ�ϵ����������Ϣʧ��
        OP_SNAPSHOT_ACTIVED = 0x5005 | 0x20000000,//���չ�ϵ�Ѽ���

        OP_CDP_MODE_SOURCE_VOLUME_NOT_FOUND = 0x6001 | 0x20000000,//δ�ҵ�CDP���ձ����
        OP_CDP_MODE_SOURCE_VOLUME_TOO_SMALL = 0x6002 | 0x20000000,//CDP���ձ��������̫С,���ܽ���ģʽ����
        OP_CDP_MODE_MAKEING_VOLUME_USED = 0x6003 | 0x20000000,//���ܽ���ʹ�õľ�����ΪCDP���ձ����
        OP_CDP_MODE_REMOVE_WITH_SNAPSHOT = 0x6004 | 0x20000000,//����ɾ������CDP���յ�CDPģʽ����
        OP_CDP_MODE_FAILED_DEL_ITEM = 0x6005 | 0x20000000,//ɾ��CDPģʽ����������Ϣʧ��
        OP_CDP_MODE_FAILED_ACCESS_REGISTRY = 0x6006 | 0x20000000,//����CDPģʽ����ע�����ʧ��
        OP_CDP_FAILED_GET_CDP_COUNT = 0x6007 | 0x20000000,//��ȡCDP���ո���ʧ��
        OP_CDP_MODE_NOT_MAKE_SETTING = 0x6008 | 0x20000000,//CDPģʽδ����
        OP_CDP_SOURCE_VOLUME_SPACE_NOT_ENOUGH = 0x6009 | 0x20000000,//CDP���ձ������������
        OP_CDP_CAPATICY_UNPERMISSION = 0x600a | 0x20000000,//�Ѵ�CDP����������
        OP_CDP_RESTORE_FAILED_CREATE_NEW = 0x600b | 0x20000000,//�ָ�ʱ���½�CDP����ʧ��
        OP_CDP_RESTORE_FAILED_CREATE_SNAPSHOT = 0x600c | 0x20000000,//�ָ�ʱ,�������洢�����չ�ϵʧ��
        OP_CDP_RESTORE_FAILED_ENABLE_SNAPSHOT = 0x600d | 0x20000000,//�ָ�ʱ,�������洢�����չ�ϵʧ��
        OP_BACKUP_MODE_CHANGE_WITH_RELATION = 0x7001 | 0x20000000,//�б��ݹ�ϵ����ʱ���ܽ�����ģʽ�޸�Ϊͬ��ģʽ
        OP_BACKUP_MODE_VOLUME_NOT_FOUND = 0x7002 | 0x20000000,//�첽����ģʽ���õĴ洢��δ�ҵ�
        OP_BACKUP_MODE_MAKE_VOLUME_USED = 0x7003 | 0x20000000,//���ܽ���ʹ�õľ�����Ϊ�첽ģʽ�����
        OP_BACKUP_FAILED_VERIFY_INITIATOR = 0x7004 | 0x20000000,//OSNAPIVerifyInitiator����ʧ��
        OP_BACKUP_BAKCFG_NOT_FOUND = 0x7005 | 0x20000000,//�ļ�OSNServerBackup.cfgδ�ҵ�
        OP_BACKUP_SERVER_NOT_FOUND = 0x7006 | 0x20000000,//δ�ӱ��������ļ��ҵ����ݷ�������Ϣ
        OP_BACKUP_FAILED_ACCESS_REGISTRY = 0x7007 | 0x20000000,//����������ݹ�ϵʱ����ע���ʧ��
        OP_BACKUP_FAILED_GET_STATUS = 0x7008 | 0x20000000,//��ȡ����״̬ʧ��
        OP_DELETE_BACKUP_STATE_UNPERMISSION = 0x7009 | 0x20000000,//���ݹ�ϵ�ǳ�ʼ����ָ�״̬ʱ���ܱ�ɾ��
        OP_BACKUP_FAILED_WRITE_ITEM = 0x700a | 0x20000000,//д���ر��ݹ�ϵ������Ϣʧ��
        OP_BACKUP_FAILED_DEL_ITEM = 0x700b | 0x20000000,//ɾ�����ر��ݹ�ϵ������Ϣʧ��
        OP_BACKUP_NOT_FOUND = 0x700c | 0x20000000,//���ݹ�ϵδ�ҵ�
        OP_MIRROR_FAILED_GET_SERVER_WWN = 0x8001 | 0x20000000,//��ȡ��������WWNʧ��
        OP_MIRROR_FAILED_CREATE_MIRRORCLIENT = 0x8002 | 0x20000000,//����MirrorClient�洢Ӧ�÷�����ʧ��
        OP_MIRROR_SERVER_NOT_FOUND = 0x8003 | 0x20000000,//�����������Ϣδ�ܴӱ���������Ϣ�ҵ�
        OP_MIRROR_CHANNEL_NOT_LINKED = 0x8004 | 0x20000000,//ͨ��δ��������
        OP_MIRROR_FAILED_WRITE_CHANNEL_ITEM = 0x8005 | 0x20000000,//дmirror��initiator channel������Ϣʧ��
        OP_MIRROR_FAILED_WRITE_MAPPING_ITEM = 0x8006 | 0x20000000,//дmirror�ı���mapping������Ϣʧ��
        OP_MIRROR_FAILED_WRITE_MIRROR_ITEM = 0x8007 | 0x20000000,//дmirror�ı���������Ϣʧ��
        OP_MIRROR_FAILED_SET_PRIMARY = 0x8008 | 0x20000000,//������������������mirrorʧ��
        OP_MIRROR_FAILED_CONFIG_REGISTRY = 0x8009 | 0x20000000,//�޸�mirror��ע�����ʧ��
        OP_MIRROR_FAILED_MAPPING_VOLUME = 0x800a | 0x20000000,//����mirror mappingʧ��
        OP_MIRROR_FAILED_DEL_MAPPING_VOLUME = 0x800b | 0x20000000,//ɾ��mirror mappingʧ��
        OP_MIRROR_FAILED_SET_SUBSIDIARY = 0x800c | 0x20000000,//����ӷ�����������mirrorʧ��
        OP_MIRROR_FAILED_GET_MIRROR_ITEM = 0x800d | 0x20000000,//����������Ϣδ�ӱ��������ļ����
        OP_MIRROR_FAILED_GET_STATUS = 0x800e | 0x20000000,//��ȡ����״̬ʧ��
        OP_MIRROR_DELETE_UNPERMISSION = 0x800f | 0x20000000,//����״̬Ϊrecovery,initialize,devicetestingʱ������ɾ��
        OP_MIRROR_DELETE_WITH_COMMON_MAPPING = 0x8010 | 0x20000000,//�����ӳ�䣬����ɾ��
        OP_MIRROR_FAILED_DEL_MAPPING_ITEM = 0x8011 | 0x20000000,//ɾ��mirror�ı���mapping������Ϣʧ��
        OP_MIRROR_FAILED_DEL_MIRROR_ITEM = 0x8012 | 0x20000000,//ɾ��mirror�ı���������Ϣʧ��
        OP_MIRROR_INITIALIZING_OR_RECOVERYING = 0x8013 | 0x20000000,//�������ڳ�ʼ����ָ���
        OP_MIRROR_NOT_FOUND_IN_DRIVER = 0x8014 | 0x20000000,//δ�ҵ�����

        OP_NAS_INVALID_HOST_GROUP_NAME = 0x9001 | 0x20000000,//����������Ч
        OP_NAS_HOST_GROUP_NAME_CONFLICT = 0x9002 | 0x20000000,//����������ͻ
        OP_NAS_HOST_GROUP_NOT_FOUND = 0x9003 | 0x20000000,//������δ�ҵ�
        OP_NAS_HOST_GROUP_MEMBER_NOT_FOUND = 0x9004 | 0x20000000,//ָ�����������Աδ�ҵ�
        OP_NAS_HOST_GROUP_MEMBER_CONFLICT = 0x9005 | 0x20000000,//�������Ա��ͻ
        OP_NAS_INVALID_HOST_GROUP_MEMBER = 0x9006 | 0x20000000,//�������Ա��Ч
        OP_NAS_DELETE_HOST_GROUP_WITH_MEMBER = 0x9007 | 0x20000000,//����ɾ������������Ա����
        OP_NAS_INVALID_USER_GROUP_NAME = 0x9008 | 0x20000000,//�û�������Ч
        OP_NAS_USER_GROUP_NAME_CONFLICT = 0x9009 | 0x20000000,//�û�������ͻ
        OP_NAS_USER_GROUP_ID_CONFLICT = 0x900a | 0x20000000,////�û���ID��ͻ
        OP_NAS_USER_GROUP_NOT_FOUND = 0x900b | 0x20000000,//�û���δ�ҵ�
        OP_NAS_DELETE_USER_GROUP_WITH_MEMBER = 0x900c | 0x20000000,//����ɾ�������û���Ա����
        OP_NAS_USER_GROUP_MEMBER_NOT_FOUND = 0x900d | 0x20000000,//ָ�����û����Աδ�ҵ�
        OP_NAS_INVALID_USER_GROUP_MEMBER_NAME = 0x900e | 0x20000000,//�û����Ա����Ч
        OP_NAS_USER_GROUP_MEMBER_NAME_CONFLICT = 0x900f | 0x20000000,//�û����Ա���Ƴ�ͻ
        OP_NAS_USER_GROUP_MEMBER_ID_CONFLICT = 0x9010 | 0x20000000,//�û����ԱID��ͻ
        OP_NAS_FAILED_ADD_DOMAIN_MAP = 0x9011 | 0x20000000,//adddomainmap -dʧ��
        OP_NAS_USER_MAPPING_EXSITED = 0x9012 | 0x20000000,//�û�ӳ�������Ѵ���
        OP_NAS_FAILED_SET_PRIMARY_MAPPING = 0x9013 | 0x20000000,//������ӳ��ʧ��
        OP_NAS_DELETE_SIMPLE_NAME_MAPPING = 0x9014 | 0x20000000,//Ӧͨ��ȡ��������ӳ��ɾ��
        OP_NAS_USER_NAME_MAPPING_NOT_FOUND = 0x9015 | 0x20000000,//����ӳ������δ�ҵ�
        OP_NAS_DELETE_PRIMARY_USER_NAME_MAPPING = 0x9016 | 0x20000000,//��UNIX�û����ԣ����������ӳ�����ã���ӳ�䲻��ɾ��
        OP_NAS_REPEAT_SET_PRIMARY_USER_NAME_MAPPING = 0x9017 | 0x20000000,//�ظ�������ӳ��
        OP_NAS_FAILED_GET_IMAGE_PATH = 0x9018 | 0x20000000,//��ע����ȡimagepathʧ��
        OP_NAS_VG_NOT_FOUND = 0x9019 | 0x20000000,//����δ�ҵ�
        OP_NAS_VG_NOT_CONFIGED = 0x901a | 0x20000000,//����δ����(��ʼ�����á��ļ�ϵͳ�ȣ�
        OP_NAS_VG_SHARE_EXSITED = 0x901b | 0x20000000,//����������NAS����
        OP_NAS_VG_CIFS_SHARE_NAME_CONFLICT = 0x901c | 0x20000000,//CIFS��������ͻ
        OP_NAS_VG_NFS_SHARE_NAME_CONFLICT = 0x901d | 0x20000000,//NFS��������ͻ
        OP_NAS_VG_FAILED_MAKE_CIFS_SHARE = 0x901e | 0x20000000,//����CIFS����ʧ��
        OP_NAS_VG_FAILED_MAKE_NFS_SHARE = 0x901f | 0x20000000,//����NFS����ʧ��
        OP_NAS_VG_INVALID_NAS_SHARE = 0x9020 | 0x20000000,//��Ч�ľ���NAS����
        OP_NAS_VG_CIFS_SHARE_NAME_EXSITED = 0x9021 | 0x20000000,//CIFS����������
        OP_NAS_VG_INVALID_CIFS_SHARE_NAME = 0x9022 | 0x20000000,//CIFS��������Ч
        OP_NAS_VG_FAILED_DEL_CIFS_SHARE = 0x9023 | 0x20000000,//ɾ��CIFS����ʧ��
        OP_NAS_VG_FAILED_DEL_NFS_SHARE = 0x9024 | 0x20000000,//ɾ��NFS����ʧ��
        OP_NAS_VG_NFS_SHARE_NAME_EXSITED = 0x9025 | 0x20000000,//NFS����������
        OP_NAS_VG_INVALID_NFS_SHARE_NAME = 0x9026 | 0x20000000,//NFS��������Ч
        OP_NAS_VG_INVALID_SYSTEM_ACCOUNT = 0x9027 | 0x20000000,//��Ч��ϵͳ�û�����
        OP_NAS_VG_INVALID_NAME = 0x9028 | 0x20000000,//����������Ч
        OP_NAS_VG_NAME_CONFLICT = 0x9029 | 0x20000000,//��������ͻ
        OP_NAS_DELETE_CONFIGED_VG = 0x902a | 0x20000000,//����ɾ�������õľ���
        OP_NAS_DELETE_VG_WITH_MEMBER = 0x902b | 0x20000000,//Ӧ��ɾ�������Ա
        OP_NAS_EXTEND_VG_EXCEED_MAX_NUMBER = 0x902c | 0x20000000,//��������32�������Ա
        OP_NAS_UNPERMIT_EXTEND_CONFIGED_STRIPE_VG = 0x902d | 0x20000000,//���������������õ�stripe���;���
        OP_NAS_VG_FAILED_LOOPBACK_VOLUME = 0x902e | 0x20000000,//loopback��ʧ��
        OP_NAS_VG_FAILED_GET_DISK_INDEX = 0x902f | 0x20000000,//��ȡ��loopback�ľ���ϵͳ�еĴ�������ʧ��
        OP_NAS_VG_FAILED_INITIALIZE_DISK = 0x9030 | 0x20000000,//��ʼ�����̣�ת���ɶ�̬���̣�ʧ��
        OP_NAS_VG_FAILED_GET_DISK_LABEL = 0x9031 | 0x20000000,//��ȡ���̵�labelʧ��
        OP_NAS_UNPERMIT_SHRINK_CONFIGED_VG = 0x9032 | 0x20000000,//���ܴ������þ�����ɾ�����Ա
        OP_NAS_UNPERMIT_SHRINK_SHARED_VG = 0x9033 | 0x20000000,//���ܴ�������nas����ľ�����ɾ�����Ա
        OP_NAS_CONFIG_VG_NO_ENOUGH_MEMBER = 0x9034 | 0x20000000,//���þ��飬����Ա��������
        OP_NAS_VG_CONFIGED = 0x9035 | 0x20000000,//����������
        OP_NAS_VG_ERROR_CALL_METHOD = 0x9036 | 0x20000000,//�Ծ���������õķ�������
        OP_NAS_VG_FAILED_CREATE_SIMPLE_VOLUME = 0x9037 | 0x20000000,//OsnNasApi.CreateSimpleVolume����ʧ��
        OP_NAS_VG_FAILED_CREATE_STRIPE_VOLUME = 0x9038 | 0x20000000,//OsnNasApi.CreateStripeVolume����ʧ��
        OP_NAS_VG_FAILED_CREATE_FOLDER = 0x9039 | 0x20000000,//���ļ���OsnNasʧ��
        OP_NAS_VG_FAILED_INITIALIZE_FILE_SYSTEM = 0x9040 | 0x20000000,//��ʼ���ļ�ϵͳʧ��
        OP_NAS_FAILED_CREATE_NASGROUP_ACCOUNT = 0x9041 | 0x20000000,//��NASGroupϵͳ�û���ʧ��
        OP_NAS_FAILED_CREATE_ACCOUNT = 0x9042 | 0x20000000,//��ϵͳ�û�ʧ��
        OP_NAS_CREATE_ACCOUNT_WITH_CONFLICT = 0x9043 | 0x20000000,//ϵͳ�û���ͻ
        OP_NAS_GROUP_NAME_MAPPING_EXSITED = 0x9044 | 0x20000000,//����ӳ�������Ѵ���
        OP_NAS_GROUP_NAME_MAPPING_NOT_FOUND = 0x9045 | 0x20000000,//����ӳ������δ�ҵ�
        OP_NAS_DELETE_PRIMARY_GROUP_NAME_MAPPING = 0x9046 | 0x20000000,//��UNIX�û�����ԣ����������ӳ�����ã���ӳ�䲻��ɾ��
        OP_NAS_REPEAT_SET_PRIMARY_GROUP_NAME_MAPPING = 0x9047 | 0x20000000,//�ظ������û�����ӳ��

        OP_RS_VOLUME_USED = 0xa001 | 0x20000000,//������ʹ����
        OP_RS_VOLUME_NOT_FOUND = 0xa002 | 0x20000000,//��δ�ҵ�
        OP_RS_WRITE_ITEM_FAILED = 0xa003 | 0x20000000,//д����RS������Ϣʧ��
        OP_RS_DELETE_ITEM_FAILED = 0xa004 | 0x20000000,//д����RS������Ϣʧ��
        OP_RS_BACKUP_NOT_FOUND = 0xa005 | 0x20000000,//����RS������Ϣδ�ҵ�

        OP_LUNMIRROR_FAILED_WRITE_ITEM = 0x10001 | 0x20000000,//����дLun Mirror��Ϣʧ��
        OP_LUNMIRROR_VOLUME_USED = 0x10002 | 0x20000000,//������ʹ����
        OP_LUNMIRROR_VOLUME_NOT_FOUND = 0x10003 | 0x20000000,//��δ�ҵ�
        OP_LUNMIRROR_NOT_FOUND = 0x10004 | 0x20000000,//����Lun Mirror��Ϣδ�ҵ�
        OP_LUNMIRROR_FAILED_DEL_ITEM = 0x10005 | 0x20000000,//����ɾ��Lun Mirror��Ϣʧ��
        OP_LUNMIRROR_SOURCE_ASYNC_MAKED = 0x10006 | 0x20000000,//Դ���������첽Lun Mirror
        OP_LUNMIRROR_SOURCE_SYNC_MAKED = 0x10007 | 0x20000000,//Դ��������ͬ��Lun Mirror
        OP_LUNMIRROR_SOURCE_SYNC_MAXIMUM = 0x10008 | 0x20000000,//��ͬһԴ������ͬ��Lun Mirror�������

        OP_VVG_NAME_CONFLICT = 0x20001 | 0x20000000,//Virtual Volume Group���Ƴ�ͻ
        OP_VVG_NOT_FOUND = 0x20002 | 0x20000000,//Virtual Volume Groupδ�ҵ�
        OP_VVG_USED = 0x20003 | 0x20000000,//Virtual Volume Group����ʹ����
        OP_VVG_CDPSETTING_FAILED_WRITE_ITEM = 0x20004 | 0x20000000,
        OP_VVG_CDPSETTING_FAILED_DEL_ITEM = 0x20005 | 0x20000000,
        OP_VVG_CDPSETTING_NOT_FOUND = 0x20006 | 0x20000000,
        OP_VVG_CDPSETTING_FOUND = 0x20007 | 0x20000000,

        OP_SCHEDULE_EXSITED = 0x30001 | 0x20000000,//����ƻ��Ѿ�����
        OP_SCHEDULE_NOT_FOUND = 0x30002 | 0x20000000,
        OP_SCHEDULE_NOT_SUPPORT = 0x30003 | 0x20000000,

        OP_TARGETCHANNEL_REMOVE_WITH_MAPPING = 0x40001 | 0x20000000,
        OP_TARGETCHANNEL_FAILED_DEL_ITEM = 0x40002 | 0x20000000,
        OP_TARGETCHANNEL_NOT_FOUND = 0x40003 | 0x20000000,

        //0x2a000000���ɿͻ���ʹ��
        OP_DB_REMOVE_SERVER_SUCCESS = 0x0001 | 0x2b000000,//�����ݿ�ɾ����������¼�ɹ�
        OP_DB_REMOVE_SERVER_FAILED = 0x0002 | 0x2b000000,//�����ݿ�ɾ����������¼ʧ��
        OP_DB_ADD_SERVER_SUCCESS = 0x0003 | 0x2b000000,
        OP_DB_ADD_SERVER_FAILED = 0x0004 | 0x2b000000,
        OP_DB_ADD_POOL_SUCCESS = 0x0005 | 0x2b000000,
        OP_DB_ADD_POOL_FAILED = 0x0006 | 0x2b000000,
        OP_DB_DELETE_POOL_SUCCESS = 0x0007 | 0x2b000000,
        OP_DB_DELETE_POOL_FAILED = 0x0008 | 0x2b000000,
        OP_DB_CREATE_VOLUME_SUCCESS = 0x0009 | 0x2b000000,
        OP_DB_CREATE_VOLUME_FAILED = 0x000a | 0x2b000000,
        OP_DB_BATCH_CREATE_VOLUME_SUCCESS = 0x000b | 0x2b000000,
        OP_DB_BATCH_CREATE_VOLUME_WARNING = 0x000c | 0x2b000000,
        OP_DB_BATCH_CREATE_VOLUME_FAILED = 0x000e | 0x2b000000,
        OP_DB_DELETE_VOLUME_SUCCESS = 0x000f | 0x2b000000,
        OP_DB_DELETE_VOLUME_FAILED = 0x0010 | 0x2b000000,
        OP_DB_UPDATE_POOL_SIZE_SUCCESS = 0x0011 | 0x2b000000,
        OP_DB_UPDATE_POOL_SIZE_FAILED = 0x0012 | 0x2b000000,
        OP_DB_ADD_STORAGE_SERVER_SUCCESS = 0x0013 | 0x2b000000,
        OP_DB_ADD_STORAGE_SERVER_FAILED = 0x0014 | 0x2b000000,
        OP_DB_REMOVE_STORAGE_SERVER_SUCCESS = 0x0015 | 0x2b000000,
        OP_DB_REMOVE_STORAGE_SERVER_FAILED = 0x0016 | 0x2b000000,
        OP_DB_ADD_CHANNEL_TO_CLIENT_SUCCESS = 0x0017 | 0x2b000000,
        OP_DB_ADD_CHANNEL_TO_CLIENT_FAILED = 0x0018 | 0x2b000000,
        OP_DB_REMOVE_CHANNEL_FROM_CLIENT_SUCCESS = 0x0019 | 0x2b000000,
        OP_DB_REMOVE_CHANNEL_FROM_CLIENT_FAILED = 0x001a | 0x2b000000,
        OP_DB_ADD_BACKUP_SERVER_SUCCESS = 0x001b | 0x2b000000,
        OP_DB_ADD_BACKUP_SERVER_FAILED = 0x001c | 0x2b000000,
        OP_DB_REMOVE_BACKUP_SERVER_SUCCESS = 0x001d | 0x2b000000,
        OP_DB_REMOVE_BACKUP_SERVER_FAILED = 0x001e | 0x2b000000,
        OP_DB_UPDATE_VOLUME_FLAG_SUCCESS = 0x001f | 0x2b000000,
        OP_DB_UPDATE_VOLUME_FLAG_FAILED = 0x0020 | 0x2b000000,
        OP_DB_ADD_COMMON_MAPPING_SUCCESS = 0x0021 | 0x2b000000,
        OP_DB_ADD_COMMON_MAPPING_FAILED = 0x0022 | 0x2b000000,
        OP_DB_REMOVE_COMMON_MAPPING_SUCCESS = 0x0023 | 0x2b000000,
        OP_DB_REMOVE_COMMON_MAPPING_FAILED = 0x0024 | 0x2b000000,
        OP_DB_ADD_SNAPSHOT_SUCCESS = 0x0025 | 0x2b000000,
        OP_DB_ADD_SNAPSHOT_FAILED = 0x0026 | 0x2b000000,
        OP_DB_REMOVE_SNAPSHOT_SUCCESS = 0x0027 | 0x2b000000,
        OP_DB_REMOVE_SNAPSHOT_FAILED = 0x0028 | 0x2b000000,
        OP_DB_RESTORE_CDP_SNAPSHOT_SUCCESS = 0x0029 | 0x2b000000,
        OP_DB_RESTORE_CDP_SNAPSHOT_FAILED = 0x002a | 0x2b000000,
        OP_DB_ADD_MIRROR_SUCCESS = 0x002b | 0x2b000000,
        OP_DB_ADD_MIRROR_FAILED = 0x002c | 0x2b000000,
        OP_DB_ADD_MIRROR_MAPPING_SUCCESS = 0x002d | 0x2b000000,
        OP_DB_ADD_MIRROR_MAPPING_FAILED = 0x002e | 0x2b000000,
        OP_DB_REMOVE_MIRROR_SUCCESS = 0x002f | 0x2b000000,
        OP_DB_REMOVE_MIRROR_FAILED = 0x0030 | 0x2b000000,
        OP_DB_REMOVE_MIRROR_MAPPING_SUCCESS = 0x0031 | 0x2b000000,
        OP_DB_REMOVE_MIRROR_MAPPING_FAILED = 0x0032 | 0x2b000000,
        OP_DB_ADD_VG_SUCCESS = 0x0033 | 0x2b000000,
        OP_DB_ADD_VG_FAILED = 0x0034 | 0x2b000000,
        OP_DB_EXTEND_VG_SUCCESS = 0x0035 | 0x2b000000,
        OP_DB_EXTEND_VG_FAILED = 0x0036 | 0x2b000000,
        OP_DB_SHRINK_VG_SUCCESS = 0x0037 | 0x2b000000,
        OP_DB_SHRINK_VG_FAILED = 0x0038 | 0x2b000000,
        OP_DB_REMOVE_VG_SUCCESS = 0x0039 | 0x2b000000,
        OP_DB_REMOVE_VG_FAILED = 0x003a | 0x2b000000,
        OP_DB_ADD_NASSHARE_SUCCESS = 0x003b | 0x2b000000,
        OP_DB_ADD_NASSHARE_FAILED = 0x003c | 0x2b000000,
        OP_DB_SET_CIFSSHARE_SUCCESS = 0x003d | 0x2b000000,
        OP_DB_SET_CIFSSHARE_FAILED = 0x003e | 0x2b000000,
        OP_DB_REM_CIFSSHARE_SUCCESS = 0x003f | 0x2b000000,
        OP_DB_REM_CIFSSHARE_FAILED = 0x0040 | 0x2b000000,
        OP_DB_SET_NFSSHARE_SUCCESS = 0x0041 | 0x2b000000,
        OP_DB_SET_NFSSHARE_FAILED = 0x0042 | 0x2b000000,
        OP_DB_REM_NFSSHARE_SUCCESS = 0x0043 | 0x2b000000,
        OP_DB_REM_NFSSHARE_FAILED = 0x0044 | 0x2b000000,
        OP_DB_ADD_HOSTGROUP_SUCCESS = 0x0045 | 0x2b000000,
        OP_DB_ADD_HOSTGROUP_FAILED = 0x0046 | 0x2b000000,
        OP_DB_REMOVE_HOSTGROUP_SUCCESS = 0x0047 | 0x2b000000,
        OP_DB_REMOVE_HOSTGROUP_FAILED = 0x0048 | 0x2b000000,
        OP_DB_ADD_HOSTMEMBER_SUCCESS = 0x0049 | 0x2b000000,
        OP_DB_ADD_HOSTMEMBER_FAILED = 0x004a | 0x2b000000,
        OP_DB_REMOVE_HOSTMEMBER_SUCCESS = 0x004b | 0x2b000000,
        OP_DB_REMOVE_HOSTMEMBER_FAILED = 0x004c | 0x2b000000,
        OP_DB_ADD_UNIXUSER_GROUP_SUCCESS = 0x004d | 0x2b000000,
        OP_DB_ADD_UNIXUSER_GROUP_FAILED = 0x004e | 0x2b000000,
        OP_DB_REMOVE_UNIXUSER_GROUP_SUCCESS = 0x004f | 0x2b000000,
        OP_DB_REMOVE_UNIXUSER_GROUP_FAILED = 0x0050 | 0x2b000000,
        OP_DB_ADD_UNIXUSER_MEMBER_SUCCESS = 0x0051 | 0x2b000000,
        OP_DB_ADD_UNIXUSER_MEMBER_FAILED = 0x0052 | 0x2b000000,
        OP_DB_REMOVE_UNIXUSER_MEMBER_SUCCESS = 0x0053 | 0x2b000000,
        OP_DB_REMOVE_UNIXUSER_MEMBER_FAILED = 0x0054 | 0x2b000000,
        OP_DB_ADD_SSBACKUP_SUCCESS = 0x0055 | 0x2b000000,
        OP_DB_ADD_SSBACKUP_FAILED = 0x0056 | 0x2b000000,
        OP_DB_REMOVE_SSBACKUP_SUCCESS = 0x0057 | 0x2b000000,
        OP_DB_REMOVE_SSBACKUP_FAILED = 0x0058 | 0x2b000000,
        OP_DB_REMOVE_SERVER_WITH_MIRROR = 0x005a | 0x2b000000,
        OP_DB_REMOVE_SERVER_WITH_SSBACKUP = 0x005c | 0x2b000000,
        OP_DB_REMOVE_SERVER_WITH_RSBACKUP = 0x005e | 0x2b000000,
        OP_DB_UPDATE_HOSTGROUP_SUCCESS = 0x005f | 0x2b000000,
        OP_DB_UPDATE_HOSTGROUP_FAILED = 0x0060 | 0x2b000000,
        OP_DB_ADD_RSBACKUP_SUCCESS = 0x0061 | 0x2b000000,
        OP_DB_ADD_RSBACKUP_FAILED = 0x0062 | 0x2b000000,
        OP_DB_REMOVE_RSBACKUP_SUCCESS = 0x0063 | 0x2b000000,
        OP_DB_REMOVE_RSBACKUP_FAILED = 0x0064 | 0x2b000000,
        OP_DB_UPDATE_TARGET_IP_SUCCESS = 0x0065 | 0x2b000000,
        OP_DB_UPDATE_TARGET_IP_FAILED = 0x0066 | 0x2b000000,
        OP_DB_ADD_LUNMIRROR_SUCCESS = 0x0067 | 0x2b000000,
        OP_DB_ADD_LUNMIRROR_FAILED = 0x0068 | 0x2b000000,
        OP_DB_REMOVE_LUNMIRROR_SUCCESS = 0x0069 | 0x2b000000,
        OP_DB_REMOVE_LUNMIRROR_FAILED = 0x006a | 0x2b000000,
        OP_DB_ADD_VVG_SUCCESS = 0x006b | 0x2b000000,
        OP_DB_ADD_VVG_FAILED = 0x006c | 0x2b000000,
        OP_DB_REMOVE_VVG_SUCCESS = 0x006d | 0x2b000000,
        OP_DB_REMOVE_VVG_FAILED = 0x006e | 0x2b000000,
        OP_DB_UPDATE_CHANNEL_SUCCESS = 0x006f | 0x2b000000,
        OP_DB_UPDATE_CHANNEL_FAILED = 0x0070 | 0x2b000000,
        OP_DB_UPDATE_TARGET_ALIAS_SUCCESS = 0x0071 | 0x2b000000,
        OP_DB_UPDATE_TARGET_ALIAS_FAILED = 0x0072 | 0x2b000000,

        OP_DB_CONNECT_FAILED = 0xfffa | 0x2b000000,//���ݿ�����ʧ��
        OP_DB_FAILED = 0xfffe | 0x2b000000,//���ݿ����һ���Դ���
        OP_DB_EXCEPTION = 0xffff | 0x2b000000,//���ݿ���������쳣


        OP_PROXY_FAILED_GET_SERVER_INFOMATION = 0x0001 | 0x2c000000,
        OP_PROXY_FAILED = 0x0002 | 0x2c000000,
        OP_PROXY_COMPILE_ASSEMBLE_FAILED = 0xfffe | 0x2c000000,//���붯̬���ô�����ʧ��
        OP_PROXY_EXCEPTION = 0xffff | 0x2c000000,

        OP_DEL_ITEM_SUCCESS_WITH_FAILURE = 0x0001 | 0x2d000000,//ɾ������������Ŀ�ɹ���������APIʧ��

        OP_SEND_CMD_FAILED = 0x0001 | 0x2e000000,//socket��������ʧ��

        OP_UnPermission = 0xfffe | 0x2f000000,//��Ȩ��
        OP_EXCEPTION = 0xffff | 0x2f000000,//�쳣
    }
    [Serializable]
    public enum ScheduleMode
    {
        MODE_MINUTES,
        MODE_DAYLY,
        MODE_WEEKLY,
        MODE_MONTHLY,
        MODE_NONE
    }
    [Serializable]
    public enum WeekBase
    {
        FIRST = 1,
        SECOND = 2,
        THIRD = 3,
        FOURTH = 4,
        LAST = 5,
        NONE = 6
    }
    [Serializable]
    public enum DayOfWeek
    {

        /// <remarks/>
        Sunday,

        /// <remarks/>
        Monday,

        /// <remarks/>
        Tuesday,

        /// <remarks/>
        Wednesday,

        /// <remarks/>
        Thursday,

        /// <remarks/>
        Friday,

        /// <remarks/>
        Saturday,
    }
    [Serializable]
    public enum RepServiceStatus
    {
        RS_SUCCESS = 0x0,//�ɹ�
        RS_ERROR_NOT_FIND_TARGET = 0x00001 | 0x2A000000,  //ʧ��:Client������Ϣ��δ���ҵ���Target��Ϣ
        RS_ERROR_BACKUP_EXISTED = 0x00002 | 0x2A000000,  //ʧ��:�½�������ʱ��Client������Ϣ���Ѵ��ڸñ�������Ϣ
        RS_ERROR_NOT_FIND_DISK = 0x00003 | 0x2A000000,  //ʧ��:Client���Ҳ�����Ӧ���̻��
        RS_ERROR_API_ADD_BACKUP = 0x00004 | 0x2A000000,  //ʧ��:Client�е���OSNAddBackupInfo��������
        RS_ERROR_API_SET_DEVICE_ACTIVE = 0x00005 | 0x2A000000,  //ʧ��:Client�е���OSNSetDeviceActiveBackup��������
        RS_ERROR_API_SET_VOLUME_ACTIVE = 0x00006 | 0x2A000000,  //ʧ��:Client�е���OSNSetVolumeActiveBackup��������
        RS_ERROR_API_GET_DISK_INIT_STATUS = 0x00007 | 0x2A000000,  //ʧ��:Client�е���OSNCheckDiskInitInfo��������
        RS_ERROR_API_GET_DISK_MIRROR_STATUS = 0x00008 | 0x2A000000,  //ʧ��:Client�е���OSNGetDeviceState��������
        RS_ERROR_API_GET_VOLUME_INIT_STATUS = 0x00009 | 0x2A000000,  //ʧ��:Client�е���OSNCheckVolumeInitInfo��������
        RS_ERROR_API_GET_VOLUME_MIRROR_STATUS = 0x0000A | 0x2A000000,  //ʧ��:Client�е���OSNGetVolumeState��������
        RS_ERROR_BACKUP_NOT_EXISTED = 0x0000B | 0x2A000000,  //ʧ��:Client������Ϣ��δ�ҵ��˱�������Ϣ
        RS_ERROR_API_UNSET_DEVICE_ACTIVE = 0x0000C | 0x2A000000,  //ʧ��:Client�е���OSNUnsetDeviceActiveBackup��������
        RS_ERROR_API_UNSET_VOLUME_ACTIVE = 0x0000D | 0x2A000000,  //ʧ��:Client�е���OSNUnsetVolumeActiveBackup��������
        RS_ERROR_API_REMOVE_BACKUP = 0x0000E | 0x2A000000,  //ʧ��:Client�е���OSNRemoveBackupInfo��������
        RS_ERROR_BACKUP_DISABLED = 0x0000F | 0x2A000000,  //ʧ��:Client�б�����ΪDisabled״̬
        RS_ERROR_API_INIT_DISK_BACKUP = 0x00010 | 0x2A000000,  //ʧ��:Client�е���OSNInitializeDevice��������
        RS_ERROR_API_INIT_VOLUME_BACKUP = 0x00011 | 0x2A000000,  //ʧ��:Client�е���OSNInitializeVolume��������
        RS_ERROR_API_CANCEL_INIT_DISK_BACKUP = 0x00012 | 0x2A000000,  //ʧ��:Client�е���OSNCancelInitializeDevice��������
        RS_ERROR_API_CANCEL_INIT_VOLUME_BACKUP = 0x00013 | 0x2A000000,  //ʧ��:Client�е���OSNCancelInitializeVolume��������
        RS_ERROR_OPEN_REPLICATOR_REGKEY = 0x00014 | 0x2A000000,  //ʧ��:Client�д�"SOFTWARE\\Enterprise Information Management\\OSN Replicator"ע������
        RS_ERROR_OPEN_OSNSERVICE_PARA_REGKEY = 0x00015 | 0x2A000000,  //ʧ��:Client�д�"System\\CurrentControlSet\\Services\\OSNService\\Parameters"ע������
        RS_ERROR_SET_SAME_REPLICATION_MODE = 0x00016 | 0x2A000000,  //ʧ��:Client��ԭ�ȵı���ģʽ����Ҫ��ı���ģʽ��ͬ
        RS_ERROR_CANNOT_SET_SYNCH_MODE_BACKUP_EXISTED = 0x00017 | 0x2A000000,  //ʧ��:Client�������Ѿ������˱����飬�������첽����ģʽ��Ϊͬ��ģʽ
        RS_ERROR_API_GET_LOG_VOLUME_DEVICE_NAME = 0x00018 | 0x2A000000,  //ʧ��:Client�е���OSNGetLogVolumeDevice��������
        RS_ERROR_LOG_DEVICE_BACKUPED = 0x00019 | 0x2A000000,  //ʧ��:Client��Ҫ�����õ��첽���ݱ�����ѽ����˱����飬��ѡ�������첽���ݱ����
        RS_ERROR_API_SET_REPLICATION_MODE = 0x0001A | 0x2A000000,  //ʧ��:Client�е���OSNSetReplicationMode��������
        RS_ERROR_API_GET_SYSTEM_WWN = 0x0001B | 0x2A000000,  //ʧ��:Client�е���OSNGetCharWWN��������
        RS_ERROR_NOT_FIND_CONFIG_FILE = 0x0001C | 0x2A000000,  //ʧ��:ClientĿ¼��δ�ҵ������ļ�
        RS_ERROR_FILE_TARGET_LINE_ILLEGAL_FORMAT = 0x0001D | 0x2A000000,  //ʧ��:Client�����ļ��У�target��¼�и�ʽ����
        RS_ERROR_FILE_BACKUP_LINE_ILLEGAL_FORMAT = 0x0001E | 0x2A000000,  //ʧ��:Client�����ļ��У�backup��¼�и�ʽ����
        RS_ERROR_DELETE_SERVER_BACKUP_EXISTED = 0x0001F | 0x2A000000,  //ʧ��:Client���Խ����б����飬���Բ���ɾ��������
        RS_ERROR_API_REMOVE_TARGET_INFO = 0x00020 | 0x2A000000,  //ʧ��:Client�е���OSNRemoveTargetInfo��������
        RS_ERROR_FIND_TARGET = 0x00021 | 0x2A000000,  //ʧ��:Client������Ϣ���Ѿ����ڸ�Target��Ϣ
        RS_ERROR_API_ADD_TARGET_INFO = 0x00022 | 0x2A000000,  //ʧ��:Client�е���OSNAddTargetInfo��������
        RS_ERROR_API_GET_VOLUME_ID = 0x00023 | 0x2A000000,  //ʧ��:Client�е���OSNGetVolumeID�������� 
        RS_ERROR_API_GET_VOLUME_SIZE = 0x00024 | 0x2A000000,  //ʧ��:Client�е���OSNVfGetVolumeSize�������� 
        RS_ERROR_API_CATCH_EXCEPTION = 0x00025 | 0x2A000000,  //ʧ��:Client��API���������в����쳣
        RS_SUCCESS_START_CREATE_CDP = 0x00026 | 0x2A000000,  //�ɹ�:Client�п�ʼ�½�CDP
        RS_SUCCESS_FINISH_CREATE_CDP = 0x00027 | 0x2A000000,  //�ɹ�:Client������½�CDP
        RS_ERROR_NOT_FIND_RAWPARTITION = 0x00028 | 0x2A000000,  //ʧ�ܣ�Client��û�п�����Ϊ�첽�����̵����
        RS_ERROR_FILE_CREATE_CDP = 0x00029 | 0x2A000000,   //ʧ�ܣ�Client�˴���CDPʧ��
        RS_ERROR_SET_ASYNCH_LOG_DEVICE_BACKUP_EXISTED = 0x0002A | 0x2A000000,   //ʧ�ܣ�Client�������µ��첽�����ʧ�ܣ�����ԭ���첽��������Ѵ��ڱ�������Ϣ
        RS_ERROR_BACKUP_INITIALIZE = 0x002B | 0x2A000000,//ʧ�ܱ��������ڳ�ʼ��
        RS_ERROR_API_GET_DISK_RECOVERY_STATUS = 0x002C | 0x2A000000,//ʧ�ܣ���ȡ���̵Ļָ���Ϣʧ�� 
        RS_ERROR_API_GET_VOLUME_RECOVERY_STATUS = 0x002D | 0x2A000000,//ʧ�ܣ���ȡ��Ļָ���Ϣʧ��
    };
    [Serializable]
    public enum RpcServiceCommand
    {
        OSN_RC_CMD_GET_DISKLIST = 0x00000,   //��ȡClient���̻����Ϣ���������Ϊnull

        OSN_RC_CMD_CREATE_BACKUPINFO = 0x00001,   //֪ͨClient���������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
        //(1): DesTargetName
        //(2): DesVolumeID

        OSN_RC_CMD_DELETE_BACKUPINFO = 0x00002,   //֪ͨClientɾ�������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
        //(1): DesTargetName

        OSN_RC_CMD_GET_BACKUP_STATUS = 0x00003,   //��ȡClient������״̬���������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_INITIALIZE_BACKUP = 0x00004,   //֪ͨClient��ʼ�������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_CANCEL_INITI_BACKUP = 0x00005,   //֪ͨClientȡ����ʼ�������飬�������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_GET_REPLICATE_MODE = 0x00006,   //��ȡClient������ģʽ���������Ϊnull

        OSN_RC_CMD_SET_REPLICATE_MODE = 0x00007,   //��ȡClient������ģʽ���������Ϊ(0):_REPLICATION_MODE(MODE_ASYNCH��MODE_SYNCH) 
        //(1):LogDeviceLabel (C:��D:)

        OSN_RC_CMD_GET_INITIATOR_INFO = 0x00008,   //��ȡClient�������Ϣ���������Ϊnull

        OSN_RC_CMD_READ_CONFIG_FILE = 0x00009,   //֪ͨClient���¶�ȡ�����ļ����������Ϊnull ���ͻ�����ӷ�����ʱʹ�ã�

        OSN_RC_CMD_DELETE_TARGET_INFO = 0x0000A,   //֪ͨClientɾ�����������������Ϊ(0):TargetName
        //(1):TargetIP

        OSN_RC_CMD_CREATE_CDP = 0x00010,   //֪ͨClient����CDP���������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_ADD_TARGET_INFO = 0x00011,   //֪ͨClient��ӷ��������������Ϊ(0): TargetName
        //(1): TargetIPAddress
        //(2): ClientIPAddress
        OSN_RC_CMD_CREATE_CDP_SCHEDULE = 0x00012,   //֪ͨClient���CDP����ƻ����������Ϊ(0): TargetName
        //(1): TargetIPAddress
        //(2): SrcDiskSign
        //(3): CDPStartTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
        //WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
        //Second:1,2...,59;)
        //(4): CDPStopTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
        //WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
        //Second:1,2...,59;)
        //(5): CDPMode
        OSN_RC_CMD_DELETE_CDP_SCHEDULE = 0x00013,    //֪ͨClientɾ��CDP����ƻ����������Ϊ(0): SrcDiskSign

        OSN_RC_CMD_SET_STATUSERVICEIP = 0x00014,    //֪ͨClient�޸�BackupStatuThread�ķ��͵�Ŀ�ĵ�ַ���������Ϊ��0����StatuServiceIP
        OSN_RC_CMD_GET_RAWPARTITION = 0x00015,    //֪ͨClient��������б��������
        OSN_RC_CMD_GET_CDP_SCHEDULE = 0x00016,    //֪ͨClient��ȡCDP����ƻ�����, �������Ϊ(0):TargetName  (1):SrcDiskSign
        OSN_RC_CMD_CREATE_TIMECDP = 0x00017,    //֪ͨClient����CDP������IOCTL_OSNCFG_RC_TIME_MARKER�ķ�ʽ���������Ϊ(0): SrcDiskSignature(Signature(X8)_Offset(X8))

    };
    [Serializable]
    public enum CDP_SCHEDULE_MODE
    {
        MODE_MONTH = 0,
        MODE_WEEK = 1,
        MODE_DAY = 2,
        MODE_MINUTE = 3,
        MODE_NONE = 4,
    }
    [Serializable]
    public class WeekModifier
    {
        public WeekBase Base;
        public DayOfWeek[] Days;
        public WeekModifier()
        {
            Base = WeekBase.NONE;
            Days = null;
        }
    }
    [Serializable]
    public class ScheduleModifier
    {
        //[FieldOffset(0)]
        public UInt32 IntervalDays;//1~365
        //[FieldOffset(0)]
        public UInt32 IntervalMinutes;//1~1440
        //[FieldOffset(0)]
        public WeekModifier Week;
    }
    [Serializable]
    public class Date
    {
        public UInt32 Year;//1~...
        public UInt32 Month;//1~12
        public UInt32 Day;//1~31
        public Date()
        {
            Year = 0;
            Month = 0;
            Day = 0;
        }
        public Date(UInt32 year, UInt32 month, UInt32 day)
        {
            Year = year;
            Month = month;
            Day = day;
        }
    }
    [Serializable]
    public class Time
    {
        public UInt32 Hour;//1~24
        public UInt32 Minute;//1~59
        public UInt32 Sencond;//1~59
        public Time()
        {
            Hour = 0;
            Minute = 0;
            Sencond = 0;
        }
        public Time(UInt32 hour, UInt32 minute)
        {
            Hour = hour;
            Minute = minute;
            Sencond = 0;
        }
    }
    [Serializable]
    public class CDPSchedule
    {
        public ScheduleMode Mode;
        public Date[] StartDates;
        public Time StartTime;
        public Date[] EndDates;
        public Time EndTime;
        public ScheduleModifier Modifier;
        public CDPSchedule()
        {
            Mode = ScheduleMode.MODE_NONE;
            StartDates = null;
            StartTime = null;
            EndDates = null;
            EndTime = null;
            Modifier = null;
        }
        /// <summary>
        /// ת��Ϊ�ַ�����ʽ
        /// </summary>
        /// <returns>NULL��ʾ��ʽ����</returns>
        public String FormatStartDateTime()
        {
            if (ScheduleMode.MODE_NONE == Mode)
                return null;
            if (ScheduleMode.MODE_MINUTES == Mode &&
                (StartDates == null || StartTime == null || Modifier == null))
                return null;
            if (ScheduleMode.MODE_DAYLY == Mode &&
                (StartDates == null || StartTime == null || Modifier == null))
                return null;
            if (ScheduleMode.MODE_WEEKLY == Mode &&
                (StartTime == null || Modifier == null || Modifier.Week.Days == null))
                return null;
            if (ScheduleMode.MODE_MONTHLY == Mode &&
               (StartDates == null || StartTime == null))
                return null;
            String strSchedule = null;
            String[] weekDayArray = null;
            Date start = null;
            switch (Mode)
            {
                case ScheduleMode.MODE_MINUTES:
                    start = StartDates[0];
                    strSchedule = String.Format("Year:{0};Month:{1};Day:{2};WeekNum:ALL;DayOfWeek:ALL",
                                                start.Year, start.Month, start.Day);
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                StartTime.Hour, StartTime.Minute, StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:ALL;IntervalMinutes:{1}", strSchedule,
                                                Modifier.IntervalMinutes);
                    break;
                case ScheduleMode.MODE_DAYLY:
                    start = StartDates[0];
                    strSchedule = String.Format("Year:{0};Month:{1};Day:{2};WeekNum:ALL;DayOfWeek:ALL",
                                                start.Year, start.Month, start.Day);
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                StartTime.Hour, StartTime.Minute, StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:{1};IntervalMinutes:ALL", strSchedule,
                                                Modifier.IntervalDays);
                    break;
                case ScheduleMode.MODE_WEEKLY:
                    weekDayArray = new String[Modifier.Week.Days.Length];
                    for (int index = 0; index < Modifier.Week.Days.Length; ++index)
                        weekDayArray[index] = ((int)(Modifier.Week.Days[index])).ToString();
                    strSchedule = String.Format("Year:ALL;Month:ALL;Day:ALL;WeekNum:{0};DayOfWeek:{1}",
                                                Modifier.Week.Base, String.Join(",", weekDayArray));
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                StartTime.Hour, StartTime.Minute, StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:ALL;IntervalMinutes:ALL", strSchedule);
                    break;
                case ScheduleMode.MODE_MONTHLY:
                    String[] monthArray = new String[StartDates.Length];
                    for (int index = 0; index < StartDates.Length; ++index)
                        monthArray[index] = StartDates[index].Month.ToString();
                    bool dayFlag = true;//default to indicate specific day
                    if (StartDates[0].Day == 0)
                        dayFlag = false;
                    if (Modifier != null)
                    {
                        weekDayArray = new String[Modifier.Week.Days.Length];
                        for (int index = 0; index < Modifier.Week.Days.Length; ++index)
                            weekDayArray[index] = ((int)(Modifier.Week.Days[index])).ToString();
                    }
                    strSchedule = String.Format("Year:ALL;Month:{0};Day:{1};WeekNum:{2};DayOfWeek:{3}",
                                                String.Join(",", monthArray), dayFlag ? ((int)(StartDates[0].Day)).ToString() : "ALL",
                                                (dayFlag || null == Modifier) ? "ALL" : ((int)(Modifier.Week.Base)).ToString(),
                                                (dayFlag || null == Modifier) ? "ALL" : String.Join(",", weekDayArray));
                    strSchedule = String.Format("{0};Hour:{1};Minute:{2};Second:{3}", strSchedule,
                                                StartTime.Hour, StartTime.Minute, StartTime.Sencond);
                    strSchedule = String.Format("{0};IntervalDays:ALL;IntervalMinutes:ALL", strSchedule);
                    break;
                default: break;
            }
            return strSchedule;
        }
        private String FormatDayOfWeek(DayOfWeek dayWeek)
        {
            String result;
            switch (dayWeek)
            {
                case DayOfWeek.Sunday: result = "������"; break;
                case DayOfWeek.Saturday: result = "������"; break;
                case DayOfWeek.Friday: result = "������"; break;
                case DayOfWeek.Thursday: result = "������"; break;
                case DayOfWeek.Wednesday: result = "������"; break;
                case DayOfWeek.Tuesday: result = "���ڶ�"; break;
                case DayOfWeek.Monday: result = "����һ"; break;
                default: result = ""; break;
            }
            return result;
        }
        public String FormatDescription()
        {
            String description = null;
            System.Collections.Generic.List<String> list;
            list = new System.Collections.Generic.List<String>();
            if (ScheduleMode.MODE_MINUTES == Mode)
            {
                description = String.Format("��{0}��{1}��{2}��{3}ʱ{4}�ֿ�ʼ��ÿ{5}����", StartDates[0].Year,
                                             StartDates[0].Month, StartDates[0].Day, StartTime.Hour,
                                             StartTime.Minute, Modifier.IntervalMinutes);
            }
            else if (ScheduleMode.MODE_DAYLY == Mode)
            {
                description = String.Format("��{0}��{1}��{2}�տ�ʼ��ÿ{3}��", StartDates[0].Year,
                                             StartDates[0].Month, StartDates[0].Day, Modifier.IntervalDays);
            }
            else if (ScheduleMode.MODE_WEEKLY == Mode)
            {
                foreach (DayOfWeek dayWeek in Modifier.Week.Days)
                {
                    list.Add(FormatDayOfWeek(dayWeek));
                }
                description = String.Format("��{0}��{1}ʱ{2}��", String.Join("��", list.ToArray()),
                                             StartTime.Hour, StartTime.Minute);
            }
            else if (ScheduleMode.MODE_MONTHLY == Mode)
            {
                foreach (Date date in StartDates)
                {
                    list.Add(date.Month.ToString());
                }
                String month = String.Join("��", list.ToArray());
                if (Modifier != null)
                {
                    String week = FormatDayOfWeek(Modifier.Week.Days[0]);
                    description = String.Format("��{0}�µĵ�{1}��{2}{3}ʱ{4}��", month, (int)Modifier.Week.Base,
                                                 week, StartTime.Hour, StartTime.Minute);
                }
                else
                {
                    description = String.Format("��{0}�µĵ�{1}��{2}ʱ{3}", month, StartDates[0].Day,
                                                 StartTime.Hour, StartTime.Minute);
                }
            }
            else
            {
                description = "δ֪����ƻ�.";
            }
            return description;
        }
        public static CDPSchedule FormatSchedule(ScheduleMode mode, String strSchedule)
        {
            CDPSchedule schedule = new CDPSchedule();
            schedule.Mode = mode;
            if (ScheduleMode.MODE_NONE == mode)
                return schedule;
            schedule.StartTime = new Time();
            schedule.Modifier = new ScheduleModifier();
            String[] tmpArray = strSchedule.Split(new String[] { ":", ";" },
                                                  StringSplitOptions.None);
            String strYear = tmpArray[1];
            String strMonth = tmpArray[3];
            String strDay = tmpArray[5];
            String strWeekNum = tmpArray[7];
            String strDayOfWeek = tmpArray[9];
            String strHour = tmpArray[11];
            String strMinute = tmpArray[13];
            String strSecond = tmpArray[15];
            String strIntervalDays = tmpArray[17];
            String strIntervalMinutes = tmpArray[19];
            schedule.StartTime.Hour = Convert.ToUInt32(strHour);
            schedule.StartTime.Minute = Convert.ToUInt32(strMinute);
            schedule.StartTime.Sencond = Convert.ToUInt32(strSecond);
            if (ScheduleMode.MODE_MINUTES == mode)
            {
                schedule.StartDates = new Date[1] { new Date() };
                schedule.StartDates[0].Year = Convert.ToUInt32(strYear);
                schedule.StartDates[0].Month = Convert.ToUInt32(strMonth);
                schedule.StartDates[0].Day = Convert.ToUInt32(strDay);
                schedule.Modifier.IntervalMinutes = Convert.ToUInt32(strIntervalMinutes);
            }
            else if (ScheduleMode.MODE_DAYLY == mode)
            {
                schedule.StartDates = new Date[1] { new Date() };
                schedule.StartDates[0].Year = Convert.ToUInt32(strYear);
                schedule.StartDates[0].Month = Convert.ToUInt32(strMonth);
                schedule.StartDates[0].Day = Convert.ToUInt32(strDay);
                schedule.Modifier.IntervalDays = Convert.ToUInt32(strIntervalDays);
            }
            else if (ScheduleMode.MODE_WEEKLY == mode)
            {
                schedule.Modifier.Week = new WeekModifier();
                schedule.Modifier.Week.Base = WeekBase.NONE;
                String[] weekdayArray = strDayOfWeek.Split(new String[] { "," },
                                                           StringSplitOptions.RemoveEmptyEntries);
                System.Collections.Generic.List<DayOfWeek> dayOfWeekList;
                dayOfWeekList = new System.Collections.Generic.List<DayOfWeek>();
                foreach (String strWeekDay in weekdayArray)
                {
                    DayOfWeek weekday = (DayOfWeek)(Convert.ToInt32(strWeekDay));
                    dayOfWeekList.Add(weekday);
                }
                schedule.Modifier.Week.Days = dayOfWeekList.ToArray();
            }
            else if (ScheduleMode.MODE_MONTHLY == mode)
            {
                String[] monthArray = strMonth.Split(new String[] { "," },
                                                     StringSplitOptions.RemoveEmptyEntries);
                bool flag = strDay.Equals("ALL");
                UInt32 day = flag ? 0 : Convert.ToUInt32(strDay);
                schedule.StartDates = new Date[monthArray.Length];
                for (int index = 0; index < monthArray.Length; ++index)
                {
                    schedule.StartDates[index] = new Date();
                    schedule.StartDates[index].Year = 0;
                    schedule.StartDates[index].Month = Convert.ToUInt32(monthArray[index]);
                    schedule.StartDates[index].Day = day;
                }
                if (flag)
                {
                    schedule.Modifier.Week = new WeekModifier();
                    schedule.Modifier.Week.Base = (WeekBase)(Convert.ToInt32(strWeekNum));
                    String[] weekdayArray = strDayOfWeek.Split(new String[] { "," },
                                                               StringSplitOptions.RemoveEmptyEntries);
                    System.Collections.Generic.List<DayOfWeek> dayOfWeekList;
                    dayOfWeekList = new System.Collections.Generic.List<DayOfWeek>();
                    foreach (String strWeekDay in weekdayArray)
                    {
                        DayOfWeek weekday = (DayOfWeek)(Convert.ToInt32(strWeekDay));
                        dayOfWeekList.Add(weekday);
                    }
                    schedule.Modifier.Week.Days = dayOfWeekList.ToArray();
                }
                else
                {
                    schedule.Modifier = null;
                }
            }
            return schedule;
        }
    }

    //used to OSNScheduleAgent
    [Serializable]
    public enum ScheduleCommond
    {
        OSN_SC_CREATE_SCHEDULE = 0,
        OSN_SC_REMOVE_SCHEDULE = 1,
        OSN_SC_GET_SCHEDULE = 2,
        OSN_SC_DR_TIMEMARK = 3,
        OSN_SC_MODIFY_ITEM = 4,
        OSN_SC_NONE = 100
    }
    [Serializable]
    public enum ScheduleType
    {
        OSN_SCHEDULE_VOLUME_MAPPING = 0,
        OSN_SCHEDULE_VOLUME_DR = 1,
        OSN_SCHEDULE_VVG_MAPPIING = 2,
        OSN_SCHEDULE_VVG_DR = 3,
        OSN_SCHEDULE_REPLICATOR = 4,
        OSN_SCHEDULE_VVG = 5,
        OSN_SCHEDULE_NONE = 100
    }
    [Serializable]
    public class ScheduleItem
    {
        private String m_VolumeId;
        private String m_InitiatorIQN;
        private String m_InitiatorWWN;
        private String m_ClientIpAddress;
        public ScheduleItem()
        {
            m_VolumeId = null;
            m_InitiatorIQN = null;
            m_InitiatorWWN = null;
            m_ClientIpAddress = null;
        }
        public ScheduleItem(String volId, String iqn, String wwn, String ipAddr)
        {
            m_VolumeId = volId;
            m_InitiatorIQN = iqn;
            m_InitiatorWWN = wwn;
            m_ClientIpAddress = ipAddr;
        }
        public String VolumeId
        {
            get { return m_VolumeId; }
            set { m_VolumeId = value; }
        }
        public String ClientChannelIQN
        {
            get { return m_InitiatorIQN; }
            set { m_InitiatorIQN = value; }
        }
        public String ClientChannelWWN
        {
            get { return m_InitiatorWWN; }
            set { m_InitiatorWWN = value; }
        }
        public String ClientIpAddress
        {
            get { return m_ClientIpAddress; }
            set { m_ClientIpAddress = value; }
        }
    }
    [Serializable]
    public class ScheduleMessage
    {
        public ScheduleCommond Commond;
        public ScheduleType Type;
        public String InstanceId;
        public ScheduleMode Mode;
        public String Schedule;//cdpschedule format string or interval(���ӣ�
        public ScheduleItem[] ItemCollection;
        public ScheduleMessage()
        {
            Commond = ScheduleCommond.OSN_SC_NONE;
            Type = ScheduleType.OSN_SCHEDULE_NONE;
            InstanceId = null;
            Mode = ScheduleMode.MODE_NONE;
            Schedule = null;
            ItemCollection = null;
        }
        public ScheduleMessage(ScheduleCommond cmd, ScheduleType type, String instanceId,
                               ScheduleMode mode, String schedule, ScheduleItem[] items)
        {
            Commond = cmd;
            Type = type;
            InstanceId = instanceId;
            Mode = mode;
            Schedule = schedule;
            ItemCollection = items;
        }
    }
    [Serializable]
    public class OPResult
    {
        public OPStatus Status;
        public String Message;
        public OPResult()
        {
            Status = OPStatus.OP_FAILED;
            Message = null;
        }
        public OPResult(OPStatus opstatus, String msg)
        {
            Status = opstatus;
            Message = msg;
        }
    }
    [Serializable]
    public class CRetMsgInfo
    {
        public RepServiceStatus m_RepServiceStatus;
        //public Byte[] m_RetMsgInfo;
        public String m_RetMsgInfo;
        public UInt32 m_ErrorCode;

        public CRetMsgInfo()
        { }
        public CRetMsgInfo(RepServiceStatus status, UInt32 iErrorCode, String strMsg)
        {
            m_RepServiceStatus = status;
            m_ErrorCode = iErrorCode;
            m_RetMsgInfo = strMsg;
        }

    };
    [Serializable]
    public class CSocketMsgInfo
    {
        public RpcServiceCommand m_SendCommand;
        public UInt32 m_ParaNum;
        public String[] m_StrParaList;

        public CSocketMsgInfo()
        {

        }

        public CSocketMsgInfo(RpcServiceCommand command, UInt32 paraNum, String[] strParaList)
        {
            m_SendCommand = command;
            m_ParaNum = paraNum;
            m_StrParaList = strParaList;
        }
    };
}
