#ifndef		_OSN_LICENCE_H_
#define		_OSN_LICENCE_H_




//Licence error

#define	LICENCE_SUCCESS					0
#define	LICENCE_FILE_NOT_FOUND			1
#define LICENCE_KEY_NOT_FOUND			2
#define LICENCE_FILE_INVALID_KEY		3
#define	LICENCE_FILE_UNMATCHED_KEY		4
#define LICENCE_REGISTRY_INVALID		5
#define LICENCE_EXPIRED					6
#define LICENCE_INVALID_DATE			7
#define LICENCE_INVALID_IMAGE_PATH		8


#define	PRODUCT_KEY_SIZE				38
#define LICENCE_KEY_SIZE				53
#define	LICENCE_EVALUATION_PERIOD		30		//30 day evaluation

#define TrialProductKey					"30OSNREPLI-SOLU-TION-V300-EVALUATION11"
#define NullProductKey					"00000000-0000-0000-0000-000000000000"
#define SANTrialProductKey				"41OSNSANSE-RVER-QAWS-V410-EVALUATION11"
#define CDPTrialProductKey				"41OSNCDPSE-RVER-PWRU-V410-EVALUATION11"
#define NASTrialProductKey				"41OSNNASSE-RVER-YTRV-V410-EVALUATION11"
#define HAExpressTrialProductKey		"50OSNHAXPR-RVER-YTRV-V500-EVALUATION11"
#define StreamerTrialProductKey		"50OSNCDPSM-RVER-YTRV-V500-EVALUATION11"
#define DataGuardTrialProductKey     "50OSNGUARD-RVER-YTRV-V500-EVALUATION11"
#define FullTrialProductKey				TrialProductKey


#define			LICENSE_FC_SUPPORT					0x01
#define			LICENSE_HA_SUPPORT					0x02
#define			LICENSE_CDP_SUPPORT					0x04
#define			LICENSE_REPLICATION_SUPPORT			0x08
#define			LICENSE_SNAPSHOT_SUPPORT			0x10
#define			LICENSE_DM_SUPPORT					0x20
#define			LICENSE_TVM_SUPPORT					0x40
#define			LICENSE_LUNMIRROR_SUPPORT			0x80
#define			LICENSE_CLIENT_REPLICATION_SUPPORT	0x4000

#define			LICENSE_SAN_BASE_SUPPORT			0x100
#define			LICENSE_NAS_BASE_SUPPORT			0x200

#define			LICENSE_DR_SOURCE_SUPPORT			0x1000
#define			LICENSE_DR_TARGET_SUPPORT			0x2000

#define			LICENSE_FULL_TRAIL					0x01
#define			LICENSE_SAN_TRAIL					0x02
#define			LICENSE_CDP_TRAIL					0x04
#define			LICENSE_NAS_TRAIL					0x08
#define			LICENSE_FULL_COTROL					0x10
#define			LICENSE_DATAGUARD_TRAIL				0x20
#define			LICENSE_HAEXPRESS_TRAIL				0x40
#define			LICENSE_STREAMER_TRAIL				0x80





int	OSNLicenceDecode(char	*encodedKey,		//IN, key in licence file
					 char	*decodedKey,		//Out
					 int	*pClient,			//Out, maximum allowed clients
					 int	*pCapacity,			//Out, maximum allowed disk capacity in pool
					 int	*pFeatures,			//Out, feature bits
					 bool	*pTrial);			//Out, trial version


int	OSNLicenceDecode2(char	*encodedKey,		//IN, key in licence file
					 char	*decodedKey,		//Out
					 int	*pClient,			//Out, maximum allowed clients
					 int	*pCapacity,			//Out, maximum allowed disk capacity in pool
					 int	*pFeatures,			//Out, feature bits
					 int	*pBaselicence);		//Out, trial version or full control

int	OSNLicenceDecode2New(char	*encodedKey,		//IN, key in licence file
						 char	*decodedKey,		//Out
						 int	*pClient,			//Out, maximum allowed clients
						 int	*pCapacity,			//Out, maximum allowed disk capacity in pool
						 int	*pFeatures,			//Out, feature bits
						 int	*pBaseLicence);		//Out, trial version or FULL control

int OSNLicenceEncode(char	*decodedKey,		//IN
					 int	client,				//IN, maximum allowed clients
					 int	capacity,			//IN, maximum allowed disk capacity in pool
					 int	features,			//IN, functionality bits
					 char	*encodedKey);		//OUT, key in licence file


int OSNLicenceEncodeNew(char	*decodedKey,		//IN
					 int	client,				//IN, maximum allowed clients
					 int	capacity,			//IN, maximum allowed disk capacity in pool
					 int	features,			//IN, functionality bits
					 char	*encodedKey);		//OUT, key in licence file

int OSNLicenceEncodeDate(int	year,			//in
						int		month,			//in
						int		day,			//in
						char	*encodedDate);	//OUT, MMDDYYYY



int OSNLicenceDecodeDate(char	*encodedDate,	//IN, MMDDYYYY
						int		*year,			//Out
						int		*month,			//out
						int		*day);			//OUT 



bool  OSNRetrieveSystemProductGuidNew(char *systemProductGuid);
bool OSNRetrieveSystemProductGuid(char *systemProductGuid);
bool OSNGetSystemProductKey(char *systemKey,bool newSchema);
bool OSNGetInstallationDate(int *pYear, int *pMonth, int *pDay);
int	 OSNVerifyLicence(char	*pInstallPath,		//IN,  installationPath
					 int	*pClient,			//Out, maximum allowed clients
					 int	*pCapacity,			//Out, maximum allowed disk capacity in pool
					 int	*pFeatures,			//Out, feature bits
					 bool	*pTrial);			//Out, trial version


int	 OSNVerifyLicence2(char	*pInstallPath,		//IN,  installationPath
					 int	*pClient,			//Out, maximum allowed clients
					 int	*pCapacity,			//Out, maximum allowed disk capacity in pool
					 int	*pFeatures,			//Out, feature bits
					 int	*pBaseLicence);		//Out, base licence for Full, SAN Trial, CDP Trail, Full Trial


//for snapstreamer
int		OSNLicenceSnapSGenerateProductKey(char *pSerialNumber, 
										  char *pProductKeyBuffer, 
										  int	bufferSize);

int		OSNLicenceSnapSCreateNewKey(char	*productKey,		//in,  productionKey
									char	*newKey);			//out, key into licence file

bool	OSNLicenceSnapSVerifyLicence(char *productKey);
//end snapstreamer



#endif	//_OSN_LICENCE_H_