using System;
using System.Collections.Generic;
using System.Text;

namespace HostMirrorCDPSchedule
{

    [Serializable]
    public enum OPStatus
    {
        OP_SUCCESS = 0x0,//成功
        OP_FAILED = 0x01 | 0x20000000,//失败
        OP_FAILED_DELETE_TARGET_ITEM = 0x02 | 0x20000000,//删除服务器本地配置信息失败
        OP_FAILED_WRITE_TARGET_ITEM = 0x03 | 0x20000000,//写服务器本地配置信息失败
        OP_DELETE_POOL_WITH_VOLUME = 0x1001 | 0x20000000,//删除有卷的pool
        OP_EXTEND_POOL_DISK_NOT_FOUND = 0x1002 | 0x20000000,//裸盘未找到
        OP_EXTEND_POOL_UNPERMISSION = 0x1003 | 0x20000000,//已达允许的最大容量，不能再扩容pool
        OP_FALED_GET_POOL_ALLOWED_CAPACITY = 0x1004 | 0x20000000,//获取允许的最大pool容量失败
        OP_POOL_NOT_FOUND = 0x1005 | 0x20000000,//pool未找到
        OP_SHRINK_POOL_DISK_NOT_FOUND = 0x1006 | 0x20000000,//pool中磁盘未找到
        OP_POOL_EXCEED_MAXIMUM_MEMBER = 0x1007 | 0x20000000,//超出pool最大卷成员数
        OP_POOL_WITH_CONFLICT_NAME = 0x1008 | 0x20000000,//存储池名称冲突
        OP_POOL_WITH_CONFLICT_ID = 0x1009 | 0x20000000,//存储池ID冲突
        OP_POOL_EXCEED_MAXIMUM_NUMBER = 0x100a | 0x20000000,//超出pool最大个数
        OP_VOLUME_CREATE_CAPACITY_NOT_ENOUGH = 0x2001 | 0x20000000,//新建卷容量大于可用容量或大于2T
        OP_DELETE_VOLUME_WITH_CDP = 0x2002 | 0x20000000,//不能删除已建立CDP快照的卷
        OP_DELETE_VOLUME_USED = 0x2003 | 0x20000000,//不能删除已使用的卷
        OP_DELETE_VOLUME_NOT_FOUND = 0x2004 | 0x20000000,//待删除的卷未找到
        OP_FAILED_DELETE_VOLUME_CONFIG_ITEM = 0x2005 | 0x20000000,//删除卷配置信息失败
        OP_CREATE_VOLUME_WITH_CONFLICT_NAME = 0x2006 | 0x20000000,//卷名冲突
        OP_LOOP_VOLUME_CHECK_FAILED = 0x2007 | 0x20000000,//检查是否已loop失败
        OP_LOOP_VOLUME_REFUSED = 0x2008 | 0x20000000,//卷被用做某些用途时不应loopback
        OP_LOOP_VOLUME_FAILED_WTRITE_ITEM = 0x2009 | 0x20000000,//写卷的loopback信息失败
        OP_LOOP_VOLUME_DELETE_REFUSED = 0x200a | 0x20000000,//卷被用做某些用途时不应删除loopback
        OP_LOOP_VOLUME_FAILED_DEL_ITEM = 0x200b | 0x20000000,//删除卷的loopback信息失败
        OP_RESIZE_VOLUME_CAPACITY_NOT_ENOUGH = 0x200c | 0x20000000,//卷容量大于可用容量或大于2T
        OP_RESIZE_VOLUME_USED = 0x200d | 0x20000000,//不能resize已使用的卷
        OP_RESIZE_VOLUME_NOT_FOUND = 0x200e | 0x20000000,//卷未找到
        OP_RESIZE_VOLUME_REFUSED = 0x200f | 0x20000000,//拒绝：如容量小于原来大小
        OP_RESIZE_VOLUME_FAIED_UPDATE_ITEM = 0x2010 | 0x20000000,//更新卷配置信息失败
        OP_DELETE_VOLUME_WITH_SINGLE_CDP_CONF = 0x2011 | 0x20000000,//应先删除单卷CDP设置
        OP_VOLUME_NOT_FOUND = 0x2012 | 0x20000000,//卷未找到
        OP_TARGET_ITEM_NOT_FOUND = 0x2013 | 0x20000000,//服务器配置信息未找到
        OP_PARTITION_NOT_FOUND = 0x2014 | 0x20000000,
        OP_CREATE_VOLUME_WITH_INVALID_NAME = 0x2015 | 0x20000000,

        OP_CREATE_STORAGE_SERVER_WITH_INVALID_NAME = 0x3001 | 0x20000000,//存储应用服务器名称无效
        OP_CREATE_STORAGE_SERVER_WITH_CONFLICT_NAME = 0x3002 | 0x20000000,//应用服务器名称冲突
        OP_CREATE_STORAGE_SERVER_CAPACITY_NOT_ALLOWED = 0x3003 | 0x20000000,//已达允许的最大应用服务器个数
        OP_CREATE_STORAGE_SERVER_WITH_INVALID_OS_TYPE = 0x3004 | 0x20000000,//无效的存储应用服务器系统类型
        OP_CREATE_STORAGE_SERVER_FAILED_WTRITE_ITEM = 0x3005 | 0x20000000,//写存储应用服务器本地配置信息失败
        OP_DELETE_STORAGE_SERVER_NOT_FOUND = 0x3006 | 0x20000000,//未找到存储应用服务器
        OP_DELETE_STORAGE_SERVER_WITH_MAPPING = 0x3007 | 0x20000000,//不能删除带有卷映射的存储应用服务器
        OP_DELETE_STORAGE_SERVER_WITH_INITIATOR_CHANNEL = 0x3008 | 0x20000000,//不能删除有客户端通道的存储应用服务器
        OP_DELETE_STORAGE_SERVER_FAILED_DEL_ITEM = 0x3009 | 0x20000000,//删除存储应用服务器本地配置信息失败
        OP_CREATE_BACKUP_SERVER_WITH_INVALID_NAME = 0x300a | 0x20000000,//存储备份服务器名称无效
        OP_CREATE_BACKUP_SERVER_WITH_CONFLICT = 0x300b | 0x20000000,//应用服务器名称或客户端WWN冲突
        OP_CREATE_BACKUP_SERVER_CAPACITY_NOT_ALLOWED = 0x300c | 0x20000000,//已达允许的最大应用服务器个数
        OP_CREATE_BACKUP_SERVER_WITH_INVALID_CONFIG = 0x300d | 0x20000000,//无效的备份应用服务器配置
        OP_CREATE_BACKUP_SERVER_FAILED_WTRITE_ITEM = 0x300e | 0x20000000,//写备份应用服务器本地配置信息失败
        OP_DELETE_BACKUP_SERVER_NOT_FOUND = 0x300f | 0x20000000,//未找到存储备份服务器
        OP_DELETE_BACKUP_SERVER_WITH_RELATION = 0x3010 | 0x20000000,//不能删除带有复制关系的备份应用服务器
        OP_DELETE_BACKUP_SERVER_FALED_GET_RELATION = 0x3011 | 0x20000000,//获取RS备份信息失败
        OP_DELETE_BACKUP_SERVER_FAILED_DEL_ITEM = 0x3012 | 0x20000000,//删除备份应用服务器本地配置信息失败
        OP_STORAGE_SERVER_ADD_CHANNEL_IN_OTHER = 0x3013 | 0x20000000,//不能添加已加入其它存储应用服务器的通道
        OP_STORAGE_SERVER_ADD_CHANNEL_NOT_FOUND = 0x3014 | 0x20000000,//未找到指定通道的详细信息
        OP_STORAGE_SERVER_ADD_CHANNEL_FAILED_WRITE_ITEM = 0x3015 | 0x20000000,//添加通道写本地配置信息失败
        OP_STORAGE_SERVER_REMOVE_CHANNEL_NOT_FOUND = 0x3016 | 0x20000000,//删除的通道未从本地配置信息找到
        OP_STORAGE_SERVER_REMOVE_CHANNEL_WITH_MAPPING = 0x3017 | 0x20000000,//不能删除设置卷映射的通道
        OP_STORAGE_SERVER_REMOVE_CHANNEL_FAILED_DEL_ITEM = 0x3018 | 0x20000000,//删除通道本地配置信息失败
        OP_BACKUP_SERVER_FAILED_GET_IPADDRESS = 0x3019 | 0x20000000,//获取备份服务器的IP地址失败
        OP_BACKUP_SERVER_TARGET_NOT_FOUND = 0x301a | 0x20000000,//备份服务器端Target信息未找到
        OP_BACKUP_SERVER_TARGET_EXSITED = 0x301b | 0x20000000,//备份服务器端Target已经存在
        OP_RSBACKUP_SOURCE_NOT_FOUND = 0x301c | 0x20000000,//存储源未找到
        OP_RSBACKUP_EXSITED = 0x301d | 0x20000000,//RS备份关系已建立
        OP_RSBACKUP_NOT_FOUND = 0x301e | 0x20000000,//RS备份关系无效
        OP_RSBACKUP_MODE_CHANGE_WITH_RELATION = 0x301f | 0x20000000,//RS备份关系存在的情况下修改模式

        OP_COMMON_MAPPING_SUCCESS_WITH_DIFFER_PAIR = 0x4000 | 0x20000000,//映射成功，但两通道的类型不同
        OP_COMMON_MAPPING_CHANNEL_NOT_MATCHING = 0x4001 | 0x20000000,//通道间未连接
        OP_COMMON_MAPPING_EXSITED = 0x4002 | 0x20000000,//相同的映射配置已存在，不能再次设置
        OP_COMMON_MAPPING_VOLUME_USED = 0x4003 | 0x20000000,//不能对已使用的卷设置映射
        OP_COMMON_MAPPING_VOLUME_NOT_FOUND = 0x4004 | 0x20000000,//设置映射的卷未找到
        OP_COMMON_MAPPING_FAILED_WRITE_ITEM = 0x4005 | 0x20000000,//写卷映射配置信息失败
        OP_COMMON_MAPPING_FAILED_GET_LOCAL_WWN = 0x4006 | 0x20000000,//获取存储服务器的WWN失败
        OP_DELETE_COMMON_MAPPING_FAILED_DEL_ITEM = 0x4007 | 0x20000000,//删除卷映射配置信息失败
        OP_COMMON_MAPPING_TC_NOT_LOGINED = 0x4008 | 0x20000000,//映射对应的服务器通道未连接
        OP_COMMON_MAPPING_NOT_FOUND = 0x4009 | 0x20000000,//映射配置信息未找到
        OP_COMMON_MAPPING_LUN_USED = 0x400a | 0x20000000,//LUN已被使用
        OP_COMMON_MAPPING_IC_NOT_LOGINED = 0x400b | 0x20000000,//映射对应的客户端通道未连接
        OP_COMMON_MAPPING_CHANNEL_INDEX_NOT_FOUND = 0x400c | 0x20000000,//获取通道索引失败
        OP_REMOTE_MAPPING_CHANNEL_INDEX_NOT_FOUND = 0x400d | 0x20000000,//获取通道索引失败
        OP_DELETE_REMOTE_MAPPING_FAILED_DEL_ITEM = 0x400e | 0x20000000,//删除映射配置信息失败
        OP_REMOTE_MAPPING_EXSITED = 0x400f | 0x20000000,//相同的映射配置已存在
        OP_REMOTE_MAPPING_FAILED_WRITE_ITEM = 0x4010 | 0x20000000,//写卷映射配置信息失败
        OP_REMOTE_MAPPING_FAILED_GET_SERVER_WWN = 0x4011 | 0x20000000,//获取存储服务器的WWN失败
        OP_REMOTE_MAPPING_NOT_FOUND = 0x4012 | 0x20000000,//远程服务器的映射配置信息未找到

        OP_CREATE_SNAPSHOT_CAPACITY_UNPERMISSION = 0x5001 | 0x20000000,//对快照源卷建立的快照关系已大最大个数
        OP_CREATE_SNAPSHOT_FAILED_WRITE_ITEM = 0x5002 | 0x20000000,//写快照关系本地配置信息失败
        OP_SNAPSHOT_NOT_FOUND = 0x5003 | 0x20000000,//快照关系未找到
        OP_DELETE_SNAPSHOT_FAILED_DEL_ITEM = 0x5004 | 0x20000000,//删除快照关系本地配置信息失败
        OP_SNAPSHOT_ACTIVED = 0x5005 | 0x20000000,//快照关系已激活

        OP_CDP_MODE_SOURCE_VOLUME_NOT_FOUND = 0x6001 | 0x20000000,//未找到CDP快照保存卷
        OP_CDP_MODE_SOURCE_VOLUME_TOO_SMALL = 0x6002 | 0x20000000,//CDP快照保存卷容量太小,不能进行模式设置
        OP_CDP_MODE_MAKEING_VOLUME_USED = 0x6003 | 0x20000000,//不能将已使用的卷设置为CDP快照保存卷
        OP_CDP_MODE_REMOVE_WITH_SNAPSHOT = 0x6004 | 0x20000000,//不能删除仍有CDP快照的CDP模式设置
        OP_CDP_MODE_FAILED_DEL_ITEM = 0x6005 | 0x20000000,//删除CDP模式本地配置信息失败
        OP_CDP_MODE_FAILED_ACCESS_REGISTRY = 0x6006 | 0x20000000,//访问CDP模式设置注册表项失败
        OP_CDP_FAILED_GET_CDP_COUNT = 0x6007 | 0x20000000,//获取CDP快照个数失败
        OP_CDP_MODE_NOT_MAKE_SETTING = 0x6008 | 0x20000000,//CDP模式未设置
        OP_CDP_SOURCE_VOLUME_SPACE_NOT_ENOUGH = 0x6009 | 0x20000000,//CDP快照保存卷容量不够
        OP_CDP_CAPATICY_UNPERMISSION = 0x600a | 0x20000000,//已达CDP快照最大个数
        OP_CDP_RESTORE_FAILED_CREATE_NEW = 0x600b | 0x20000000,//恢复时，新建CDP快照失败
        OP_CDP_RESTORE_FAILED_CREATE_SNAPSHOT = 0x600c | 0x20000000,//恢复时,建立两存储卷间快照关系失败
        OP_CDP_RESTORE_FAILED_ENABLE_SNAPSHOT = 0x600d | 0x20000000,//恢复时,激活两存储卷间快照关系失败
        OP_BACKUP_MODE_CHANGE_WITH_RELATION = 0x7001 | 0x20000000,//有备份关系存在时不能将备份模式修改为同步模式
        OP_BACKUP_MODE_VOLUME_NOT_FOUND = 0x7002 | 0x20000000,//异步备份模式设置的存储卷未找到
        OP_BACKUP_MODE_MAKE_VOLUME_USED = 0x7003 | 0x20000000,//不能将已使用的卷设置为异步模式保存卷
        OP_BACKUP_FAILED_VERIFY_INITIATOR = 0x7004 | 0x20000000,//OSNAPIVerifyInitiator调用失败
        OP_BACKUP_BAKCFG_NOT_FOUND = 0x7005 | 0x20000000,//文件OSNServerBackup.cfg未找到
        OP_BACKUP_SERVER_NOT_FOUND = 0x7006 | 0x20000000,//未从本地配置文件找到备份服务器信息
        OP_BACKUP_FAILED_ACCESS_REGISTRY = 0x7007 | 0x20000000,//创建并激活备份关系时访问注册表失败
        OP_BACKUP_FAILED_GET_STATUS = 0x7008 | 0x20000000,//获取备份状态失败
        OP_DELETE_BACKUP_STATE_UNPERMISSION = 0x7009 | 0x20000000,//备份关系是初始化或恢复状态时不能被删除
        OP_BACKUP_FAILED_WRITE_ITEM = 0x700a | 0x20000000,//写本地备份关系配置信息失败
        OP_BACKUP_FAILED_DEL_ITEM = 0x700b | 0x20000000,//删除本地备份关系配置信息失败
        OP_BACKUP_NOT_FOUND = 0x700c | 0x20000000,//备份关系未找到
        OP_MIRROR_FAILED_GET_SERVER_WWN = 0x8001 | 0x20000000,//获取服务器的WWN失败
        OP_MIRROR_FAILED_CREATE_MIRRORCLIENT = 0x8002 | 0x20000000,//建立MirrorClient存储应用服务器失败
        OP_MIRROR_SERVER_NOT_FOUND = 0x8003 | 0x20000000,//镜像服务器信息未能从本地配置信息找到
        OP_MIRROR_CHANNEL_NOT_LINKED = 0x8004 | 0x20000000,//通道未建立连接
        OP_MIRROR_FAILED_WRITE_CHANNEL_ITEM = 0x8005 | 0x20000000,//写mirror的initiator channel配置信息失败
        OP_MIRROR_FAILED_WRITE_MAPPING_ITEM = 0x8006 | 0x20000000,//写mirror的本地mapping配置信息失败
        OP_MIRROR_FAILED_WRITE_MIRROR_ITEM = 0x8007 | 0x20000000,//写mirror的本地配置信息失败
        OP_MIRROR_FAILED_SET_PRIMARY = 0x8008 | 0x20000000,//镜像主服务器端设置mirror失败
        OP_MIRROR_FAILED_CONFIG_REGISTRY = 0x8009 | 0x20000000,//修改mirror的注册表项失败
        OP_MIRROR_FAILED_MAPPING_VOLUME = 0x800a | 0x20000000,//设置mirror mapping失败
        OP_MIRROR_FAILED_DEL_MAPPING_VOLUME = 0x800b | 0x20000000,//删除mirror mapping失败
        OP_MIRROR_FAILED_SET_SUBSIDIARY = 0x800c | 0x20000000,//镜像从服务器端设置mirror失败
        OP_MIRROR_FAILED_GET_MIRROR_ITEM = 0x800d | 0x20000000,//镜像配置信息未从本地配置文件获得
        OP_MIRROR_FAILED_GET_STATUS = 0x800e | 0x20000000,//获取镜像状态失败
        OP_MIRROR_DELETE_UNPERMISSION = 0x800f | 0x20000000,//镜像状态为recovery,initialize,devicetesting时不允许被删除
        OP_MIRROR_DELETE_WITH_COMMON_MAPPING = 0x8010 | 0x20000000,//镜像卷被映射，不能删除
        OP_MIRROR_FAILED_DEL_MAPPING_ITEM = 0x8011 | 0x20000000,//删除mirror的本地mapping配置信息失败
        OP_MIRROR_FAILED_DEL_MIRROR_ITEM = 0x8012 | 0x20000000,//删除mirror的本地配置信息失败
        OP_MIRROR_INITIALIZING_OR_RECOVERYING = 0x8013 | 0x20000000,//镜像正在初始化或恢复中
        OP_MIRROR_NOT_FOUND_IN_DRIVER = 0x8014 | 0x20000000,//未找到镜像

        OP_NAS_INVALID_HOST_GROUP_NAME = 0x9001 | 0x20000000,//主机组名无效
        OP_NAS_HOST_GROUP_NAME_CONFLICT = 0x9002 | 0x20000000,//主机组名冲突
        OP_NAS_HOST_GROUP_NOT_FOUND = 0x9003 | 0x20000000,//主机组未找到
        OP_NAS_HOST_GROUP_MEMBER_NOT_FOUND = 0x9004 | 0x20000000,//指定的主机组成员未找到
        OP_NAS_HOST_GROUP_MEMBER_CONFLICT = 0x9005 | 0x20000000,//主机组成员冲突
        OP_NAS_INVALID_HOST_GROUP_MEMBER = 0x9006 | 0x20000000,//主机组成员无效
        OP_NAS_DELETE_HOST_GROUP_WITH_MEMBER = 0x9007 | 0x20000000,//不能删除还有主机成员的组
        OP_NAS_INVALID_USER_GROUP_NAME = 0x9008 | 0x20000000,//用户组名无效
        OP_NAS_USER_GROUP_NAME_CONFLICT = 0x9009 | 0x20000000,//用户组名冲突
        OP_NAS_USER_GROUP_ID_CONFLICT = 0x900a | 0x20000000,////用户组ID冲突
        OP_NAS_USER_GROUP_NOT_FOUND = 0x900b | 0x20000000,//用户组未找到
        OP_NAS_DELETE_USER_GROUP_WITH_MEMBER = 0x900c | 0x20000000,//不能删除还有用户成员的组
        OP_NAS_USER_GROUP_MEMBER_NOT_FOUND = 0x900d | 0x20000000,//指定的用户组成员未找到
        OP_NAS_INVALID_USER_GROUP_MEMBER_NAME = 0x900e | 0x20000000,//用户组成员名无效
        OP_NAS_USER_GROUP_MEMBER_NAME_CONFLICT = 0x900f | 0x20000000,//用户组成员名称冲突
        OP_NAS_USER_GROUP_MEMBER_ID_CONFLICT = 0x9010 | 0x20000000,//用户组成员ID冲突
        OP_NAS_FAILED_ADD_DOMAIN_MAP = 0x9011 | 0x20000000,//adddomainmap -d失败
        OP_NAS_USER_MAPPING_EXSITED = 0x9012 | 0x20000000,//用户映射设置已存在
        OP_NAS_FAILED_SET_PRIMARY_MAPPING = 0x9013 | 0x20000000,//设置主映射失败
        OP_NAS_DELETE_SIMPLE_NAME_MAPPING = 0x9014 | 0x20000000,//应通过取消简单名字映射删除
        OP_NAS_USER_NAME_MAPPING_NOT_FOUND = 0x9015 | 0x20000000,//名字映射设置未找到
        OP_NAS_DELETE_PRIMARY_USER_NAME_MAPPING = 0x9016 | 0x20000000,//对UNIX用户而言，如果有其它映射设置，主映射不能删除
        OP_NAS_REPEAT_SET_PRIMARY_USER_NAME_MAPPING = 0x9017 | 0x20000000,//重复设置主映射
        OP_NAS_FAILED_GET_IMAGE_PATH = 0x9018 | 0x20000000,//从注册表读取imagepath失败
        OP_NAS_VG_NOT_FOUND = 0x9019 | 0x20000000,//卷组未找到
        OP_NAS_VG_NOT_CONFIGED = 0x901a | 0x20000000,//卷组未配置(初始化设置、文件系统等）
        OP_NAS_VG_SHARE_EXSITED = 0x901b | 0x20000000,//卷组已设置NAS共享
        OP_NAS_VG_CIFS_SHARE_NAME_CONFLICT = 0x901c | 0x20000000,//CIFS共享名冲突
        OP_NAS_VG_NFS_SHARE_NAME_CONFLICT = 0x901d | 0x20000000,//NFS共享名冲突
        OP_NAS_VG_FAILED_MAKE_CIFS_SHARE = 0x901e | 0x20000000,//设置CIFS共享失败
        OP_NAS_VG_FAILED_MAKE_NFS_SHARE = 0x901f | 0x20000000,//设置NFS共享失败
        OP_NAS_VG_INVALID_NAS_SHARE = 0x9020 | 0x20000000,//无效的卷组NAS共享
        OP_NAS_VG_CIFS_SHARE_NAME_EXSITED = 0x9021 | 0x20000000,//CIFS共享已设置
        OP_NAS_VG_INVALID_CIFS_SHARE_NAME = 0x9022 | 0x20000000,//CIFS共享名无效
        OP_NAS_VG_FAILED_DEL_CIFS_SHARE = 0x9023 | 0x20000000,//删除CIFS共享失败
        OP_NAS_VG_FAILED_DEL_NFS_SHARE = 0x9024 | 0x20000000,//删除NFS共享失败
        OP_NAS_VG_NFS_SHARE_NAME_EXSITED = 0x9025 | 0x20000000,//NFS共享已设置
        OP_NAS_VG_INVALID_NFS_SHARE_NAME = 0x9026 | 0x20000000,//NFS共享名无效
        OP_NAS_VG_INVALID_SYSTEM_ACCOUNT = 0x9027 | 0x20000000,//无效的系统用户或组
        OP_NAS_VG_INVALID_NAME = 0x9028 | 0x20000000,//卷组名称无效
        OP_NAS_VG_NAME_CONFLICT = 0x9029 | 0x20000000,//卷组名冲突
        OP_NAS_DELETE_CONFIGED_VG = 0x902a | 0x20000000,//不能删除已配置的卷组
        OP_NAS_DELETE_VG_WITH_MEMBER = 0x902b | 0x20000000,//应先删除卷组成员
        OP_NAS_EXTEND_VG_EXCEED_MAX_NUMBER = 0x902c | 0x20000000,//最多可以有32个卷组成员
        OP_NAS_UNPERMIT_EXTEND_CONFIGED_STRIPE_VG = 0x902d | 0x20000000,//不允许扩容已配置的stripe类型卷组
        OP_NAS_VG_FAILED_LOOPBACK_VOLUME = 0x902e | 0x20000000,//loopback卷失败
        OP_NAS_VG_FAILED_GET_DISK_INDEX = 0x902f | 0x20000000,//获取已loopback的卷在系统中的磁盘索引失败
        OP_NAS_VG_FAILED_INITIALIZE_DISK = 0x9030 | 0x20000000,//初始化磁盘（转化成动态磁盘）失败
        OP_NAS_VG_FAILED_GET_DISK_LABEL = 0x9031 | 0x20000000,//获取磁盘的label失败
        OP_NAS_UNPERMIT_SHRINK_CONFIGED_VG = 0x9032 | 0x20000000,//不能从已配置卷组中删除卷成员
        OP_NAS_UNPERMIT_SHRINK_SHARED_VG = 0x9033 | 0x20000000,//不能从已配置nas共享的卷组中删除卷成员
        OP_NAS_CONFIG_VG_NO_ENOUGH_MEMBER = 0x9034 | 0x20000000,//配置卷组，但成员数量不够
        OP_NAS_VG_CONFIGED = 0x9035 | 0x20000000,//卷组已配置
        OP_NAS_VG_ERROR_CALL_METHOD = 0x9036 | 0x20000000,//对卷组操作调用的方法不对
        OP_NAS_VG_FAILED_CREATE_SIMPLE_VOLUME = 0x9037 | 0x20000000,//OsnNasApi.CreateSimpleVolume调用失败
        OP_NAS_VG_FAILED_CREATE_STRIPE_VOLUME = 0x9038 | 0x20000000,//OsnNasApi.CreateStripeVolume调用失败
        OP_NAS_VG_FAILED_CREATE_FOLDER = 0x9039 | 0x20000000,//建文件夹OsnNas失败
        OP_NAS_VG_FAILED_INITIALIZE_FILE_SYSTEM = 0x9040 | 0x20000000,//初始化文件系统失败
        OP_NAS_FAILED_CREATE_NASGROUP_ACCOUNT = 0x9041 | 0x20000000,//建NASGroup系统用户组失败
        OP_NAS_FAILED_CREATE_ACCOUNT = 0x9042 | 0x20000000,//建系统用户失败
        OP_NAS_CREATE_ACCOUNT_WITH_CONFLICT = 0x9043 | 0x20000000,//系统用户冲突
        OP_NAS_GROUP_NAME_MAPPING_EXSITED = 0x9044 | 0x20000000,//组名映射设置已存在
        OP_NAS_GROUP_NAME_MAPPING_NOT_FOUND = 0x9045 | 0x20000000,//组名映射设置未找到
        OP_NAS_DELETE_PRIMARY_GROUP_NAME_MAPPING = 0x9046 | 0x20000000,//对UNIX用户组而言，如果有其它映射设置，主映射不能删除
        OP_NAS_REPEAT_SET_PRIMARY_GROUP_NAME_MAPPING = 0x9047 | 0x20000000,//重复设置用户组主映射

        OP_RS_VOLUME_USED = 0xa001 | 0x20000000,//卷正在使用中
        OP_RS_VOLUME_NOT_FOUND = 0xa002 | 0x20000000,//卷未找到
        OP_RS_WRITE_ITEM_FAILED = 0xa003 | 0x20000000,//写本地RS备份信息失败
        OP_RS_DELETE_ITEM_FAILED = 0xa004 | 0x20000000,//写本地RS备份信息失败
        OP_RS_BACKUP_NOT_FOUND = 0xa005 | 0x20000000,//本地RS备份信息未找到

        OP_LUNMIRROR_FAILED_WRITE_ITEM = 0x10001 | 0x20000000,//本地写Lun Mirror信息失败
        OP_LUNMIRROR_VOLUME_USED = 0x10002 | 0x20000000,//卷正在使用中
        OP_LUNMIRROR_VOLUME_NOT_FOUND = 0x10003 | 0x20000000,//卷未找到
        OP_LUNMIRROR_NOT_FOUND = 0x10004 | 0x20000000,//本地Lun Mirror信息未找到
        OP_LUNMIRROR_FAILED_DEL_ITEM = 0x10005 | 0x20000000,//本地删除Lun Mirror信息失败
        OP_LUNMIRROR_SOURCE_ASYNC_MAKED = 0x10006 | 0x20000000,//源卷已设置异步Lun Mirror
        OP_LUNMIRROR_SOURCE_SYNC_MAKED = 0x10007 | 0x20000000,//源卷已设置同步Lun Mirror
        OP_LUNMIRROR_SOURCE_SYNC_MAXIMUM = 0x10008 | 0x20000000,//对同一源卷设置同步Lun Mirror达最大数

        OP_VVG_NAME_CONFLICT = 0x20001 | 0x20000000,//Virtual Volume Group名称冲突
        OP_VVG_NOT_FOUND = 0x20002 | 0x20000000,//Virtual Volume Group未找到
        OP_VVG_USED = 0x20003 | 0x20000000,//Virtual Volume Group正在使用中
        OP_VVG_CDPSETTING_FAILED_WRITE_ITEM = 0x20004 | 0x20000000,
        OP_VVG_CDPSETTING_FAILED_DEL_ITEM = 0x20005 | 0x20000000,
        OP_VVG_CDPSETTING_NOT_FOUND = 0x20006 | 0x20000000,
        OP_VVG_CDPSETTING_FOUND = 0x20007 | 0x20000000,

        OP_SCHEDULE_EXSITED = 0x30001 | 0x20000000,//任务计划已经设置
        OP_SCHEDULE_NOT_FOUND = 0x30002 | 0x20000000,
        OP_SCHEDULE_NOT_SUPPORT = 0x30003 | 0x20000000,

        OP_TARGETCHANNEL_REMOVE_WITH_MAPPING = 0x40001 | 0x20000000,
        OP_TARGETCHANNEL_FAILED_DEL_ITEM = 0x40002 | 0x20000000,
        OP_TARGETCHANNEL_NOT_FOUND = 0x40003 | 0x20000000,

        //0x2a000000已由客户端使用
        OP_DB_REMOVE_SERVER_SUCCESS = 0x0001 | 0x2b000000,//从数据库删除服务器记录成功
        OP_DB_REMOVE_SERVER_FAILED = 0x0002 | 0x2b000000,//从数据库删除服务器记录失败
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

        OP_DB_CONNECT_FAILED = 0xfffa | 0x2b000000,//数据库连接失败
        OP_DB_FAILED = 0xfffe | 0x2b000000,//数据库操作一般性错误
        OP_DB_EXCEPTION = 0xffff | 0x2b000000,//数据库操作发生异常


        OP_PROXY_FAILED_GET_SERVER_INFOMATION = 0x0001 | 0x2c000000,
        OP_PROXY_FAILED = 0x0002 | 0x2c000000,
        OP_PROXY_COMPILE_ASSEMBLE_FAILED = 0xfffe | 0x2c000000,//编译动态调用代理类失败
        OP_PROXY_EXCEPTION = 0xffff | 0x2c000000,

        OP_DEL_ITEM_SUCCESS_WITH_FAILURE = 0x0001 | 0x2d000000,//删除本地配置条目成功，但调用API失败

        OP_SEND_CMD_FAILED = 0x0001 | 0x2e000000,//socket传送命令失败

        OP_UnPermission = 0xfffe | 0x2f000000,//无权限
        OP_EXCEPTION = 0xffff | 0x2f000000,//异常
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
        RS_SUCCESS = 0x0,//成功
        RS_ERROR_NOT_FIND_TARGET = 0x00001 | 0x2A000000,  //失败:Client配置信息中未能找到该Target信息
        RS_ERROR_BACKUP_EXISTED = 0x00002 | 0x2A000000,  //失败:新建备份组时，Client配置信息中已存在该备份组信息
        RS_ERROR_NOT_FIND_DISK = 0x00003 | 0x2A000000,  //失败:Client中找不到相应磁盘或卷
        RS_ERROR_API_ADD_BACKUP = 0x00004 | 0x2A000000,  //失败:Client中调用OSNAddBackupInfo函数出错
        RS_ERROR_API_SET_DEVICE_ACTIVE = 0x00005 | 0x2A000000,  //失败:Client中调用OSNSetDeviceActiveBackup函数出错
        RS_ERROR_API_SET_VOLUME_ACTIVE = 0x00006 | 0x2A000000,  //失败:Client中调用OSNSetVolumeActiveBackup函数出错
        RS_ERROR_API_GET_DISK_INIT_STATUS = 0x00007 | 0x2A000000,  //失败:Client中调用OSNCheckDiskInitInfo函数出错
        RS_ERROR_API_GET_DISK_MIRROR_STATUS = 0x00008 | 0x2A000000,  //失败:Client中调用OSNGetDeviceState函数出错
        RS_ERROR_API_GET_VOLUME_INIT_STATUS = 0x00009 | 0x2A000000,  //失败:Client中调用OSNCheckVolumeInitInfo函数出错
        RS_ERROR_API_GET_VOLUME_MIRROR_STATUS = 0x0000A | 0x2A000000,  //失败:Client中调用OSNGetVolumeState函数出错
        RS_ERROR_BACKUP_NOT_EXISTED = 0x0000B | 0x2A000000,  //失败:Client配置信息中未找到此备份组信息
        RS_ERROR_API_UNSET_DEVICE_ACTIVE = 0x0000C | 0x2A000000,  //失败:Client中调用OSNUnsetDeviceActiveBackup函数出错
        RS_ERROR_API_UNSET_VOLUME_ACTIVE = 0x0000D | 0x2A000000,  //失败:Client中调用OSNUnsetVolumeActiveBackup函数出错
        RS_ERROR_API_REMOVE_BACKUP = 0x0000E | 0x2A000000,  //失败:Client中调用OSNRemoveBackupInfo函数出错
        RS_ERROR_BACKUP_DISABLED = 0x0000F | 0x2A000000,  //失败:Client中备份组为Disabled状态
        RS_ERROR_API_INIT_DISK_BACKUP = 0x00010 | 0x2A000000,  //失败:Client中调用OSNInitializeDevice函数出错
        RS_ERROR_API_INIT_VOLUME_BACKUP = 0x00011 | 0x2A000000,  //失败:Client中调用OSNInitializeVolume函数出错
        RS_ERROR_API_CANCEL_INIT_DISK_BACKUP = 0x00012 | 0x2A000000,  //失败:Client中调用OSNCancelInitializeDevice函数出错
        RS_ERROR_API_CANCEL_INIT_VOLUME_BACKUP = 0x00013 | 0x2A000000,  //失败:Client中调用OSNCancelInitializeVolume函数出错
        RS_ERROR_OPEN_REPLICATOR_REGKEY = 0x00014 | 0x2A000000,  //失败:Client中打开"SOFTWARE\\Enterprise Information Management\\OSN Replicator"注册表出错
        RS_ERROR_OPEN_OSNSERVICE_PARA_REGKEY = 0x00015 | 0x2A000000,  //失败:Client中打开"System\\CurrentControlSet\\Services\\OSNService\\Parameters"注册表出错
        RS_ERROR_SET_SAME_REPLICATION_MODE = 0x00016 | 0x2A000000,  //失败:Client中原先的备份模式和所要求的备份模式相同
        RS_ERROR_CANNOT_SET_SYNCH_MODE_BACKUP_EXISTED = 0x00017 | 0x2A000000,  //失败:Client中由于已经建立了备份组，不能由异步备份模式改为同步模式
        RS_ERROR_API_GET_LOG_VOLUME_DEVICE_NAME = 0x00018 | 0x2A000000,  //失败:Client中调用OSNGetLogVolumeDevice函数出错
        RS_ERROR_LOG_DEVICE_BACKUPED = 0x00019 | 0x2A000000,  //失败:Client中要求设置的异步备份保存卷已建立了备份组，请选择其它异步备份保存卷
        RS_ERROR_API_SET_REPLICATION_MODE = 0x0001A | 0x2A000000,  //失败:Client中调用OSNSetReplicationMode函数出错
        RS_ERROR_API_GET_SYSTEM_WWN = 0x0001B | 0x2A000000,  //失败:Client中调用OSNGetCharWWN函数出错
        RS_ERROR_NOT_FIND_CONFIG_FILE = 0x0001C | 0x2A000000,  //失败:Client目录下未找到配置文件
        RS_ERROR_FILE_TARGET_LINE_ILLEGAL_FORMAT = 0x0001D | 0x2A000000,  //失败:Client配置文件中，target记录行格式有误
        RS_ERROR_FILE_BACKUP_LINE_ILLEGAL_FORMAT = 0x0001E | 0x2A000000,  //失败:Client配置文件中，backup记录行格式有误
        RS_ERROR_DELETE_SERVER_BACKUP_EXISTED = 0x0001F | 0x2A000000,  //失败:Client中仍建立有备份组，所以不能删除服务器
        RS_ERROR_API_REMOVE_TARGET_INFO = 0x00020 | 0x2A000000,  //失败:Client中调用OSNRemoveTargetInfo函数出错
        RS_ERROR_FIND_TARGET = 0x00021 | 0x2A000000,  //失败:Client配置信息中已经存在该Target信息
        RS_ERROR_API_ADD_TARGET_INFO = 0x00022 | 0x2A000000,  //失败:Client中调用OSNAddTargetInfo函数出错
        RS_ERROR_API_GET_VOLUME_ID = 0x00023 | 0x2A000000,  //失败:Client中调用OSNGetVolumeID函数出错 
        RS_ERROR_API_GET_VOLUME_SIZE = 0x00024 | 0x2A000000,  //失败:Client中调用OSNVfGetVolumeSize函数出错 
        RS_ERROR_API_CATCH_EXCEPTION = 0x00025 | 0x2A000000,  //失败:Client中API函数调用中捕获到异常
        RS_SUCCESS_START_CREATE_CDP = 0x00026 | 0x2A000000,  //成功:Client中开始新建CDP
        RS_SUCCESS_FINISH_CREATE_CDP = 0x00027 | 0x2A000000,  //成功:Client中完成新建CDP
        RS_ERROR_NOT_FIND_RAWPARTITION = 0x00028 | 0x2A000000,  //失败：Client中没有可以作为异步保存盘的裸卷
        RS_ERROR_FILE_CREATE_CDP = 0x00029 | 0x2A000000,   //失败：Client端创建CDP失败
        RS_ERROR_SET_ASYNCH_LOG_DEVICE_BACKUP_EXISTED = 0x0002A | 0x2A000000,   //失败：Client端设置新的异步保存卷失败，由于原有异步保存卷上已存在备份组信息
        RS_ERROR_BACKUP_INITIALIZE = 0x002B | 0x2A000000,//失败备份组正在初始化
        RS_ERROR_API_GET_DISK_RECOVERY_STATUS = 0x002C | 0x2A000000,//失败：获取磁盘的恢复信息失败 
        RS_ERROR_API_GET_VOLUME_RECOVERY_STATUS = 0x002D | 0x2A000000,//失败：获取卷的恢复信息失败
    };
    [Serializable]
    public enum RpcServiceCommand
    {
        OSN_RC_CMD_GET_DISKLIST = 0x00000,   //获取Client磁盘或卷信息，输入参数为null

        OSN_RC_CMD_CREATE_BACKUPINFO = 0x00001,   //通知Client创建备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
        //(1): DesTargetName
        //(2): DesVolumeID

        OSN_RC_CMD_DELETE_BACKUPINFO = 0x00002,   //通知Client删除备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8)),
        //(1): DesTargetName

        OSN_RC_CMD_GET_BACKUP_STATUS = 0x00003,   //获取Client备份组状态，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_INITIALIZE_BACKUP = 0x00004,   //通知Client初始化备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_CANCEL_INITI_BACKUP = 0x00005,   //通知Client取消初始化备份组，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_GET_REPLICATE_MODE = 0x00006,   //获取Client备份组模式，输入参数为null

        OSN_RC_CMD_SET_REPLICATE_MODE = 0x00007,   //获取Client备份组模式，输入参数为(0):_REPLICATION_MODE(MODE_ASYNCH或MODE_SYNCH) 
        //(1):LogDeviceLabel (C:或D:)

        OSN_RC_CMD_GET_INITIATOR_INFO = 0x00008,   //获取Client的相关信息，输入参数为null

        OSN_RC_CMD_READ_CONFIG_FILE = 0x00009,   //通知Client重新读取配置文件，输入参数为null （客户端添加服务器时使用）

        OSN_RC_CMD_DELETE_TARGET_INFO = 0x0000A,   //通知Client删除服务器，输入参数为(0):TargetName
        //(1):TargetIP

        OSN_RC_CMD_CREATE_CDP = 0x00010,   //通知Client创建CDP，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

        OSN_RC_CMD_ADD_TARGET_INFO = 0x00011,   //通知Client添加服务器，输入参数为(0): TargetName
        //(1): TargetIPAddress
        //(2): ClientIPAddress
        OSN_RC_CMD_CREATE_CDP_SCHEDULE = 0x00012,   //通知Client添加CDP任务计划，输入参数为(0): TargetName
        //(1): TargetIPAddress
        //(2): SrcDiskSign
        //(3): CDPStartTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
        //WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
        //Second:1,2...,59;)
        //(4): CDPStopTime (Example:  CDPStartTime: Year:2009; Month:1,2,3...,12; Day:1,2,3...31; 
        //WeekNum:1,2...,4,5; DayOfWeek:0,1,...6; Hour:0,1,2,...23; Minute:1,2,..,59;
        //Second:1,2...,59;)
        //(5): CDPMode
        OSN_RC_CMD_DELETE_CDP_SCHEDULE = 0x00013,    //通知Client删除CDP任务计划，输入参数为(0): SrcDiskSign

        OSN_RC_CMD_SET_STATUSERVICEIP = 0x00014,    //通知Client修改BackupStatuThread的发送的目的地址，输入参数为（0）：StatuServiceIP
        OSN_RC_CMD_GET_RAWPARTITION = 0x00015,    //通知Client将其裸卷列表发给服务端
        OSN_RC_CMD_GET_CDP_SCHEDULE = 0x00016,    //通知Client获取CDP任务计划描述, 输入参数为(0):TargetName  (1):SrcDiskSign
        OSN_RC_CMD_CREATE_TIMECDP = 0x00017,    //通知Client创建CDP，采用IOCTL_OSNCFG_RC_TIME_MARKER的方式，输入参数为(0): SrcDiskSignature(Signature(X8)_Offset(X8))

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
        /// 转化为字符串形式
        /// </summary>
        /// <returns>NULL表示格式不对</returns>
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
                case DayOfWeek.Sunday: result = "星期日"; break;
                case DayOfWeek.Saturday: result = "星期六"; break;
                case DayOfWeek.Friday: result = "星期五"; break;
                case DayOfWeek.Thursday: result = "星期四"; break;
                case DayOfWeek.Wednesday: result = "星期三"; break;
                case DayOfWeek.Tuesday: result = "星期二"; break;
                case DayOfWeek.Monday: result = "星期一"; break;
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
                description = String.Format("从{0}年{1}月{2}日{3}时{4}分开始，每{5}分钟", StartDates[0].Year,
                                             StartDates[0].Month, StartDates[0].Day, StartTime.Hour,
                                             StartTime.Minute, Modifier.IntervalMinutes);
            }
            else if (ScheduleMode.MODE_DAYLY == Mode)
            {
                description = String.Format("从{0}年{1}月{2}日开始，每{3}天", StartDates[0].Year,
                                             StartDates[0].Month, StartDates[0].Day, Modifier.IntervalDays);
            }
            else if (ScheduleMode.MODE_WEEKLY == Mode)
            {
                foreach (DayOfWeek dayWeek in Modifier.Week.Days)
                {
                    list.Add(FormatDayOfWeek(dayWeek));
                }
                description = String.Format("在{0}的{1}时{2}分", String.Join("、", list.ToArray()),
                                             StartTime.Hour, StartTime.Minute);
            }
            else if (ScheduleMode.MODE_MONTHLY == Mode)
            {
                foreach (Date date in StartDates)
                {
                    list.Add(date.Month.ToString());
                }
                String month = String.Join("、", list.ToArray());
                if (Modifier != null)
                {
                    String week = FormatDayOfWeek(Modifier.Week.Days[0]);
                    description = String.Format("在{0}月的第{1}个{2}{3}时{4}分", month, (int)Modifier.Week.Base,
                                                 week, StartTime.Hour, StartTime.Minute);
                }
                else
                {
                    description = String.Format("在{0}月的第{1}天{2}时{3}", month, StartDates[0].Day,
                                                 StartTime.Hour, StartTime.Minute);
                }
            }
            else
            {
                description = "未知任务计划.";
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
        public String Schedule;//cdpschedule format string or interval(分钟）
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
